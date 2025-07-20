CPPSRC = src/SnakeGame.cpp src/FrameBare.cpp
HPPSRC = src/SnakeGame.hpp src/FrameBare.hpp

main: main.cpp $(CPPSRC) $(HPPSRC)
	g++ -g main.cpp $(CPPSRC) -lncurses -o main

run: main
	./main

clean:
	rm main