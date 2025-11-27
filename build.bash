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
    if gcc -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 2
    fi
else
    echo "Building binary for Windows platform..."
    if x86_64-w64-mingw32-gcc -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -lraylib -lgdi32 -lwinmm -lstdc++; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 2
    fi
fi
exit 0