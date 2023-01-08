add_rules("mode.release", "mode.debug")

target("ice")
    set_kind("static")
    add_files("*.cpp")
    add_links("juice-static", "/NODEFAULTLIB:MSVCRTD")
    add_includedirs("../../thirdparty/libjuice/include")
    add_linkdirs("../../thirdparty/libjuice/lib")
    add_defines("FORCE_SHARED_CRT=OFF")