add_rules("mode.release", "mode.debug")

target("ice")
    set_kind("static")
    add_files("ice/*.cpp")
    add_links("juice", "ws2_32", "Bcrypt")
    -- add_cxflags("-MD")
    add_includedirs("../thirdparty/libjuice/include")
    add_linkdirs("../thirdparty/libjuice/lib")

target("ws")
    set_kind("static")
    add_files("ws/*.cpp")
    add_packages("asio")
    add_includedirs("../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")

target("pc")
    set_kind("static")
    add_deps("ws", "ice")
    add_files("pc/*.cpp")
    add_packages("asio")
    add_includedirs("ice", "ws")
    add_includedirs("../thirdparty/libjuice/include")
    add_includedirs("../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")

target("drtc")
    set_kind("shared")
    add_deps("ice", "ws", "pc")
    add_files("rtc/*.cpp")
    add_packages("asio")
    add_includedirs("rtc", "ice", "ws", "pc")
    add_includedirs("../thirdparty/libjuice/include")
    add_includedirs("../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")
    add_rules("utils.symbols.export_all", {export_classes = true})
    -- set_policy("build.merge_archive", true)
    -- set_targetdir("$(projectdir)/libdrtc/lib")