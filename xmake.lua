add_rules("mode.debug", "mode.release")
set_languages("c++17")

-- Enable automatic installation and checking of required packages
set_policy("package.requires_auto_install", true)
set_policy("package.requires_auto_check", true)

-- webview-xmake compiler toolchain
add_repositories("webview-xmake https://github.com/Winterreisender/webview-xmake.git")
add_requires("webview 0.10.0.230210")
if is_plat("linux") then
    add_requires("pkgconfig::gtk+-3.0", "pkgconfig::webkit2gtk-4.1", {system = true})
end

-- nlohmann/json
add_requires("nlohmann_json", {configs = {shared = false}})
add_packages("nlohmann_json")
    
-- libcurl
add_requires("libcurl", {configs = {shared = false, ssl = true}})
add_packages("libcurl")
    
-- webview
if is_plat("linux") then
    add_packages("pkgconfig::gtk+-3.0")
    add_packages("pkgconfig::libwebkit2gtk-4.1-dev")
end
if is_plat("macosx") then
    set_languages("c++11")
    add_frameworks("WebKit")
end

if is_plat("windows") then
    add_defines("ETL_PLATFORM_WINDOWS")
elseif is_plat("macosx") then
    add_defines("ETL_PLATFORM_MACOS")
elseif is_plat("linux") then
    add_defines("ETL_PLATFORM_LINUX")
end

after_build(function(target)
    local target_dir = target:targetdir()
    if os.exists("resources") then
        os.cp("resources", path.join(target_dir, "resources"))
    end
    if os.exists("assets") then
        os.cp("assets", path.join(target_dir, "assets"))
    end
end)

target("EtherealLauncher")
    set_kind("binary")

    add_files("src/**.cpp")
    add_includedirs("src")

target("test")

    set_kind("binary")
    add_files("src/**.cpp")
    add_includedirs("src")

    add_tests("test_with_stub", {
        files = "tests/stub_*.cpp",  -- 添加额外的测试文件
        defines = "TEST_MODE",
        remove_files = "src/main.cpp"  -- 移除不需要的文件
    })