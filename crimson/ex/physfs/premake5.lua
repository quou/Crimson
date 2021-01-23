project "physfs"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	
	files {
		"src/**.c",
		"src/**.h"
	}
	
	filter "system:linux"
		pic "on"

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "on"

	filter "configurations:release"
		defines { "RELEASE" }
		optimize "on"
