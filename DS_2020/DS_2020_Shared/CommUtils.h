#pragma once
#include "FrequencyRanking.h"
#include "BPTrees.h"

wchar_t* charToWChar(const char* text);

class CalcTime {
	ULONG64 start_time;
public:
	CalcTime();
	~CalcTime();
};
