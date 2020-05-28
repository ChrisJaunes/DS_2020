#pragma once
#include "pch.h"
#include "Info.h"
#include "config.h"
#include "f5Solver.h"
#include "I5Solver.h"
#include "xmlhelper.h"
#include "DblpBptMs.h"
#include "CliquesCounting.h"

namespace FST {
	void test_f5(DWORD flag) {
		if (flag < 2) {
			F5Solver f5(DS_DBLP_Info, DS_DBLP_Author, FILE_Status::EXIST);
			if (flag == 0) f5.ExportToFile();
			if (flag == 1) f5.ExportToFile_2();
		}
		else if (flag == 2) {
			I5Solver i5(DS_DBLP, XMLPARSETYPE_article);
			i5.WriteToFile();
		}
	}
}