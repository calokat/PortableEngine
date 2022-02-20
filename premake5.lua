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
    includedirs {"PE_Core", "PE_OpenGL", "PE_DX11", "PE_XR", "C:/vcpkg/installed/include/x64-windows"}
    defines {"_WIN64", "_WIN32", "XR_USE_GRAPHICS_API_OPENGL", "XR_USE_GRAPHICS_API_D3D11", "XR_USE_PLATFORM_WIN32"}
    undefines {"UNICODE", "_UNICODE"}
    os.mkdir "build"
    basedir "build"
project "PortableEngine_Win32"
    kind "ConsoleApp"
    language "C++"
    basedir "build"
    targetdir "build/bin/%{cfg.buildcfg}"
    os.mkdir "build/bin/Debug"
    files {"PortableEngine/*.h", "PortableEngine/*.cpp"}
    system "windows"
    architecture "x86_64"
    cppdialect "C++17"
    links {"PE_Core", "PE_DX11", "PE_OpenGL", "PE_XR", "assimp-vc142-mt.lib", "bz2.lib", "DirectXTK.lib", "glew32.lib", "glfw3dll.lib", "GlU32.Lib", "imgui.lib", "Irrlicht.lib", "jpeg.lib", "jsoncpp.lib", "kubazip.lib", "libpng16.lib", "minizip.lib", "OpenGL32.Lib", "openxr_loader.lib", "poly2tri.lib", "polyclipping.lib", "turbojpeg.lib", "zlib.lib"}
    libdirs {"C:/vcpkg/installed/x64-windows/lib"}
    stringpooling "on"
    local dllDir = "C:/vcpkg/installed/x64-windows/bin/"
    local dlls = os.matchfiles(dllDir .. "*.dll")
    for i,d in pairs(dlls) do
        os.copyfile(d, "build/bin/" .. "Debug/" .. path.getname(d))
    end
    copydirs("build/Assets", "Assets/**")
    copydirs("build/Shaders", "Shaders/**")
project "PE_Core"
    kind "SharedItems"
    language "C++"
    basedir "build"
    files {"PE_Core/*.h", "PE_Core/*.cpp"}
project "PE_OpenGL"
    kind "SharedItems"
    language "C++"
    basedir "build"
    files {"PE_OpenGL/*.h", "PE_OpenGL/*.cpp"}
project "PE_DX11"
    kind "SharedItems"
    language "C++"
    basedir "build"
    files {"PE_DX11/*.h", "PE_DX11/*.cpp"}
project "PE_XR"
    kind "SharedItems"
    language "C++"
    basedir "build"
    files {"PE_XR/**.h", "PE_XR/**.cpp"}