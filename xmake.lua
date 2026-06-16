add_rules("mode.debug", "mode.release")
set_languages("c++17")

set_policy("build.accelerate", true)


-- nlohmann/json
add_requires("nlohmann_json", {configs = {shared = false}})
add_packages("nlohmann_json")
    
-- libcurl
add_requires("libcurl", {configs = {shared = false, ssl = true}})
add_packages("libcurl")
    
-- webview
if is_plat("windows") then
    -- Windows
    add_requires("webview", {configs = {backend = "webview2"}})
    add_packages("webview")

    add_syslinks("user32", "shell32", "ole32", "shlwapi")
        
elseif is_plat("macosx") then
    -- macOS: use WKWebView 
    add_requires("webview", {configs = {backend = "cocoa"}})
    add_packages("webview")
    
    add_frameworks("Cocoa", "WebKit", "Security")
    
elseif is_plat("linux") then
    -- Linux: use WebKitGTK
    add_requires("webview", {configs = {backend = "gtk"}})
    add_packages("webview")
        
    add_syslinks("webkit2gtk-4.0", "gtk-3", "glib-2.0")
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
    add_files("src/*.cpp")
    add_tests("test_with_stub", {
        -- files = "tests/stub_*.cpp"
        defines = "TEST_MODE",
        remove_files = "src/main.cpp"
    })