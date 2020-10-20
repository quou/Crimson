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

ExternalDependencies = {}
ExternalDependencies["GLFW"] = {}
ExternalDependencies["OpenGL"] = {}
ExternalDependencies["ImGui"] = {"ImGui"}
ExternalDependencies["Glad"] = {"Glad"}
ExternalDependencies["PhysFS"] = {"PhysFS"}

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
group ""

include "Crimson"
include "Game"
