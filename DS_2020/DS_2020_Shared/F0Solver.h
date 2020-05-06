#pragma once
#include "pch.h"
#include "Info.h"
#include "Author.h"
#include "BPTMS.h"
#include "Solver.h"
class F0Solver : public ISolver {
	BPTMS<key_t, Info>* info_ms;
	BPTMS<key_t, Author>* author_ms;
public:
	F0Solver(BPTMS<key_t, Info>* info_ms, BPTMS<key_t, Author>* author_ms);
	~F0Solver();
	void GenerateCache();
	void InsertObject(Info& _info);
};