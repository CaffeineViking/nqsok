#!lua

local name = "nqsok"
solution(name)
    location("build/")
    targetdir("bin/")
    language("C++")

    flags("ExtraWarnings")
    configurations({"debug", "release"})
    configuration("debug") flags("Symbols")
    configuration("release") flags("Optimize")
    configuration("gmake") buildoptions("-std=c++14 -Wextra -Wpedantic")
    links({"png", "glfw", "GLEW", "GL"})
    links({"vorbis", "ogg", "openal"})
    links({"freetype", "jsoncpp"})

    project(name)
        kind("WindowedApp")
        files({"src/**.cc", "src/**.hh",
               "src/**.cpp", "src/**.hpp", "src/**.h"})
        excludes({"src/tests/**.cc", "src/tests/**.hh",
                  "src/tests/**.cpp", "src/tests/**.hpp", "src/tests/**.h"})

    project(name .. "_tests")
        kind("ConsoleApp")
        files({"src/**.cc", "src/**.hh",
               "src/**.cpp", "src/**.hpp", "src/**.h"})
        excludes("src/main.cc", "src/main.cpp")
