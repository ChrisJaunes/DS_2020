#pragma once
#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
class FSolver
{
	DblpBptMs* ms;
public:
	FSolver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist);
	~FSolver();
	std::pair<OPRESULT, Author> F1_getAuthorByName(const MYSTR&);
	std::pair<OPRESULT, std::vector<Info>>   F1_getInfoByTitle(const MYSTR&);
	std::vector<Author> F2_getTop100();
};
