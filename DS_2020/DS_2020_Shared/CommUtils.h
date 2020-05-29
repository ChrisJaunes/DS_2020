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
	printf("%s use: %f ms\n", msg, 1000.0 * ((LONG64)end - start) / CLOCKS_PER_SEC);
}

class MyLog {
public:
	static void v(const char* msg) {
		printf("%s", msg);
	}
	static void d(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		vprintf(msg, ap);
		va_end(ap);
	}
	static void i(const char* msg) {
		printf("%s", msg);
	}
	static void e(const char* msg, ...) {
		puts("ERROR!!!!!!!!!!!!!!!");
		va_list ap;
		va_start(ap, msg);
		vprintf(msg, ap);
		va_end(ap);
	}
	static void ew(const wchar_t* msg, ...) {
		puts("ERROR!!!!!!!!!!!!!!!");
		va_list ap;
		va_start(ap, msg);
		vwprintf(msg, ap);
		va_end(ap);
	}
};