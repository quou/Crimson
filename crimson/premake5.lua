-- Crimson premake file

group "ex"
include "ex/glfw"
include "ex/physfs"
group ""

systemDeps = {}
if os.host() == "linux" then
	systemDeps = {
		"pthread",
		"dl",
		"X11",
		"GL"
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

	staticruntime "on"

	targetdir "../bin"

	files { "include/**.h", "src/**.h", "src/**.cpp" }

	includedirs {
		"src",
		"include",
		"ex/glfw/include",
		"ex/physfs/src",
		"ex/angelscript/angelscript/include",
		"ex/angelscript/angelscript/",
	}

	links {
		systemDeps,
		"physfs",
		"glfw"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
