# Building from Source
Not comprehensive, but should get it building on most x86_64 systems.

## Building on Linux

### Dependencies

Requires the `gcc` compiler for linux builds, and the `mingw-w64-gcc` compiler for windows builds.

Raylib also has some required dependencies: `mesa`, `x11`, and `alsa`.

#### Arch-based Distributions
```bash
sudo pacman -S gcc mingw-w64-gcc alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

#### Debian-based Distributions
```bash
sudo apt install build-essential gcc-mingw-w64 libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

### Build Script (Recommended)

To build linux and windows binaries with static raylib libraries, run the build script in the root directory of this repository and select the desired target platform.

```bash
bash build.bash
```

### Building with Shared Raylib Libraries

To use system-installed shared raylib libraries, navigate to the root of the repository and copy the asset files:

```bash
mkdir -p build
cp -r assets ./build/assets
```
then build the game. To build for linux, run:
```bash
g++ -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

To build for windows, run:
```bash
x86_64-w64-mingw32-g++ -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -lraylib -lgdi32 -lwinmm
```

## Building on Windows

### Using a Raylib System Installation (Recommended)
Install `raylib` and the `mingw-w64-gcc` compiler through the [raylib installer](https://raysan5.itch.io/raylib).

To build for windows, run `C:\raylib\w64devkit.exe`, then navigate to the root directory of this repository and run:

```bash
mkdir -p build
cp -r assets ./build/assets
gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -lraylib -lgdi32 -lwinmm -lstdc++
```

### Using Non-Installed Libraries
Alternatively, you can manually download a compiler and a raylib release, and then link the libraries in the build command.

Requires a C++ compiler, such as the `mingw-w64-gcc` compiler included in [w64devkit](https://github.com/skeeto/w64devkit).

To build for windows, run `w64devkit.exe`, navigate to the root directory of this repository, then run:

```bash
mkdir -p build
cp -r assets ./build/assets
g++ -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./lib/raylib/include ./lib/raylib/static/windows/libraylib.a -lgdi32 -lwinmm
```