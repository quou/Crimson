#include "logger.h"
#include "assets.h"
#include "utils/stb_image.h"
#include "errorassets/errorshader.h"

#include <physfs.h>

static const char* defaultShaderSource = R"(
#shader vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_uv;
out vec3 v_worldPos;
out vec3 v_normal;

void main() {
	v_uv = a_uv;
	v_normal = a_normal;
	v_worldPos = (u_model * vec4(a_position, 1.0)).xyz;

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}

#shader pixel
#version 330 core

in vec2 v_uv;
in vec3 v_worldPos;
in vec3 v_normal;
out vec4 v_color;

struct Material {
	vec3 color;
	float shininess;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct SkyLight {
	vec3 color;
	float intensity;
};

struct Sun {
	vec3 direction;
	vec3 color;
	float intensity;

	mat4 transform;
};

uniform vec3 u_cameraPosition;

uniform PointLight u_pointLights[100];
uniform int u_pointLightCount;

uniform SkyLight u_skyLights[5];
uniform int u_skyLightCount;

uniform bool u_useSun;
uniform bool u_shadowEnable;
uniform Sun u_sun;
uniform sampler2D u_shadowmap;

uniform Material u_material = Material(vec3(1.0, 0.0, 0.0), 32.0);

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - v_worldPos);
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), u_material.shininess) * light.intensity;
	
	float distance = length(light.position - v_worldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 diffuse = light.color * diff * u_material.color * light.intensity;
	vec3 specular = light.color * spec * u_material.color;
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

float CalculateDirectionalShadow(Sun light, vec3 normal, vec3 lightDir) {
	vec4 lightSpacePos = light.transform * vec4(v_worldPos, 1.0);
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closestDepth = texture(u_shadowmap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_shadowmap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(u_shadowmap, projCoords.xy + vec2(x, y) * texelSize).r;
			float pcf = currentDepth > pcfDepth ? 1.0 : 0.0;
			shadow += pcf;
		}
	}
	shadow /= 9.0;
	
	if (projCoords.z > 1.0) {
		shadow = 0.0;
	}
	return shadow;
}

vec3 CalculateSun(Sun light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	
    vec3 diffuse = light.color * diff * u_material.color * light.intensity;
    vec3 specular = light.color * spec * u_material.color * light.intensity;

	if (u_shadowEnable) {
    	return (1.0 - CalculateDirectionalShadow(light, normal, lightDir)) * (diffuse + specular);
	}
	return diffuse + specular;
}

void main() {
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(u_cameraPosition - v_worldPos);

	vec3 lightingResult = vec3(0.0);

	for (int i = 0; i < u_skyLightCount; i++) {
		lightingResult += u_material.color * u_skyLights[i].color * u_skyLights[i].intensity;
	}

	lightingResult += CalculateSun(u_sun, normal, viewDir);

	for (int i = 0; i < u_pointLightCount; i++) {
		lightingResult += CalculatePointLight(u_pointLights[i], normal, viewDir);
	}

	v_color = vec4(lightingResult, 1.0);
}
)";

namespace Crimson {
	void AssetManager::Init(const char* resDir) {
		instance().m_currentDir = resDir;

		if (PHYSFS_isInit()) { /* Incase we need to re-init */
			Quit();
		}

		PHYSFS_init(NULL);
		PHYSFS_mount(resDir, "/", 1);
		PHYSFS_setWriteDir(resDir);

		/* Init error assets */
		Shader::ShaderSource s = Shader::Parse(defaultShaderSource);
		instance().m_standardShader = ref<Shader>(new Shader(s.vertex.c_str(), s.pixel.c_str()));
	}

	std::string AssetManager::LoadTerminatedString(const char* path, bool reload) {
		AssetManager& i = instance();

		/* Check if we cannot use a cached file */
		if (i.m_terminatedStrings.count(path) == 0 || reload) {
			PHYSFS_file* file = PHYSFS_openRead(path);
			if (file == NULL) {
				Log(LogType::ERROR, "Failed to load: %s", path);
				return "";
			}

			size_t fileSize = PHYSFS_fileLength(file);

			char* buffer = (char*)malloc(fileSize + 1);

			size_t bytesRead = PHYSFS_readBytes(file, buffer, fileSize);
			buffer[bytesRead] = '\0'; /* Null-terminator */

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			i.m_terminatedStrings[path] = {buffer, stat.modtime};
			
			/* Cleanup */
			free(buffer);
			PHYSFS_close(file);
		}

		/* Return the cached version of the file */
		return i.m_terminatedStrings[path].first;
	}

	std::pair<unsigned char*, unsigned int> AssetManager::LoadBinary(const char* path, bool reload) {
		AssetManager& i = instance();

		/* Check if we cannot use a cached file */
		if (i.m_binary.count(path) == 0 || reload) {
			/* Make sure to free any existing data that might be cached */
			if (reload || i.m_binary[path].first.first != NULL) {
				free(i.m_binary[path].first.first);
			}

			PHYSFS_file* file = PHYSFS_openRead(path);
			if (file == NULL) {
				Log(LogType::ERROR, "Failed to load: %s", path);
				return {NULL, 0};
			}

			size_t fileSize = PHYSFS_fileLength(file);

			unsigned char* buffer = (unsigned char*)malloc(fileSize + 1);

			PHYSFS_readBytes(file, buffer, fileSize);

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			i.m_binary[path] = {{buffer, fileSize}, stat.modtime};
			
			/* Cleanup */
			PHYSFS_close(file);
		}

		/* Return the cached version of the file */
		return i.m_binary[path].first;
	}

