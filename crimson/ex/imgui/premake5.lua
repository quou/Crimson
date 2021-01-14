project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "On"

	includedirs {
		"include"
	}

	files {
		"include/**.h",
		"src/**.cpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
