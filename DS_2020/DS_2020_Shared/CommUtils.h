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

class NoCopy {
public:
	NoCopy() = default;
private:
	NoCopy(const NoCopy&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};