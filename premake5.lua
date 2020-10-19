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

	linkgroups "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Relative to Crimson
IncludeDir = {}
IncludeDir["GLFW"] = "Dependencies/glfw/include"

group "Dependencies"
	include "Crimson/Dependencies/glfw"
group ""

include "Crimson"
include "Game"
