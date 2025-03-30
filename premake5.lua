workspace "dogocraft"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    raylibinclude = "vendor/raylib/include"
    rayliblib = "vendor/raylib/lib"
    raygui = "vendor/raygui"
    fastnoiselite = "vendor/fastnoiselite"

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "dogocraft"
    kind "ConsoleApp"
    language "C"
    staticruntime "on"

    targetdir ("bin/".. outputDir .. "/%{prj.name}")
    objdir ("bin-int/".. outputDir .. "/%{prj.name}")

    files
    {
        "include/**.h",
        "src/**.c"
    }

    includedirs
    {
        "include",
        raylibinclude,
        raygui,
        fastnoiselite
    }
    libdirs
    {
        rayliblib
    }
    links
    {
        "raylib",
        "gdi32", 
        "winmm", 
        "opengl32"
    }
    buildoptions { "-Wall", "-Wextra", "-Wpedantic", "-std=c11" }


    filter "system:windows"
        system "windows"
        systemversion = "latest"

    filter "system:linux"
        system "linux"
        systemversion = "latest"

       buildoptions
       {
           "-v"
       }
    

    filter "configurations:Debug"
        defines "DG_BUILD_DEBUG"
        symbols "on"
        optimize "Off"
        runtime "Debug"


    filter "configurations:Release"
        defines "DG_BUILD_RELEASE"
        optimize "on"
        optimize "Speed"
        omitframepointer "On"
        flags { "LinkTimeOptimization" }
        runtime "Release"
