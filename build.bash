clear
echo "Pocket Crew"
echo "----------"
echo "Build for:"
echo "[1] Linux"
echo "[2] Windows"
echo "----------"
echo -n "Selected platform: "
read selection

if [ $selection != "1" ] && [ $selection != "2" ]; then
    echo "Invalid Selection"
    exit 1
fi

echo "----------"

echo "Clearing build directory..."
rm -rf build
mkdir -p build

echo "Copying assets to build directory..."
cp -r assets ./build/assets

if [ $selection == "1" ]; then
    echo "Building binary for Linux platform..."
    if g++ -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -I ./lib/raylib/include ./lib/raylib/static/linux/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 1
    fi
else
    echo "Building binary for Windows platform..."
    if x86_64-w64-mingw32-g++ -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./lib/raylib/include ./lib/raylib/static/windows/libraylib.a -lgdi32 -lwinmm; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 3
    fi
fi

echo "Done."
exit 0