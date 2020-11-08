project "Lua"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	files {
		"src/**.h",
		"src/**.c"
	}

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
