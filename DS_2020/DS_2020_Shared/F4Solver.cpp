#include "pch.h"
#include "F4Solver.h"
#include <fstream>
using namespace std;

F4Solver::F4Solver(const TCHAR* xmlfile, DWORD parseInfo)
{
	pParser = new XMLParser(parseInfo);
	//pParser->ParseFile(xmlfile, this);
	delete pParser;
}

F4Solver::~F4Solver()
{
	delete pF4;
}

//void F4Solver::ExportToFile(const TCHAR* filename)
//{
//	// 1936-2020 868.157s 6G
//	std::vector<MYSTR> a = pF4->GetYears();
//	wofstream ofs;
//	ofs.open(filename, ios::out);
//
//	for (auto i : a) {
//		ofs << (wchar_t*)i << L"\n";
//		for (auto j : pF3->Get(i)) {
//			ofs << (wchar_t*)j.first << L"\t" << j.second << endl;
//		}
//		ofs << L"---" << endl;
//	}
//
//	ofs.close();
//}
//
//std::map < MYSTR, std::map<MYSTR, ULONG64> > F4Solver::ImportFromFile(const TCHAR* filename)
//{
//	wifstream ifs;
//	std::map < MYSTR, std::map<MYSTR, ULONG64> > result;
//	ifs.open(filename, ios::in);
//	wchar_t buffer[0x200]{ 0 };
//	while (ifs >> buffer) {
//		MYSTR year(buffer);
//		std::map<MYSTR, ULONG64> result_peryear;
//		while (MYSTR(buffer) != MYSTR()) {
//			ifs >> buffer;
//			MYSTR word(buffer);
//			if (MYSTR(buffer) == MYSTR(L"---")) {
//				break;
//			}
//			ULONG64 times;
//			ifs >> times;
//			result_peryear.insert(std::make_pair(word, times));
//		}
//		result.insert(std::make_pair(year, result_peryear));
//	}
//	ifs.close();
//	return result;
//}

void F4Solver::InitMemory() {
	pF4 = new f4func();
	pF4->initial();
}
bool F4Solver::SearchTitles(std::string&keywords, std::vector<std::string>&titles) {
	return pF4->keyword_search(keywords, titles);
}
std::vector<Info> F4Solver::SearchInfo(std::vector<std::string>&titles) {
	std::vector<Info> res;
	for (int i = 0; i < titles.size(); i++) {
		//string转换wchar*

		//wchar*获取info

		//info放入res
	}
	return res;
}
