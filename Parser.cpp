#include "Parser.hpp"

namespace parser {

	double to_number(const std::string& s)
	{
		istringstream ist{s};

		// Tell the input stream to throw an exception if it can't read the number.
		ist.exceptions(std::ios_base::failbit);

		double x;
		ist >> x;
		return x;
	}
	double Parser::parse(const string& expression)
	{
		tockens = tockenizer.parse(expression);
		iter = tockens.begin();
		return add_expr();
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

		for(;;) {
			switch (current().mType)
			{
			case MULTIPLY:
				advance();
				result *= mul_expr();
				break;
			case DIVIDE:
				advance();
				result /= mul_expr();
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
				throw runtime_error("too few parameters to funciton");
			if(func == MMAX)
				return *max_element(parameters.begin(), parameters.end());
			else	
				return *min_element(parameters.begin(), parameters.end());
		case CELL:
			tmp_expr = current().mText;
			advance();
			return get_cell(tmp_expr);
		default:
			throw runtime_error("unknown tocken");
		}
	}
}