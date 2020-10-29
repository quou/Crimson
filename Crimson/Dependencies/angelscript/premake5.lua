project "AngelScript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs {
		"angelscript/include/angelscript"
	}

	files {
		"angelscript/**.cpp",
		"angelscript/**.h",
		"add_on/**.cpp",
		"add_on/**.h"
	}
