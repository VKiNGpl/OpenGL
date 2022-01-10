workspace "OpenGL"
	architecture "x64"
	startproject "OpenGL"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL/vendor/GLFW/include"
IncludeDir["GLEW"] = "OpenGL/vendor/GLEW/include"
IncludeDir["glm"]  = "OpenGL/vendor/glm"

group "Dependencies"
	include "OpenGL/vendor/GLFW"
group ""

project "OpenGL"
	location "OpenGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

--	pchheader "gepch.h"
--	pchsource "OpenGL/src/gepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",
		"%{prj.name}/res/**.shader",
		"%{prj.name}/res/**.PNG"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLEW_STATIC"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"glew32s.lib",
		"opengl32.lib"
	}

	libdirs
	{
		"%{prj.name}/vendor/GLEW/lib/Release/x64"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLEW_STATIC"
		}

		linkoptions
		{ 
			"/NODEFAULTLIB:LIBCMT",
			"/IGNORE:4099"
		}

	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_DIST"
		runtime "Release"
		optimize "on"
