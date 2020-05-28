#include "pch.h"
#include "I5Solver.h"

I5Solver::I5Solver(const TCHAR* xmlfile, DWORD parseInfo)
{
	pF5 = new CliquesCounting();
	pParser = new XMLParser(parseInfo);
	pParser->ParseFile(xmlfile, this);
	delete pParser;
}

I5Solver::~I5Solver()
{
	delete pF5;
}
void I5Solver::InitMemory() {
	//pF5 = new CliquesCounting();
}

void I5Solver::InsertObject(Info& temp)
{
	pF5->insert(temp);
}

void I5Solver::WriteToFile() {
	pF5->initial();
	pF5->writefile_f5result(pF5->getcnt());
}