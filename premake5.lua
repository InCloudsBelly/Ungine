workspace "Ungine"
	architecture "x64"
	targetdir "build"
    startproject "UngineEditor"

	configurations 
	{ 
		"Debug", 
        "Release",
        "Dist"
    }  
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ungine/vendor/GLFW/include"
IncludeDir["Glad"] = "Ungine/vendor/Glad/include"
IncludeDir["ImGui"] = "Ungine/vendor/ImGui"
IncludeDir["glm"] = "Ungine/vendor/glm"
IncludeDir["Box2D"] = "Ungine/vendor/Box2D/include"
IncludeDir["entt"] = "Ungine/vendor/entt/include"
IncludeDir["mono"] = "Ungine/vendor/mono/include"
IncludeDir["FastNoise"] = "Ungine/vendor/FastNoise"
IncludeDir["PhysX"] = "Ungine/vendor/PhysX/include"

LibraryDir = {}
LibraryDir["mono"] = "vendor/mono/lib/Debug/mono-2.0-sgen.lib"
LibraryDir["PhysX_LowLevel"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevel.lib"
LibraryDir["PhysX_LowLevelAABB"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevelAABB.lib"
LibraryDir["PhysX_LowLevelDynamics"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevelDynamics.lib"
LibraryDir["PhysX"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysX_static_64.lib"
LibraryDir["PhysXCharacterKinematic"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCharacterKinematic_static_64.lib"
LibraryDir["PhysXCommon"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCommon_static_64.lib"
LibraryDir["PhysXCooking"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCooking_static_64.lib"
LibraryDir["PhysXExtensions"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXExtensions_static_64.lib"
LibraryDir["PhysXFoundation"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXFoundation_static_64.lib"LibraryDir["PhysXPvd"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXPvdSDK_static_64.lib"
LibraryDir["PhysXTask"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXTask.lib"
LibraryDir["PhysXVehicle"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXVehicle_static_64.lib"
LibraryDir["PhysX_SceneQuery"] = "vendor/PhysX/lib/%{cfg.buildcfg}/SceneQuery.lib"
LibraryDir["PhysX_SimulationController"] = "vendor/PhysX/lib/%{cfg.buildcfg}/SimulationController.lib"



group "Dependencies"
include "Ungine/vendor/GLFW"
include "Ungine/vendor/Glad"
include "Ungine/vendor/ImGui"
include "Ungine/vendor/Box2D"

group ""

group "Core"
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
		"%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/FastNoise/**.cpp",
        "%{prj.name}/vendor/yaml-cpp/src/**.cpp",
		"%{prj.name}/vendor/yaml-cpp/src/**.h",
		"%{prj.name}/vendor/yaml-cpp/include/**.h"
    }

    includedirs
	{
		"%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
        "%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
        "%{IncludeDir.FastNoise}",
		"%{IncludeDir.PhysX}",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/stb/include",
        "%{prj.name}/vendor/yaml-cpp/include"
    }
    
    links 
	{ 
        "GLFW",
        "Glad",
        "ImGui",
		"Box2D",
        "opengl32.lib",
		"%{LibraryDir.mono}",
		--"%{LibraryDir.PhysX_LowLevel}",
		--"%{LibraryDir.PhysX_LowLevelAABB}",
		--"%{LibraryDir.PhysX_LowLevelDynamics}",
		"%{LibraryDir.PhysX}",
		"%{LibraryDir.PhysXCharacterKinematic}",
		"%{LibraryDir.PhysXCommon}",
		"%{LibraryDir.PhysXCooking}",
		"%{LibraryDir.PhysXExtensions}",
		"%{LibraryDir.PhysXFoundation}",
		"%{LibraryDir.PhysXPvd}",
		--"%{LibraryDir.PhysXTask}",
		--"%{LibraryDir.PhysXVehicle}",
		--"%{LibraryDir.PhysX_SceneQuery}",
		--"%{LibraryDir.PhysX_SimulationController}"

    }

    filter "files:Ungine/vendor/FastNoise/**.cpp or files:Ungine/vendor/yaml-cpp/src/**.cpp"
   	flags { "NoPCH" }
    
	filter "system:windows"
        systemversion "latest"
        
		defines 
		{ 
            "U_PLATFORM_WINDOWS",
            "U_BUILD_DLL",
			"PX_PHYSX_STATIC_LIB"
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



project "UCore-Script"
	location "UCore-Script"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}
group ""

group "Tools"
project "UngineEditor"
    location "UngineEditor"
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}"
    }

    postbuildcommands 
	{
		'{COPY} "../UngineEditor/assets" "%{cfg.targetdir}/assets"'
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

        postbuildcommands 
		{
			'{COPY} "../Ungine/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Ungine/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

                
    filter "configurations:Release"
        defines "U_RELEASE"
        optimize "on"
         links
		{
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mtd.lib"
		}
        postbuildcommands 
		{
			'{COPY} "../Ungine/vendor/assimp/bin/Debug/assimp-vc141-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../Ungine/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}


    filter "configurations:Dist"
        defines "U_DIST"
        optimize "on"
        links
		{
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

        postbuildcommands 
		{
			'{COPY} "../Ungine/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Ungine/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'	
		}
group""


workspace "Sandbox"
        architecture "x64"
        targetdir "build"

        configurations
        {
            "Debug", 
		    "Release",
		    "Dist"
        }

project "UCore-Script"
	    location "UCore-Script"
	    kind "SharedLib"
	    language "C#"

	    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	    files 
	    {
		    "%{prj.name}/src/**.cs", 
	    }

group"exapmle"
project "ExampleApp"
        location "ExampleApp"
	    kind "SharedLib"
	    language "C#"

	    targetdir ("UngineEditor/assets/scripts")
	    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	    files 
	    {
		    "%{prj.name}/src/**.cs", 
	    }

	    links
	    {
		    "UCore-Script"
	    }
group ""

--[[project "Sandbox"
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
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

	filter "configurations:Dist"
		defines "U_DIST"
		optimize "on"

		links
		{
			"Ungine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}
--]]
group ""
