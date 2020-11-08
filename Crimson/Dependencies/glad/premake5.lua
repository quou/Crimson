project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs {
		"include"
	}

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}
