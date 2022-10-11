#pragma once
#include "Tockenizer.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <list>

namespace parser {
	using namespace std;
	double to_number(const std::string& s);

	class Parser {
		Tockenizer tockenizer;
		tocken_vec tockens;
		tocken_vec::iterator iter;
		list<Tocken> recursion_list;

		Tocken& current() { return *iter; }
		void advance() { 
			if(iter++ == tockens.end())
				throw exception();
		}
		function<double(string)> get_cell;
	public:
		Parser(function<double(string)> get_cell_num) : get_cell(get_cell_num) {}
		Parser(Tockenizer& tok, function<double(string)> get_cell_num) : tockenizer(tok), get_cell(get_cell_num) {}
		double parse(const string& expression);
		double add_expr();
		double mul_expr();
		double pow_expr();
		double primary();

		void save_vec_and_iter(tocken_vec& vec, tocken_vec::iterator& iter);
		void load_vec_and_iter(tocken_vec& vec, tocken_vec::iterator& iter_);
	};
}