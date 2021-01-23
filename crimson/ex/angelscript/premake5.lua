project "angelscript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"

	includedirs {
		"angelscript/include/angelscript"
	}

	files {
		"angelscript/source/**.cpp",
		"angelscript/source/**.h",
		"angelscript/add_on/**.cpp",
		"angelscript/add_on/**.h"
	}

	if (os.host() == "windows") then
		filter "configurations:Debug"
			defines { "DEBUG", "WIN32" }
			symbols "On"
			files {
				"angelscript/source/as_callfunc_x64_msvc.cpp",
				"angelscript/source/as_callfunc_x64_msvc_asm.asm"
			}

		filter "configurations:Release"
			defines { "RELEASE", "WIN32" }
			optimize "On"
			files {
				"angelscript/source/as_callfunc_x64_msvc.cpp",
				"angelscript/source/as_callfunc_x64_msvc_asm.asm"
			}
	else
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			runtime "Release"
			optimize "on"
	end
