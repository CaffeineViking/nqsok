NQ Sokoban
==========

Rather than being a straight up 3D Sokoban clone (there are already a lot of those), the game project *Not Quite Sokoban* tries to innovate somewhat by adding a few extra gameplay twists/mechanics.

Besides the original rules of Sokoban, additional *layers* (an extra dimension that can be traversed) are added to the world/level. These create interesting implications, especially if disallowing *blocks* to be pushed up a layer but still allowing them to fall down several of these. An additional mechanic that can be envisioned is to disallow traversal through a path that has an abyss (meaning that there are no blocks to fall down to) *but* if a *goal area* is there, the player is allowed to build a *"bridge"* by pushing a movable block there (gaining access to a new area).

The above changes to the gameplay mechanics do not affect normal Sokoban levels, so *NQ Sokoban* is essentially "backwards compatible" with the original Sokoban levels given that these only use compatible mechanics (2 layers). Additional mechanics could be added later, but additional complexity would make the game inelegant.

Requirements
------------

* Support for OpenGL 2.1 or better.
* Dynamically linked libraries (might be provided).
* The resource files (might be bundled under share/).
* Only tested on a GNU + Linux system, Arch Linux.
* If building, a good C++11/14 implementation.

Building
--------

The build system used here is the quite amazing Premake 4. You can get it over [here](http://premake.github.io/download.html).

This project has external dependencies on *libPNG*, *GLFW*, *GLEW*, *OGG Vorbis*, *OpenAL Soft* and *FreeType*. You can install these via your favourite package manager or just manually. Additionally, the *TinyObjLoader* library is used, however, that is already included in this repository and should therefore already work fine.

Currently, the only development system this has been tested on is by using the tools GNU Make and GCC 5. However, since the project is using Premake 4 everything should work more or less smoothly on other development systems.

1. Clone this repository: `git clone https://github.com/CaffeineViking/nqsok.git`
2. Generate the build files with Premake (within project directory): `premake4 gmake`
3. Make sure you have the project dependencies and that they are reachable via the path.
4. Compile the generated build files (default: GNU Make and GCC): `make -j4 -C build`
5. Run the program (hopefully it works...): `bin/nqsok`
6. Always bundle the program with the `share/` folder.

Libraries
---------

*"If I have seen further, it is by standing on the shoulders of giants."*

* libPNG: [libPNG License](http://www.libpng.org/pub/png/src/libpng-LICENSE.txt)
* GLFW: [zlib/libPNG License](http://www.glfw.org/license.html)
* GLEW: [Modified BSD License](https://github.com/nigels-com/glew/blob/master/LICENSE.txt)
* OGG Vorbis: [BSDv2 License](https://xiph.org/vorbis/)
* OpenAL Soft: [LGPL2 License](https://github.com/kcat/openal-soft/blob/master/COPYING)
* FreeType: [GPLv2 License](http://www.freetype.org/license.html)
* TinyObjLoader: [BSDv2 License](https://github.com/syoyo/tinyobjloader)
