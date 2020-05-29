#pragma once
#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
#include "CliquesCounting.h"

class FSolver
{
	DblpBptMs* ms;
	f4func* f4p;
	CliquesCounting* pf5;

public:
	FSolver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist);
	~FSolver();
	std::pair<OPRESULT, Author> F1_getAuthorByName(const MYSTR&);
	std::pair<OPRESULT, std::vector<Info>>   F1_getInfoByTitle(const MYSTR&);
	std::vector<std::pair<MYSTR, MYW>> F2_getTop100(const TCHAR* filename);
	std::map<MYSTR, std::map<MYSTR, ULONG64>> F3_getHotspot(const TCHAR*);
	bool F4_KeywordSearch(std::string&, std::vector<std::string>&);
	std::map<MYW, MYW> F5_getResult();
	std::map<MYW, MYSTR> F5_getResult_2();
};
