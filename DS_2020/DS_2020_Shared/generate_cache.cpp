#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
#include "xmlhelper.h"
void generate_cache(const wchar_t* xmlfile, const DWORD parseInfo)
{	
	XMLParser *pParser = new XMLParser(parseInfo);
	DblpBptMs*f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author);
	pParser->ParseFile(xmlfile, f);
	delete f;
	delete pParser;
}