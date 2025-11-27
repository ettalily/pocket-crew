g++ -o debug ./src/*.hpp ./src/*.cpp ./src/levels/*cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./debug
rm debug