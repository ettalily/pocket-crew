# Pocket Crew
3D platformer collectathon made in C++ and raylib.

## Building on Linux
Requires ``gcc`` for linux builds and ``mingw-w64-gcc`` for windows builds.

To build for linux, navigate to the root directory of the repository and run:
```
bash build.bash
```
To build for windows, navigate to the root directory of the repository and run:
```
bash buildWin.bash
```

## Building on Windows
Requires a C++ compiler, such as the ``mingw-w64-gcc`` compiler included in [w64devkit](https://github.com/skeeto/w64devkit).

To build for windows, open ``w64devkit.exe``, then navigate to the root directory of this repository and run:
```
gcc -o pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./raylib/include ./raylib/windows/lib/libraylib.a -lgdi32 -lwinmm
```

## Notes
- Raylib 5.5 builds for Linux and Windows for x86_64 platforms are included in the ``./raylib`` directory along with their licenses. The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.