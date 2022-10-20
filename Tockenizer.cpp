#include "Tockenizer.hpp"
#include <iostream>

namespace parser {
	using namespace std;

	vector<Tocken> Tockenizer::parse(const string& expression) {
		tockens.clear();
		current = WHITESPACE;
		int brackets = 0;

		for(auto iter = expression.cbegin(); iter != expression.cend(); iter++) {
			switch (*iter)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(current.mType == INTEGER_LITERAL) {
					current.mText.append(1, *iter);
				} else if(current.mType == STRING_LITERAL || current.mType == CELL) {
					current.mText.append(1, *iter);
					current.mType = CELL;
				} else {
					endTocken();
					current.mType = INTEGER_LITERAL;
					current.mText.append(1, *iter);
				}
				break;
			case '-':
			case '+':
			case '/':
			case '*':
			case '%':
			case '^':
				endTocken();
				current.mText.append(1, *iter);
				current.mType = OPERATOR;
				endTocken();
				break;
			case ',':
				endTocken();
				current.mText.append(1, *iter);
				current.mType = COMMA;
				endTocken();
				break;
			case ')':
			case '(':
				if(*iter == '(')
					brackets++;
				else if(brackets > 0)
					brackets--;
				else
					throw runtime_error("BADBRACKET");
				endTocken();
				current.mType = BRACKET;
				current.mText.append(1, *iter);
				endTocken();
				break;
			default:
				if((*iter >= 'a' && *iter <= 'z') || (*iter >= 'A' && *iter <= 'Z'))
				{
					if(current.mType != WHITESPACE && current.mType != STRING_LITERAL)
						throw runtime_error("BADLITERAL");
					current.mType = STRING_LITERAL;
					current.mText.append(1, tolower(*iter));
				} else if (*iter != ' ' && *iter != '\t' && *iter != '\n') {
					throw runtime_error("BADLITERAL");
				}
				break;
			}
		}
		if(brackets > 0)
			throw runtime_error("BADBRACKET");
		endTocken();
		sub_divide_tockens();

		tockens.push_back(Tocken(WHITESPACE));
		return tockens;
	}
	void Tockenizer::sub_divide_tockens()
	{
		for(auto& elem : tockens) {
			if(elem.mType == OPERATOR) {
				switch (elem.mText[0])
				{
				case '+':
					elem.mType = ADD;
					break;
				case '-':
					elem.mType = SUBSTRACT;
					break;
				case '*': 
					elem.mType = MULTIPLY;
					break;
				case '/':
					elem.mType = DIVIDE;
					break;
				case '%':
					elem.mType = MOD;
					break;
				case '^':
					elem.mType = POW;
					break;
				default:
					break;
				}
			} else if(elem.mType == STRING_LITERAL) {
				if(elem.mText == "mmax") 
					elem.mType = MMAX;
				else if(elem.mText == "mmin")
					elem.mType = MMIN;
				else if(validateCoords(elem.mText))
					elem.mType = CELL;
				else
					throw runtime_error("BADLITERAL");
			}
		}
	}
	bool Tockenizer::validateCoords(const string& coords) {
		int i = 0;
		int size = coords.size();
		while (i < size && isalpha(coords[i]))
		{
			i++;
		}
		if(!(size - i)) return false;
		while (i < size && isdigit(coords[i]))
		{
			i++;
		}
		if(i != size)
			return false;
		return true;
	}

	void Tockenizer::endTocken() 
	{
		if(current.mType != WHITESPACE)
			tockens.push_back(current);

		current.mText.erase();
		current.mType = WHITESPACE;
	}

}