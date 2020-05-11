#pragma once
#include "pch.h"
#include "info.h"
#include "Solver.h"
#include "DblpBptMs.h"
namespace FST {
	class test_xml : public ISolver {
	public:
		test_xml(const DWORD types);
		~test_xml();
		void InitMemory();
		void InsertObject(Info& _info);
	private:
		int info_cnt;
	};
	void test_xmlparse();
}

