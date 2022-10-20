#include "Parser.hpp"

namespace parser {

	double to_number(const std::string& s)
	{
		istringstream ist{s};

		// Tell the input stream to throw an exception if it can't read the number.
		ist.exceptions(std::ios_base::failbit);

		double x;
		try {
			ist >> x;
		} catch (...) {
			throw runtime_error("BADNUMBER");
		}
		return x;
	}
	void Parser::save_vec_and_iter(tocken_vec& vec, tocken_vec::iterator& iter_)
	{
		auto dist = iter - tockens.begin();
		vec = move(tockens);
		iter_ = vec.begin();
		iter_ += dist;
	}
	void Parser::load_vec_and_iter(tocken_vec& vec, tocken_vec::iterator& iter_)
	{
		auto dist = iter_ - vec.begin();
		tockens = move(vec);
		iter = tockens.begin();
		iter += dist;
	}
	double Parser::parse(const string& expression)
	{
		// saving previous vector of tockens
		tocken_vec tmp_vec;
		tocken_vec::iterator tmp_iter;
		save_vec_and_iter(tmp_vec, tmp_iter);

		tockens = tockenizer.parse(expression);
		iter = tockens.begin();
		double res;
		try {
			res = add_expr();
		} catch(...) {
			recursion_list.clear();
			throw;
		}
		
		load_vec_and_iter(tmp_vec, tmp_iter);
		return res;
	}

	double Parser::add_expr()
	{
		double result = mul_expr();

		for(;;) {
			switch (current().mType)
			{
			case ADD:
				advance();
				result += mul_expr();
				break;
			case SUBSTRACT:
				advance();
				result -= mul_expr();
				break;
			default:
				return result;
			}
		}
	}

	double Parser::mul_expr()
	{
		double result = pow_expr();
		double test_zero;

		for(;;) {
			switch (current().mType)
			{
			case MULTIPLY:
				advance();
				result *= mul_expr();
				break;
			case DIVIDE:
				advance();
				test_zero = mul_expr();
				if(test_zero == 0)
					throw runtime_error("DIVZERO");
				result /= test_zero;
				break;
			case MOD:
				advance();
				result = long(result) % long(mul_expr());
				break;
			default:
				return result;
			}
		}
	}

	double Parser::pow_expr()
	{
		double result = primary();

		if(current().mType == POW) {
			advance();
			result = pow(result, primary());
		}
		return result;
	}

	double Parser::primary() 
	{
		string text = current().mText;
		double arg;
		vector<double> parameters;
		TockenType func;
		string tmp_expr;
		Tocken saved_tocken;


		switch (current().mType)
		{
		case INTEGER_LITERAL:
			advance();
			return to_number(text);
		case BRACKET:
			advance();
			arg = add_expr();
			advance();
			return arg;
		case MMAX:
		case MMIN:
			func = current().mType;
			advance();
			do {
				advance();
				parameters.push_back(add_expr());
			} while(current().mType != BRACKET);
			advance();
			if(parameters.size() == 0)
				throw runtime_error("NOPARAMETERS");
			if(func == MMAX)
				return *max_element(parameters.begin(), parameters.end());
			else	
				return *min_element(parameters.begin(), parameters.end());
		case CELL:
			if(find(recursion_list.begin(), recursion_list.end(), current()) != recursion_list.end()) {
				throw runtime_error("RECURSION");
			}

			recursion_list.push_back(current());
			advance();
			arg = get_cell(recursion_list.back().mText);
			recursion_list.pop_back();
			return arg;
		default:
			throw runtime_error("BADTOCKEN");
		}
	}
}