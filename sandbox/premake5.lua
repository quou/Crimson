-- Sandbox premake file

project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"

	targetdir "../bin"

	files { "src/**.h", "src/**.cpp" }

	includedirs {
		"../crimson/ex/imgui/include",
		"../crimson/src"
	}

	links {
		crimsonExternalDeps,
		"crimson"
	}

	defines {
		"CR_STATIC"
	}

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
