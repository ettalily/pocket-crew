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

echo "Downloading raylib library..."
mkdir -p ./build/tmp
cd ./build/tmp
if [ $selection == "1" ]; then
    if ! wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz; then
        echo "Failed to download library."
        exit 1
    fi
    
    echo "Extracting raylib archive..."
    if ! tar -xzvf raylib-5.5_linux_amd64.tar.gz; then
        echo "Failed to extract archive."
        exit 1
    fi
    cd ../../

    echo "Building binary for Linux platform..."
    if g++ -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -I ./build/tmp/raylib-5.5_linux_amd64/include ./build/tmp/raylib-5.5_linux_amd64/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 1
    fi
else
    if ! wget https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip; then
        echo "Failed to download library."
        exit 1
    fi
    
    echo "Extracting raylib archive..."
    if ! unzip raylib-5.5_win64_mingw-w64.zip; then
        echo "Failed to extract archive."
        exit 1
    fi
    cd ../../

    echo "Building binary for Windows platform..."
    if x86_64-w64-mingw32-g++ -o ./build/pocket.exe ./src/*.hpp ./src/*.cpp ./src/levels/*.cpp -I ./build/tmp/raylib-5.5_win64_mingw-w64/include ./build/tmp/raylib-5.5_win64_mingw-w64/lib/libraylib.a -lgdi32 -lwinmm; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 3
    fi
fi

echo "Deleting temporary files..."
rm -rf ./build/tmp

echo "Done."
exit 0