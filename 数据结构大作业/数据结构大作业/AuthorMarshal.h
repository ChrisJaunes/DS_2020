#pragma once
#include "Author.h"
#define STR bstr_t
/*
	Author* AUTHORS[10];
	AUTHORS[0] = new Author;
	AUTHORS[0]->SetName(L"sb");
	std::vector<bstr_t> tmp;
	tmp.push_back(L"B01");
	tmp.push_back(L"B02");
	tmp.push_back(L"B03");
	STR t1 = bstr_t(L"A1");
	STR t2 = bstr_t(L"A2");
	STR t3 = bstr_t(L"A3");
	STR t4 = bstr_t(L"A4");

	AUTHORS[0]->AddArticle(t1, tmp);
	tmp.clear();
	tmp.push_back(L"B01");
	tmp.push_back(L"C03");
	tmp.push_back(L"C04");
	AUTHORS[0]->AddArticle(t2, tmp);
	tmp.clear();
	tmp.push_back(L"B01");
	tmp.push_back(L"D03");
	tmp.push_back(L"D04");
	AUTHORS[0]->AddArticle(t3, tmp);
	tmp.clear();
	tmp.push_back(L"B01");
	tmp.push_back(L"D03");
	tmp.push_back(L"E04");
	AUTHORS[0]->AddArticle(t4, tmp);

	STR res = AuthorMarshal::Marshal(*AUTHORS[0]);
	_tprintf(L"%s\n",res.GetBSTR());
	Author r=AuthorMarshal::Unmarshal(res);

*/
class AuthorMarshal {
public:
	// 用到了CommUtils.h -> charToWchar函数, 好像不支持中文
	static STR Marshal(Author);
	static Author Unmarshal(STR);
};