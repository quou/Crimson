project "AngelScript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "On"

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

	if (os.host() == "windows") then
		filter "configurations:Debug"
			defines { "DEBUG", "WIN32" }
			symbols "On"
			files {
				"angelscript/source/as_callfunc_x64_msvc.cpp",
				"angelscript/source/as_callfunc_x64_msvc_asm.asm"
			}

		filter "configurations:Release"
			defines { "NDEBUG", "WIN32" }
			optimize "On"
			files {
				"angelscript/source/as_callfunc_x64_msvc.cpp",
				"angelscript/source/as_callfunc_x64_msvc_asm.asm"
			}
	end
