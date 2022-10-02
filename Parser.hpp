#include "Tockenizer.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace parser {
	using namespace std;

	double to_number(const std::string& s);

	class Parser {
		Tockenizer& tockenizer;
		tocken_vec tockens;
		tocken_vec::iterator iter;
		Tocken& current() { return *iter; }
		void advance() { 
			if(iter++ == tockens.end())
				throw exception();
		}
	public:
		Parser(Tockenizer& tok) : tockenizer(tok) {}
		double parse(const string& expression);
		double add_expr();
		double mul_expr();
		double pow_expr();
		double primary();
	};
}