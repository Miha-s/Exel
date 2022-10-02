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
			advance();
			advance();
			while(current().mType != BRACKET) {
				parameters.push_back(add_expr());
				advance();
			}
			if(parameters.size() == 0)
				throw exception();
			if(current().mType == MMAX)
				return *max_element(parameters.begin(), parameters.end());
			else	
				return *min_element(parameters.begin(), parameters.end());
		default:
			throw exception();
		}
	}
}