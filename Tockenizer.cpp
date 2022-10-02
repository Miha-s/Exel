#include "Tockenizer.hpp"

namespace parser {
	using namespace std;

	vector<Tocken> Tockenizer::parse(const string& expression) {
		vector<Tocken> tockens;
		Tocken current;
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
				} else if(current.mType == STRING_LITERAL) {
					current.mText.append(1, *iter);
					current.mType = CELL;
					endTocken(current, tockens);
				} else {
					endTocken(current, tockens);
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
				endTocken(current, tockens);
				current.mText.append(1, *iter);
				current.mType = OPERATOR;
				endTocken(current, tockens);
				break;
			case ',':
				endTocken(current, tockens);
				current.mText.append(1, *iter);
				current.mType = COMMA;
				endTocken(current, tockens);
				break;
			case ')':
			case '(':
				if(*iter == '(')
					brackets++;
				else if(brackets > 0)
					brackets--;
				else
					throw exception();
				endTocken(current, tockens);
				current.mType = BRACKET;
				current.mText.append(1, *iter);
				endTocken(current, tockens);
				break;
			case 'M':
			case 'm':
				if((iter+1) != expression.cend() && tolower(*(iter+1)) == 'm')
				if((iter+2) != expression.cend() && tolower(*(iter+2)) == 'a')
				if((iter+3) != expression.cend() && tolower(*(iter+3)) == 'x')
				{
					endTocken(current, tockens);
					current.mText.append("mmax");
					current.mType = MMAX;
					endTocken(current, tockens);
					iter += 3;
					continue;
				}
				if((iter+1) != expression.cend() && tolower(*(iter+1)) == 'm')
				if((iter+2) != expression.cend() && tolower(*(iter+2)) == 'i')
				if((iter+3) != expression.cend() && tolower(*(iter+3)) == 'n')
				{
					endTocken(current, tockens);
					current.mText.append("mmin");
					current.mType = MMIN;
					endTocken(current, tockens);
					iter += 3;
					continue;
				}
			default:
				if((*iter >= 'a' && *iter <= 'z') || (*iter >= 'A' && *iter <= 'Z'))
				{
					if(current.mType != WHITESPACE && current.mType != STRING_LITERAL)
						throw exception();
					current.mType = STRING_LITERAL;
					current.mText.append(1, tolower(*iter));
				} else if (*iter != ' ' && *iter != '\t' && *iter != '\n') {
					throw exception();
				}
				break;
			}
		}

		tockens.push_back(Tocken(WHITESPACE));
		return tockens;
	}

	void Tockenizer::endTocken(Tocken& tock, vector<Tocken>& vec) 
	{
		if(tock.mType == POTENTIAL_CELL)
			throw exception();
		if(tock.mType != WHITESPACE)
			vec.push_back(tock);

		tock.mText.erase();
		tock.mType = WHITESPACE;
	}

}