mkdir -p build
cp -r assets ./build/assets
gcc -o ./build/pocket ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -I ./raylib/include ./raylib/linux/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
