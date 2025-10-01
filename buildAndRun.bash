gcc -o debug main.cpp -I ./raylib/include ./raylib/linux/lib/libraylib.a -no-pie -D_DEFAULT_SOURCE -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
./debug