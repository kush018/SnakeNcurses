CPPSRC = src/SnakeGame.cpp src/Frame.cpp
HPPSRC = src/SnakeGame.hpp src/Frame.hpp

main: main.cpp $(CPPSRC) $(HPPSRC)
	g++ -g main.cpp $(CPPSRC) -lncurses -o main

run: main
	./main

clean:
	rm main