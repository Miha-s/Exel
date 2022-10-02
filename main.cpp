#include "Parser.hpp"

using namespace parser;

int main() 
{

	Tockenizer tock;
	Parser pars(tock);

	auto vec = pars.parse("mmax(123, 44^1, 34*3-1^3)");
	(void)vec;

	return 0;
}