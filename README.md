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
Requires `gcc` and `raylib` for linux builds, and `mingw-w64-gcc` and `mingw-w64-raylib` for windows builds.

To build, run `build.bash` in the root of the directory and select the desired target platform.

```bash
bash build.bash
```

### Using Non-System Libraries
Alternatively, you can manually download a raylib release and link the libraries in the build command.

#### Linux Builds
Requires `gcc` and a linux [raylib release](https://github.com/raysan5/raylib/releases).

To build for linux, substitute `/path/to/raylib` for the location of your raylib release, then run:

```bash
mkdir -p build
cp -p assets ./build/assets
gcc -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -I /path/to/raylib/include /path/to/raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
```

#### Windows Builds
Requires `mingw-w64-gcc` and a windows [raylib release](https://github.com/raysan5/raylib/releases).

To build for windows, substitute `/path/to/raylib` for the location of your raylib release, then run:

```bash
mkdir -p build
cp -p assets ./build/assets
x86_64-w64-mingw32-gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I /path/to/raylib/include /path/to/raylib/lib/libraylib.a -lgdi32 -lwinmm -lstdc++
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
gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I /path/to/raylib/include /path/to/raylib/lib/libraylib.a -lgdi32 -lwinmm -lstdc++
```

## Notes
- The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.