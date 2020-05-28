#pragma once
#include "pch.h"
#include "Info.h"
#include "config.h"
#include "f5Solver.h"
#include "xmlhelper.h"
#include "DblpBptMs.h"
#include "CliquesCounting.h"

namespace FST {
	void test_f5(DWORD flag) {
		F5Solver f5(DS_DBLP_Info, DS_DBLP_Author, FILE_Status::EXIST);
		f5.ExportToFile();
	}
}