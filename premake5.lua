function copydirs(dirname, reldirpath)
    os.mkdir(dirname)
    local dirpaths = os.matchdirs(reldirpath)
    for i, d in pairs(dirpaths) do
        local rebasedpath = path.rebase(d, "C:/", ".")
        os.mkdir("build/" .. d)
    end
    local files = os.matchfiles(reldirpath)
    for i, f in pairs(files) do
        local rebasedpath = path.rebase(f, "C:/", ".")
        os.copyfile(f, "build/" .. f)
    end
end

workspace "PortableEngine"
    configurations {"Debug", "Release"}
    undefines {"UNICODE", "_UNICODE"}
    os.mkdir "build"
    basedir "build"
    includedirs {"PE_Core"}
    filter "system:windows"
        includedirs "C:/vcpkg/installed/include/x64-windows"
    filter "system:linux"
        includedirs "../vcpkg/installed/x64-linux/include"
    project "PE_Core"
        filter "system:windows"
            kind "SharedItems"
        filter "system:linux"
            kind "SharedLib"
        language "C++"
        basedir "build"
        files {"PE_Core/*.h", "PE_Core/*.cpp"}
        includedirs {"PE_OpenGL"}
    project "PE_OpenGL"
        filter "system:windows"
            kind "SharedItems"
        filter "system:linux"
            kind "SharedLib"
        language "C++"
        basedir "build"
        files {"PE_OpenGL/*.h", "PE_OpenGL/*.cpp"}
    project "PortableEngine"
        kind "ConsoleApp"
        language "C++"
        includedirs {"PE_Core", "PE_OpenGL"}
        basedir "build"
        targetdir "build/bin/%{cfg.buildcfg}"
        os.mkdir "build/bin/Debug"
        architecture "x86_64"
        cppdialect "C++17"
        links {"PE_Core", "PE_OpenGL"}
        stringpooling "on"
        copydirs("build/Assets", "Assets/**")
        copydirs("build/Shaders", "Shaders/**")
        filter "system:windows"
            files {"PortableEngine/*.h", "PortableEngine/*.cpp"}
            defines {"_WIN64", "_WIN32", "XR_USE_GRAPHICS_API_OPENGL", "XR_USE_GRAPHICS_API_D3D11", "XR_USE_PLATFORM_WIN32"}
            includedirs {"PE_DX11", "PE_XR"}
            libdirs {"C:/vcpkg/installed/x64-windows/lib"}
            links {"PE_Win32", "PE_DX11", "PE_XR", "assimp-vc142-mt.lib", "bz2.lib", "DirectXTK.lib", "glew32.lib", "glfw3dll.lib", "GlU32.Lib", "imgui.lib", "Irrlicht.lib", "jpeg.lib", "jsoncpp.lib", "kubazip.lib", "libpng16.lib", "minizip.lib", "OpenGL32.Lib", "openxr_loader.lib", "poly2tri.lib", "polyclipping.lib", "turbojpeg.lib", "zlib.lib"}
            local dllDir = "C:/vcpkg/installed/x64-windows/bin/"
            local dlls = os.matchfiles(dllDir .. "*.dll")
            for i,d in pairs(dlls) do
                os.copyfile(d, "build/bin/" .. "Debug/" .. path.getname(d))
            end
        filter "system:linux"
            includedirs {"PortableEngine_Linux"}
            libdirs "/home/calokat/git/vcpkg/installed/x64-linux/lib"
            links {"PE_Linux", "assimp", "imgui", "SDL2", "SDL2main", "GL", "GLEW", "GLU"}
            buildoptions {"-w"}
    if (_TARGET_OS == "windows")
    then
        project "PE_DX11"
            basedir "build"
            kind "SharedItems"
            language "C++"
            files {"PE_DX11/*.h", "PE_DX11/*.cpp"}
        project "PE_XR"
            basedir "build"
            kind "SharedItems"
            language "C++"
            files {"PE_XR/**.h", "PE_XR/**.cpp"}
        project "PE_Win32"
            basedir "build"
            kind "SharedItems"
            language "C++"
            files {"PE_Win32/**.h", "PE_Win32/**.cpp"}
    end
    if (_TARGET_OS == "linux")
    then
        project "PE_Linux"
        basedir "build"    
        kind "SharedLib"
        language "C++"
        files {"PE_Linux/*.h", "PE_Linux/*.cpp"}
    end