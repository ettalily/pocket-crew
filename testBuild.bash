gcc -o debug ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
./debug
rm debug
