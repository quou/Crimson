-- Crimson premake file

group "ex"
include "ex/tinyxml2"
include "ex/glfw"
include "ex/physfs"
include "ex/glad"
include "ex/imgui"
include "ex/angelscript"
group ""

crimsonSystemDeps = {}
if os.host() == "linux" then
	crimsonSystemDeps = {
		"pthread",
		"dl",
		"X11",
		"GL",
	}
elseif os.host() == "windows" then
	crimsonSystemDeps = {
		"opengl32"
	}
end

crimsonExternalDeps = {
	crimsonSystemDeps,
	"glad",
	"physfs",
	"glfw",
	"tinyxml2",
	"glad",
	"imgui",
	"angelscript"
}

project "crimson"
	kind "StaticLib"

	language "C++"
	cppdialect "C++11"

	targetdir "../bin"

	includedirs {
		"src",
		"ex/glfw/include",
		"ex/glad/include",
		"ex/imgui/include",
		"ex/physfs/src",
		"ex/tinyxml2/src",
		"ex/angelscript/angelscript/include",
		"ex/angelscript/angelscript/add_on",
	}

	links {
		crimsonExternalDeps,
	}

	files { "include/**.h", "src/**.h", "src/**.cpp" }

	defines {
		"GLFW_INCLUDE_NONE",
		"CR_STATIC"
	}

	filter "system:windows"
		defines { "CR_LIBRARY_EXPORT" }

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
