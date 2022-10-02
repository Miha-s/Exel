#include "Parser.hpp"
#include "Window.hpp"
#include <FL/Fl_Table_Row.H>

using namespace parser;
using namespace window;

int main() 
{

	Tockenizer tock;
	Parser pars(tock);

	string expr;
	window::Window win;

	
	
	Fl::run();
	return 0;
}