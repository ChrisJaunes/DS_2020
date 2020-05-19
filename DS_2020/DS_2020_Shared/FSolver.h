#pragma once
#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
class FSolver
{
	DblpBptMs* ms;
public:
	std::pair<OPRESULT, Author> F1_getAuthorByName(const MYSTR&);
	std::pair<OPRESULT, Info>   F1_getInfoByTitle(const MYSTR&);
	std::vector<Author> F2_getTop100();
};
