add_requires("asio 1.24.0")

target("ws")
    set_kind("static")
    add_files("ws_endpoint.cpp", "ws_client.cpp")
    add_packages("asio")
    add_includedirs("../../thirdparty/websocketpp/include")
    add_defines("ASIO_STANDALONE","_WEBSOCKETPP_CPP11_INTERNAL_", "ASIO_HAS_STD_TYPE_TRAITS", "ASIO_HAS_STD_SHARED_PTR", 
    "ASIO_HAS_STD_ADDRESSOF", "ASIO_HAS_STD_ATOMIC", "ASIO_HAS_STD_CHRONO", "ASIO_HAS_CSTDINT", "ASIO_HAS_STD_ARRAY",
    "ASIO_HAS_STD_SYSTEM_ERROR")
