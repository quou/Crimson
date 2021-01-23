-- Main premake file

workspace "crimson"
	configurations { "debug", "release" }
	linkgroups "on"

	toolset "GCC"

include "crimson"
include "sandbox"
include "editor"