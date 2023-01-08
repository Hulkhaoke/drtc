add_defines("TEST")
-- target("test")
--     set_kind("binary")
--     add_files("*.cpp")
--     add_includedirs("../src/ice")


add_requires("asio 1.24.0")

target("answer")
    set_kind("binary")
    add_files("answer.cpp")
    add_links("juice")
    add_includedirs("../thirdparty/libjuice/include")
    add_linkdirs("../thirdparty/libjuice/libshared")

target("offer")
    set_kind("binary")
    add_files("offer.cpp")
    add_links("juice")
    add_includedirs("../thirdparty/libjuice/include")
    add_linkdirs("../thirdparty/libjuice/libshared")

target("server")
    set_kind("binary")
    add_files("server.cpp")
    add_packages("asio")
    add_includedirs("../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")

target("client")
    set_kind("binary")
    add_files("client.cpp")
    add_packages("asio")
    add_includedirs("../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")
