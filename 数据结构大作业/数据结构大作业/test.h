#pragma once
//���ļ����ڲ���
#ifndef TEST_H
#define TEST_H
#define TEST_DEBUG
#include "Author.h"
#include "Info.h"
#include <qstring.h>
namespace FST {
	const int AUTHORSN = 10;
	const int INFON = 10;
	extern Author  *AUTHORS[AUTHORSN];
	extern Info    *INFO[INFON];
	bstr_t getStr(char ch);
	void create_FST();
}
#endif