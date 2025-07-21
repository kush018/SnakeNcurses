CPPSRC = src/SnakeGame.cpp src/FrameBare.cpp src/StatusBar.cpp src/SnakeApp.cpp src/MainMenu.cpp
HPPSRC = src/SnakeGame.hpp src/FrameBare.hpp src/StatusBar.hpp src/SnakeApp.hpp src/MainMenu.hpp

main: main.cpp $(CPPSRC) $(HPPSRC)
	g++ -g main.cpp $(CPPSRC) -lncurses -o main

run: main
	./main

clean:
	rm main