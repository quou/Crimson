-- Crimson premake file

group "ex"
include "ex/glfw"
include "ex/physfs"
include "ex/glad"
group ""

systemDeps = {}
if os.host() == "linux" then
	systemDeps = {
		"pthread",
		"dl",
		"X11",
		"GL",
		"glad"
	}
elseif os.host() == "windows" then
	systemDeps = {
		"opengl32"
	}
end

project "crimson"
	kind "SharedLib"

	language "C++"
	cppdialect "C++11"

	targetdir "../bin"

	includedirs {
		"src",
		"ex/glfw/include",
		"ex/glad/include",
		"ex/physfs/src",
		"ex/angelscript/angelscript/include",
		"ex/angelscript/angelscript/",
	}

	links {
		systemDeps,
		"glad",
		"physfs",
		"glfw"
	}

	files { "include/**.h", "src/**.h", "src/**.cpp" }

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		defines { "CR_LIBRARY_EXPORT" }

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
