#include "testf4.h"
#include"pch.h"
#include "Info.h"
#include"F4Solver.h"
namespace FST {
	void FST::test_f4() {
		std::vector<std::string>titles;
		std::string kw = "meltdown";
		F4Solver x;
		x.ImportFromFile();
		bool res = x.SearchTitles(kw, titles);
		for (int i = 0; i < titles.size(); i++)
			std::cout << titles[i] << std::endl;
		std::pair<OPRESULT, std::vector<Info> >pinfo = x.SearchInfo(titles[0]);
	}
}