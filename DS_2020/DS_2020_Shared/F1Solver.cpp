#include "pch.h"
#include "F1Solver.h"
#include "xmlhelper.h"
#include "CommUtils.h"

#include "BPTrees.h"

F1Solver::F1Solver(const TCHAR* xmlfile, DWORD parseInfo)
{
	pParser = new XMLParser(parseInfo);
	pParser->ParseFile(xmlfile, this);
	delete pParser;
}

F1Solver::~F1Solver()
{
	delete pF1;
}

void F1Solver::InitMemory()
{
	// TODO
    pF1 = new BPTree<Info, MYSTR>;
}

void F1Solver::InsertObject(Info temp)
{
	// TODO
	pF1->insert(temp, MYSTR(temp.GetProperty(MYSTR(L"title")).at(0)));
}

