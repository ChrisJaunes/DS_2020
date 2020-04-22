#include "pch.h"
#include "CommUtils.h"
wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}
CalcTime::CalcTime()
{
	std::cout << "CalcTime start" << std::endl;
	start_time = GetTickCount64();

}
CalcTime::~CalcTime()
{
	DWORD end_time = GetTickCount64();
	std::cout << "The run time is:" << (end_time - start_time) * 1.00 / 1000 << "s!" << std::endl;//输出运行时间
	getc(stdin);
}
