#pragma once
#include "FrequencyRanking.h"
#include "BPTrees.h"

#define STR bstr_t

wchar_t* charToWChar(const char* text);

class CalcTime {
	ULONG64 start_time;
public:
	CalcTime();
	~CalcTime();
};
