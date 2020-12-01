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
		"../Crimson/Dependencies/imgui/include",
		"../Crimson/Dependencies/glm",
		"../Crimson/Dependencies/entt/include",
		"../Crimson/Dependencies/reactphysics3d/include",
		"../Crimson/Dependencies/minizip"
	}

	links {
		ExternalDependencies.GLFW,
		ExternalDependencies.Glad,
		ExternalDependencies.OpenGL,
		ExternalDependencies.PhysFS,
		ExternalDependencies.ImGui,
		ExternalDependencies.Lua,
		ExternalDependencies.ReactPhysics3D,
		ExternalDependencies.AngelScript,
		ExternalDependencies.Minizip,
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
