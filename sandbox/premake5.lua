-- Sandbox premake file

project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"

	targetdir "../bin"

	files { "src/**.h", "src/**.cpp" }

	includedirs {
		"../crimson/src"
	}

	links {
		"crimson"
	}

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
