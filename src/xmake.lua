add_rules("mode.release", "mode.debug")

add_requires("asio 1.24.0", "nlohmann_json", "spdlog 1.11.0")

add_defines("JUICE_STATIC")
add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")

add_links("ws2_32", "Bcrypt")
add_cxflags("-MD")
add_packages("spdlog")

target("ice")
    set_kind("static")
    add_deps("log")
    add_files("ice/*.cpp")
    add_links("juice")
    add_includedirs("../thirdparty/libjuice/include", {public = true})
    add_linkdirs("../thirdparty/libjuice/lib")

target("ws")
    set_kind("static")
    add_deps("log")
    add_files("ws/*.cpp")
    add_packages("asio")
    add_includedirs("../thirdparty/websocketpp/include", {public = true})

target("log")
    set_kind("headeronly")
    add_packages("spdlog")
    add_headerfiles("log/log.h")
    add_includedirs("log", {public = true})

target("pc")
    set_kind("static")
    add_deps("log")
    add_deps("ws", "ice")
    add_files("pc/*.cpp")
    add_packages("asio", "nlohmann_json")
    add_includedirs("ws", "ice")

target("drtc")
    set_kind("shared")
    add_deps("log")
    add_deps("ice", "ws", "pc")
    add_files("rtc/*.cpp")
    add_packages("asio")
    add_includedirs("rtc", "ice", "ws", "pc")
    add_rules("utils.symbols.export_all", {export_classes = true})
    -- set_policy("build.merge_archive", true)
    -- set_targetdir("$(projectdir)/libdrtc/lib")