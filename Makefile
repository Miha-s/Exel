
GTKFLAGS = `pkg-config gtkmm-4.0 --cflags --libs`

calc: main.cpp Parser.cpp Tockenizer.cpp
	g++ -g -Wall main.cpp Parser.cpp Tockenizer.cpp $(GTKFLAGS) -o calc

tests: tests.h
	cxxtestgen --error-printer -o tests.cpp tests.h
	g++ -o tests -I$CXXTEST tests.cpp Parser.cpp Tockenizer.cpp