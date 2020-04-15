#pragma once
//本文件用于测试
#ifndef TEST_H
#define TEST_H
#define TEST_DEBUG
#define TEST_DEBUG_INFO
#define TEST_DEBUG_AUTHOR
#define TEST_DEBUG_TOP
#define TEST_DEBUG_INFO_TITLE
//#define TEST_DEBUG_HOTSPOT
#ifdef TEST_DEBUG
#include "Author.h"
#include "Info.h"
#include <qstring.h>
namespace FST {
	const int AUTHORSN = 10;
	const int INFON = 10;
	extern Author *AUTHORS[AUTHORSN];
	extern Info   *INFO[INFON];
	bstr_t getStr(char ch);
	void create_FST();
	void tmain();
}
#endif

#endif