project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs {
		"../Crimson/Source",
		"../Crimson/%{IncludeDir.GLFW}"
	}

	links {
		"Crimson"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