	ref<Shader>& AssetManager::LoadShader(const char* path, bool reload) {
		AssetManager& i = instance();

		if (std::string(path) == "standard") {
			return i.m_standardShader;
		}

		/* Check if a cached version cannot be used */
		if (i.m_shaders.count(path) == 0 || reload) {
			/* Load the shader text */
			std::string source = LoadTerminatedString(path, reload);

			/* Parse the shader */
			Shader::ShaderSource parsedSource = Shader::Parse(source.c_str());

			/* To get the modtime */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			if (!source.empty()) {
				i.m_shaders[path] = {ref<Shader>(new Shader(
							parsedSource.vertex.c_str(),
							parsedSource.pixel.c_str())),
							stat.modtime};
			}
		}

		return i.m_shaders[path].first;
	}

	Texture* AssetManager::LoadTexture(const char* path, bool reload) {
		AssetManager& i = instance();

		if (i.m_textures.count(path) == 0 || reload) {
			/* Load the compressed binary data */
			unsigned char* rawData = LoadBinary(path, reload).first;
			if (!rawData) { return NULL; }

			unsigned char* pixels;
			int w, h, componentCount;
	
			/* To get the modtime & file size */
			PHYSFS_Stat stat;
			PHYSFS_stat(path, &stat);

			/* Uncompress and parse the data */
			pixels = stbi_load_from_memory(rawData, stat.filesize, &w, &h,
					&componentCount, 0);
			
			if (pixels == NULL) {
				Log(LogType::ERROR, "Failed to load %s: %s",
						path, stbi_failure_reason());
				return NULL;
			}

			i.m_textures[path] = {ref<Texture>(new Texture(
						pixels, w, h, componentCount)),
						stat.modtime};
		}

		return i.m_textures[path].first.get();
	}


	void AssetManager::WriteTerminatedString(const char* path, const std::string& text) {
		PHYSFS_File* file = PHYSFS_openWrite(path);

		if (file) {
			PHYSFS_writeBytes(file, text.c_str(), text.size());
			PHYSFS_close(file);
		} else {
			Log(LogType::ERROR, "Failed to write %s, %s", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
	}

	void AssetManager::HotReload() {
		AssetManager& i = instance();
		
		/* Loop over cached text files */
		for (auto& f : i.m_terminatedStrings) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadTerminatedString(f.first.c_str(), true);
			}
		}

		/* Loop over cached binary files */
		for (auto& f : i.m_binary) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadBinary(f.first.c_str(), true);
			}
		}

		/* Loop over cached shaders */
		for (auto& f : i.m_shaders) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadShader(f.first.c_str(), true);
			}
		}

		/* Loop over cached textures */
		for (auto& f : i.m_textures) {
			/* Grab the file stats */
			PHYSFS_Stat stat;
			PHYSFS_stat(f.first.c_str(), &stat);

			/* If there are new changes */
			if (stat.modtime > f.second.second) {
				/* Reload the file */
				LoadTexture(f.first.c_str(), true);
			}
		}

	}

	bool AssetManager::TextFileModified(const char* path) {
		auto f = instance().m_terminatedStrings[path];

		PHYSFS_Stat stat;
		PHYSFS_stat(path, &stat);

		/* If there are new changes */
		if (stat.modtime > f.second) {
			return true;
		}

		return false;
	}

	
	static std::string GetExtension(const std::string& fname) {
		auto idx = fname.rfind('.');
		std::string extension;

		if(idx != std::string::npos) {
			extension = fname.substr(idx+1);
		}

		return extension;
	}

	std::vector<std::pair<std::string, std::string>> AssetManager::GetDir(const std::string& dir) {
		std::vector<std::pair<std::string, std::string>> result;

		char **rc = PHYSFS_enumerateFiles(dir.c_str());
		char **i;
		for (i = rc; *i != NULL; i++) {
			PHYSFS_Stat stat;
			std::string fname = dir + "/" + *i;
			PHYSFS_stat(fname.c_str(), &stat);

			std::pair<std::string, std::string> currentEntry;

			if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
				auto a = result;
				auto b = GetDir(fname);
				a.insert(a.end(), b.begin(), b.end());
				result = a;
			} else {
				currentEntry.first = fname;
				currentEntry.second = GetExtension(fname);
				result.push_back(currentEntry);
			}

		}
		PHYSFS_freeList(rc);

		return result;
	}

	void AssetManager::Quit() {
		AssetManager& i = instance();

		/* Loop over cached binary files */
		for (auto& f : i.m_binary) {
			free(f.second.first.first);
		}

		i.m_binary.clear();
		i.m_textures.clear();
		i.m_terminatedStrings.clear();
		i.m_shaders.clear();

		PHYSFS_unmount(i.m_currentDir.c_str());

		PHYSFS_deinit();
	}
}
