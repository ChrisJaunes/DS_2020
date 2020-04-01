#include "pch.h"
#include "F1Solver.h"
#include "xmlhelper.h"
#include "CommUtils.h"

#include "BPTrees.h"

F1Solver::F1Solver(const TCHAR* xmlfile)
{
    // Ä¬ÈÏÖ»½âÎöarticle
    ImportDataWrapper(xmlfile, this);
}

F1Solver::F1Solver(const TCHAR* xmlfile, DWORD parseInfo)
{
    ImportDataWrapperEx(xmlfile, this, parseInfo);
}

void F1Solver::InitMemory()
{
    ImportData.authorTree = new BPTree<Info, STR>;
    ImportData.titleTree = new BPTree<Info, STR>;
}

void F1Solver::InsertObject(Info temp)
{
    ImportData.titleTree->insert(temp, STR(temp.GetProperty(STR(L"title")).at(0)));
}

