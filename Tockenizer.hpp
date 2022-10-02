#include <string>
#include <vector>


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
		public:
			vector<Tocken> parse(const string& expression); 
		private:
			void endTocken(Tocken& tock, vector<Tocken>& vec);
	};
}; // namespace parser
