NQ Sokoban
==========

Rather than being a straight up 3-D Sokoban clone (there are already quite a lot of those around), this project: *Not Quite Sokoban*, tries to innovate somewhat by adding a few novel gameplay twists and mechanics.

Besides the original rules of Sokoban, additional *layers* (an actual extra dimension) is added to the world. These create interesting new rules, especially once we start disallowing *blocks* to be pushed up a layer but still allowing them to fall down several layers. The player needs to be careful when pushing blocks around! An additional mechanic is to disallow traversal through a path that has an abyss (meaning there are no blocks to fall down), *but* if a *goal area* is there, the player is allowed to build a *"bridge"* by pushing a movable block (gaining access to new areas). It's a mix between the original Sokoban rules and the game EDGE (originally released on the iOS).

The above changes to the gameplay mechanics do not affect normal Sokoban levels, so *NQ Sokoban* is essentially "backwards compatible" with the original Sokoban levels given that these only use compatible mechanics (2 layers). Additional mechanics could be added later, but additional complexity would make the game inelegant and difficult to learn. Right now only a couple of levels have been implemented, but I'm planning on getting back to this once I have more time.

There is a short report on the game, and also a presentation on the architecture and the easing equations used for the animations:

* [Project Report for Not Quite Sokoban](https://eriksvjansson.net/papers/rnqsok.pdf)
* [Not Quite Sokoban](https://eriksvjansson.net/papers/pnqsok.pdf) (the presentation)

This project was made for the course [TSBK07 (Computer Graphics) at Linköping University](http://computer-graphics.se/TSBK07/) led by Ingemar Ragnemalm.

Requirements
------------

* Support for OpenGL 2.1 or higher.
* Dynamically linked libraries (might be provided).
* The resource files (might be bundled under share/).
* Only tested on a GNU + Linux system, Arch Linux.
* If building, a good C++11/14 implementation.

Building
--------

The build system used here is the old Premake 4. You can get it over [here](http://premake.github.io/download.html).

This project has external dependencies on *libPNG*, *GLFW* and *GLEW*. You can install these via your favourite package manager or just manually. Additionally, the *TinyObjLoader* library is used, however, that is already included in this repository and should therefore already work fine.

Currently, the only development system this has been tested on is by using the tools GNU Make and GCC 5. However, since the project is using Premake 4 everything should work more or less smoothly on any other development systems (with some minor changes I assume). I've tested this on Arch Linux and on the MINGW64 MSYS2 Windows toolchain, and it should at least work OK on those (again, after installing dependencies).

1. Clone this repository: `git clone https://github.com/CaffeineViking/nqsok.git`
2. Generate the build files with Premake (within project directory): `premake4 gmake`
3. Make sure you have the project dependencies reachable in your path (e.g. `$PATH`).
4. Compile the generated build files (default: GNU Make and GCC): `make -j4 -C build`
5. Run the program: `bin/nqsok level core/hello.nql` with the "Hello World" level.
6. Always bundle the program with the `share/` folder, as they have the assets.

Libraries
---------

You'll have to install these libraries yourself (e.g. `pacman` or `apt-get`):

* libPNG: [libPNG License](http://www.libpng.org/pub/png/src/libpng-LICENSE.txt)
* GLFW: [zlib/libPNG License](http://www.glfw.org/license.html)
* GLEW: [Modified BSD License](https://github.com/nigels-com/glew/blob/master/LICENSE.txt)

The libraries below are provided for you (as they are header-only anyway):

* TinyObjLoader: [BSDv2 License](https://github.com/syoyo/tinyobjloader)

NQL Format
----------

For the levels and packs, a few file types have been introduced. They are all specified in an easy-to-parse JSON format that uses layers of PNG files to specify the files themselves (i.e. a poor man's level editor by using GIMP or Paint.NET). It was so many years since I wrote that, so I'm not 100% sure anymore how it works, but you can take a look at `share/` for how to build new levels. If you want a JSON schema for it, forget it :-). Maybe in a remake of it, but I was young and stupid back then, so we'll just have to live with the JSON / PNG file format combination. I also planned to write a original Sokoban level parser for it, so the entire classic Sokoban could be played, but I just didn't get around to doing it. If anyone wants to implement that (or have general improvments to the project), feel free to open a PR.
