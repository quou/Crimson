project "Crimson"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"Source/**.h",
		"Source/**.cpp",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.PhysFS}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}"
	}

	links {
		ExternalDependencies.GLFW,
		ExternalDependencies.Glad,
		ExternalDependencies.OpenGL,
		ExternalDependencies.PhysFS,
		ExternalDependencies.ImGui,
	}


	filter "configurations:Debug"
		runtime "Debug"
		defines {
			"DEBUG"
		}
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines {
			"RELEASE"
		}
		optimize "on"
