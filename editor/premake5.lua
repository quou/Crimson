-- Sandbox premake file

project "editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"

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

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
