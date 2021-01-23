-- Main premake file

workspace "crimson"
	configurations { "debug", "release" }
	linkgroups "on"

include "crimson"
include "sandbox"
include "editor"