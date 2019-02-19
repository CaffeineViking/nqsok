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

    configuration { "windows" }
        links({"png", "glfw3", "glew32", "OpenGL32"})
        links({"jsoncpp"})

    configuration { "linux or bsd or solaris" }
        links({"png", "GLFW", "GLEW", "GL"})
        links({"jsoncpp"})

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
