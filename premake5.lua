workspace "VolcanicPong"
	architecture "x64"
	configurations { "Debug", "Release", "Dist" }
	startproject "VolcanicPong"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

project "VolcanicPong"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "src/*.h", "src/*.cpp" }
	
	includedirs
	{
		"vendor/glfw/include",
		"vendor/glm",

		"src",

		"%{VULKAN_SDK}/Include",
	}

	libdirs
	{
		"vendor/glfw/lib-vc2022"
	}

	links
	{
		"glfw3",
		"%{VULKAN_SDK}/Lib/vulkan-1.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"
		
	filter "configurations:Debug"
		defines "CONFIGURATION_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "CONFIGURATION_RELEASE"
		runtime "Debug"
		symbols "off"
		optimize "on"
			
	filter "configurations:Dist"
		defines "CONFIGURATION_DIST"
		runtime "Release"
		symbols "off"
		optimize "on"