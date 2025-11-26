mkdir -p build
cp -r assets ./build/assets
x86_64-w64-mingw32-gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./raylib/include ./raylib/windows/lib/libraylib.a -lgdi32 -lwinmm -lstdc++
