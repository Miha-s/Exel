#include "Parser.hpp"
#include "Window.hpp"
#include <FL/Fl_Table_Row.H>

using namespace parser;
using namespace window;

int main() 
{
	string expr;
	window::Window win;

	std::cout << to_string(123.123) << std::endl;
	
	Fl::run();
	return 0;
}