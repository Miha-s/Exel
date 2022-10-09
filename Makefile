
GTKFLAGS = `pkg-config gtkmm-4.0 --cflags --libs`

calc: main.cpp Parser.cpp Tockenizer.cpp
	g++ -g -Wall main.cpp Parser.cpp Tockenizer.cpp $(GTKFLAGS) -o calc