set_project("drtc")
set_version("1.0.1")

add_rules("mode.release", "mode.debug")
set_languages("c++17")

includes("**/xmake.lua")