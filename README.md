# Pocket Crew
3D platformer made in C++ and raylib.

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

## Download
Linux and Windows builds of the game can be found on the [GitHub Releases page](https://github.com/ettalily/pocket-crew/releases).

Extract the archive and run `pocket` or `pocket.exe` to start the game.

## Configuration
Game and display preferences can be set in `preferences.cfg`, which is created in the game's root directory when the game is first run. It can be reset by deleting the file and restarting the game.

## Building on Linux

### Build Script (Recommended)
Requires `gcc`. Additionally requires `mingw-w64-gcc` for windows builds.

To build, run `build.bash` in the root of the directory and select the desired target platform.

```bash
bash build.bash
```

## Building on Windows

### Using a Raylib System Installation (Recommended)
Requires [raylib](https://www.raylib.com/).

To build for windows, run `C:\raylib\w64devkit.exe`, then navigate to the root directory of this repository and run:

```bash
mkdir -p build
cp -r assets ./build/assets
gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -lraylib -lgdi32 -lwinmm -lstdc++
```

### Using Non-System Libraries
Alternatively, you can manually download a compiler and a raylib release, and then link the libraries in the build command.

Requires a C++ compiler, such as the `mingw-w64-gcc` compiler included in [w64devkit](https://github.com/skeeto/w64devkit), and a windows [raylib release](https://github.com/raysan5/raylib/releases).

To build for windows, run `w64devkit.exe`, navigate to the root directory of this repository, substitute `/path/to/raylib` for the location of your raylib release, then run:

```bash
mkdir -p build
cp -r assets ./build/assets
g++ -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I /path/to/raylib/include /path/to/raylib/lib/libraylib.a -lgdi32 -lwinmm
```

## Notes
- The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.