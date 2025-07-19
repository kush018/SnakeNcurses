main: main.cpp src/SnakeGame.hpp src/SnakeGame.cpp
	g++ -g main.cpp src/SnakeGame.cpp -lncurses -o main

run: main
	./main

clean:
	rm main