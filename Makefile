main: main.cpp src/SnakeGame.hpp
	g++ -g main.cpp -lncurses -o main

run: main
	./main

clean:
	rm main