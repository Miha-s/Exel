


calc: main.cpp Parser.cpp Tockenizer.cpp
	g++ -g -Wall main.cpp Parser.cpp Tockenizer.cpp Callbacks.cpp -lfltk -o calc