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
	std::vector<Author> F2_getTop100();
	bool F4_KeywordSearch(std::string&, std::vector<std::string>&);
	std::map<W, W> F5_getResult();
	std::map<W, MYSTR> F5_getResult_2();
};
