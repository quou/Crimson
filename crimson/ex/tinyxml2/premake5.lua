project "tinyxml2"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	pic "on"

	files {
		"src/tinyxml2.h",
		"src/tinyxml2.cpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
