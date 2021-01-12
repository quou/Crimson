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
	v_worldPos = vec3(u_model * vec4(a_position, 1.0f));

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

#shader pixel
#version 330 core

in vec2 v_uv;
in vec3 v_worldPos;
in vec3 v_normal;
out vec4 v_color;

struct Material {
	vec3 albedo;
	float metallic;
	float roughness;
};

struct PointLight {
	vec3 position;
	vec3 color;
};

uniform Material u_material = Material(vec3(1.0, 0.0, 0.0), 1.0, 0.3);

uniform PointLight u_pointLights[100];
uniform int u_pointLightCount;

uniform vec3 u_cameraPos;

const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}


void main() {
	vec3 N = normalize(v_normal);
    vec3 V = normalize(u_cameraPos - v_worldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, u_material.albedo, u_material.metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_pointLightCount; ++i) 
    {
		PointLight light = u_pointLights[i];

        // calculate per-light radiance
        vec3 L = normalize(light.position - v_worldPos);
        vec3 H = normalize(V + L);
        float distance = length(light.position - v_worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light.color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, u_material.roughness);   
        float G   = GeometrySmith(N, V, L, u_material.roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - u_material.metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * u_material.albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    vec3 ambient = vec3(0.03) * u_material.albedo * 0.1;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

	v_color = vec4(color, 1.0f);
}
