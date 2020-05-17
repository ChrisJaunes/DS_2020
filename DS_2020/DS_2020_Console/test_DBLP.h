#pragma once
#include "pch.h"
#include "info.h"
#include "Solver.h"
#include "DblpBptMs.h"
/*
 * @ChrisJaunes_hjj
 * test_DBLP
 * 1:    ≤‚ ‘dblp_xmlparse;
 * 2:    ≤‚ ‘dblp_bptfile
**/
namespace FST {
	class test_xml : public ISolver {
	public:
		test_xml(const DWORD types);
		~test_xml();
		void InitMemory();
		void InsertObject(Info& _info);
	private:
		int info_cnt;
		DblpBptMs* f;
	};
	void test_DBLP(DWORD flag);
}

