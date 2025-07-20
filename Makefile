CPPSRC = src/SnakeGame.cpp src/FrameBare.cpp src/StatusBar.cpp src/SnakeApp.cpp
HPPSRC = src/SnakeGame.hpp src/FrameBare.hpp src/StatusBar.hpp src/SnakeApp.hpp

main: main.cpp $(CPPSRC) $(HPPSRC)
	g++ -g main.cpp $(CPPSRC) -lncurses -o main

run: main
	./main

clean:
	rm main