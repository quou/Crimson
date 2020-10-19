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
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"Source"
	}

	filter "system:windows"
		links {
			"opengl32.lib"
		}
	filter "system:linux"
		links {
			"libGL.so"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
