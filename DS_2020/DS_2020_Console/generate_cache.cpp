#include "pch.h"
#include "Solver.h"

#include "Info.h"
#include "Author.h"
#include "CommUtils.h"

#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

#include "xmlhelper.h"

class GenerateCache : public ISolver {
	XMLParser* pParser;
public:
	GenerateCache(const TCHAR* xmlfile, DWORD parseInfo = article);
	~GenerateCache();

	void InitMemory();
	void InsertObject(Info temp);
};

GenerateCache::GenerateCache(const TCHAR* xmlfile, DWORD parseInfo)
{
	pParser = new XMLParser(parseInfo);
	pParser->ParseFile(xmlfile, this);
	delete pParser;
}

GenerateCache::~GenerateCache()
{
}

void GenerateCache::InitMemory()
{

}

void GenerateCache::InsertObject(Info temp)
{
}
