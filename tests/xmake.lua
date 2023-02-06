add_requires("asio 1.24.0", "nlohmann_json", "spdlog 1.11.0")

add_defines("JUICE_STATIC")
add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")
add_defines("SIGNAL_LOGGER")

target("signal_server")
    set_kind("binary")
    add_deps("log")
    add_files("signal_server/*.cpp")
    add_packages("asio", "nlohmann_json", "spdlog")
    add_includedirs("../thirdparty/websocketpp/include")

-- target("signal_client")
--     set_kind("binary")
--     add_deps("ws")
--     add_files("signal_client/signal_client.cpp")
--     add_packages("asio")
--     add_includedirs("../src/ws", "../thirdparty/websocketpp/include")

-- target("ice_test")
--     set_kind("binary")
--     add_deps("ice")
--     add_files("ice_test.cpp")
--     add_links("juice", "ws2_32", "Bcrypt")
--     add_cxflags("-MD")
--     add_includedirs("../thirdparty/libjuice/include", "../src/ice")
--     add_linkdirs("../thirdparty/libjuice/lib")

target("peerconnection")
    set_kind("binary")
    add_deps("drtc")
    add_files("peerconnection/*.cpp")
    add_includedirs("../src/rtc")