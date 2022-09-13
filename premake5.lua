workspace "Ungine"
	architecture "x64"
	targetdir "build"
	
	configurations 
	{ 
		"Debug", 
        "Release",
        "Dist"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ungine/vendor/GLFW/include"
IncludeDir["Glad"] = "Ungine/vendor/Glad/include"
IncludeDir["ImGui"] = "Ungine/vendor/ImGui"
IncludeDir["glm"] = "Ungine/vendor/glm"

include "Ungine/vendor/GLFW"
include "Ungine/vendor/Glad"
include "Ungine/vendor/ImGui"

project "Ungine"
    location "Ungine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Upch.h"
    pchsource "Ungine/src/Upch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
    }

    includedirs
	{
		"%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/stb/include"
    }
    
    links 
	{ 
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
	filter "system:windows"
        systemversion "latest"
        
		defines 
		{ 
            "U_PLATFORM_WINDOWS",
            "U_BUILD_DLL"
		}

    filter "configurations:Debug"
        defines "U_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
        defines "U_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "U_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
        "Ungine"
    }
    
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
    
	includedirs 
	{
        "%{prj.name}/src",
        "Ungine/src",
        "Ungine/vendor",
        "%{IncludeDir.glm}"
    }
	
	filter "system:windows"
        systemversion "latest"
                
		defines 
		{ 
            "U_PLATFORM_WINDOWS"
		}
    
    filter "configurations:Debug"
        defines "U_DEBUG"
        symbols "on"
        links
		{
			"Ungine/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}

                
    filter "configurations:Release"
        defines "U_RELEASE"
        optimize "on"
         links
		{
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mtd.lib"
		}

    filter "configurations:Dist"
        defines "U_DIST"
        optimize "on"
        links
		{
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

