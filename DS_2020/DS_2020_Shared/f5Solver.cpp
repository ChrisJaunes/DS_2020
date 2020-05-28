#include "pch.h"
#include "f5Solver.h"
#include <fstream>
using namespace std;

F5Solver::F5Solver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist)
{
	ms = new DblpBptMs(info_bpt_file, author_bpt_file, exist);
	pF5 = new CliquesCounting(ms);
}

F5Solver::~F5Solver()
{
	delete ms;
	delete pF5;
}

void F5Solver::ExportToFile()
{
	//pF5->initial();
	pF5->InsertObject();
	pF5->writefile_f5result();
}
std::map<W,W> F5Solver::ImportFromFile()
{
	pF5->readfile_f5result();
	return pF5->GetResult();
}
/*
void F5Solver::InsertObject() 
{
	for (auto it = ms->author_bpt.begin(); it != ms->author_bpt.end(); ++it) 
	{
		auto off = *it;
		void* value = nullptr; size_t value_sz = 0;
		ms->author_bpt.valueFromFileBlock(off, value, value_sz);
		Author author = Author::deserialize((wchar_t*)value);
		pF5->Insert(&author);
		
#ifdef TEST_DEBUG
		ins_cnt++;
		printf("%d\n", ins_cnt);
#endif		

	}
}
*/
std::map<ULONG64, ULONG64>  F5Solver::GetResult() {
	return pF5->GetResult();
}

