gcc -o debug ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -I ./raylib/include ./raylib/linux/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
./debug
rm debug