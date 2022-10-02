#include <string>
#include <vector>
#include <locale>


namespace parser
{
	using namespace std;

	enum TockenType {
		WHITESPACE,
		INTEGER_LITERAL,
		STRING_LITERAL,
		OPERATOR,
		MOD, // %
		DIVIDE, 
		MULTIPLY,
		ADD,
		SUBSTRACT,
		POW,
		MMAX,
		MMIN,
		BRACKET,
		POTENTIAL_CELL,
		CELL, 
		COMMA
	};

	class Tocken {
	public:
		TockenType mType{WHITESPACE};
		string mText;
		Tocken() {}
		Tocken(TockenType t, string s = string()) : mType(t), mText(s) {}
	};

	typedef vector<Tocken> tocken_vec;

	class Tockenizer {
			tocken_vec tockens;
			Tocken current;
		public:
			tocken_vec parse(const string& expression); 
		private:
			void sub_divide_tockens();
			void endTocken();
	};
}; // namespace parser
