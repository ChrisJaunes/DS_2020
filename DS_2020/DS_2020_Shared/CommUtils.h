#pragma once
#include "pch.h"
#include "FrequencyRanking.h"
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

template<class Function>
void runBlock(Function func, const char* msg) {
	clock_t start = clock();
	func();
	clock_t end = clock();
	printf("%s use: %f ms\n", msg, 1000.0 * (end - start) / CLOCKS_PER_SEC);
}
