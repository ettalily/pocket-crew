gcc -o pocket ./src/global.hpp ./src/player.cpp ./src/main.cpp -I ./raylib/include ./raylib/linux/lib/libraylib.a -no-pie -D_DEFAULT_SOURCE -lGL -lm -lpthread -ldl -lrt -lX11
