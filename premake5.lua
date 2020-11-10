require "premake-extensions/compiledb"

workspace "Crimson"
	architecture "x64"
	startproject "Game"

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}

	-- defines {
	-- 	"GLFW_INCLUDE_NONE"
	-- }

	linkgroups "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Dependencies/glfw/include"
IncludeDir["Glad"] = "Dependencies/glad/include"
IncludeDir["PhysFS"] = "Dependencies/physfs/src"
IncludeDir["ImGui"] = "Dependencies/imgui/include"
IncludeDir["GLM"] = "Dependencies/glm"
IncludeDir["Lua"] = "Dependencies/lua/src"
IncludeDir["ReactPhysics3D"] = "Dependencies/reactphysics3d/include"
IncludeDir["Entt"] = "Dependencies/entt/include"
IncludeDir["AngelScript"] = "Dependencies/angelscript/angelscript/include"
IncludeDir["AngelScriptAddOns"] = "Dependencies/angelscript/add_on"
IncludeDir["Minizip"] = "Dependencies/minizip"
IncludeDir["yamlcpp"] = "Dependencies/yaml-cpp/include"

ExternalDependencies = {}
ExternalDependencies["GLFW"] = {}
ExternalDependencies["OpenGL"] = {}
ExternalDependencies["ImGui"] = {"ImGui"}
ExternalDependencies["Glad"] = {"Glad"}
ExternalDependencies["PhysFS"] = {"PhysFS"}
ExternalDependencies["Lua"] = {"Lua"}
ExternalDependencies["ReactPhysics3D"] = {"ReactPhysics3D"}
ExternalDependencies["AngelScript"] = {"AngelScript"}
ExternalDependencies["Minizip"] = {"Minizip"}
ExternalDependencies["yamlcpp"] = {"yaml-cpp"}

if os.host() == "linux" then
	ExternalDependencies["GLFW"] = {
		"pthread",
		"X11",
		"dl",
		"GLFW"
	}
	ExternalDependencies["OpenGL"] = {"GL"}
elseif os.host() == "windows" then
	ExternalDependencies["GLFW"] = {"GLFW"}
	ExternalDependencies["OpenGL"] = {"opengl32"}
end


group "Dependencies"
	include "Crimson/Dependencies/glfw"
	include "Crimson/Dependencies/glad"
	include "Crimson/Dependencies/physfs"
	include "Crimson/Dependencies/imgui"
	include "Crimson/Dependencies/lua"
	include "Crimson/Dependencies/reactphysics3d"
	include "Crimson/Dependencies/angelscript"
	include "Crimson/Dependencies/minizip"
	include "Crimson/Dependencies/yaml-cpp"
group ""

include "Crimson"
include "Game"
include "Editor"
