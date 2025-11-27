# Pocket Crew
3D platformer made in C++ and raylib.

## Configuration
Game and display preferences can be set in `preferences.cfg`, which is created in the game's root directory when the game is first run. It can be reset by deleting the file and restarting the game.

## Controls
### Keyboard:
- **Movement** - WASD or the Arrow Keys
- **Jump** - K or H
- **Dive** - J
- **Pause** - Enter

### Gamepad:
- **Movement** - Left Stick or Directional Pad
- **Jump** - A or B
- **Dive** - X or Y
- **Pause** - Start/Menu

## Building on Linux
Requires `raylib`. Requires `gcc` for linux builds and `mingw-w64-gcc` for windows builds.

To build for linux, navigate to the root directory of the repository and run:
```bash
bash build.bash
```
To build for windows, navigate to the root directory of the repository and run:
```bash
bash buildWin.bash
```

## Building on Windows
Requires a C++ compiler, such as the ``mingw-w64-gcc`` compiler included in [w64devkit](https://github.com/skeeto/w64devkit).

To build for windows, open ``w64devkit.exe``, then navigate to the root directory of this repository and run:
```bash
mkdir -p build
cp -r assets ./build/assets
gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./raylib/include ./raylib/windows/lib/libraylib.a -lgdi32 -lwinmm -lstdc++
```

## Notes
- Raylib 5.5 builds for Linux and Windows for x86_64 platforms are included in the ``./raylib`` directory along with the license. The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.