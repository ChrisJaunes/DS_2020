#pragma once
//本文件用于测试
#ifndef TEST_H
#define TEST_H
#define TEST_DEBUG
#include "Author.h"
#include "Article.h"
#include <qstring.h>
namespace FST {
	const int AUTHORSN = 10;
	const int ARTICLESN = 10;
	extern Author  *AUTHORS[AUTHORSN];
	extern Article *ARTICLES[ARTICLESN];
	bstr_t getStr(char ch);
	void create_FST();
}
#endif