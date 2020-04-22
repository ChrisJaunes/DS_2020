#include "Info.h"
#include "Author.h"
namespace FST {
	const int AUTHORSN = 10;
	const int INFON = 10;
	extern Author* AUTHORS[AUTHORSN];
	extern Info* INFO[INFON];
	bstr_t getStr(char ch);
	void create_FST();
}