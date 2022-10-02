#include "Parser.hpp"

using namespace parser;

int main() 
{

	Tockenizer tock;
	Parser pars(tock);

	string expr;
	while (true)
	{
		// mmax(mmin(2+4, 3^2, 5*6), (2-1)-40, 234)
		getline(cin, expr);
		cout << pars.parse(expr) << endl;
	}
	

	return 0;
}