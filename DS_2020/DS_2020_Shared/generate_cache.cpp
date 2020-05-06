#include "pch.h"
#include "config.h"
#include "Info.h"
#include "Author.h"
#include "BPTMS.h"
#include "F0Solver.h"
#include "xmlhelper.h"
void generate_cache(const TCHAR* xmlfile, DWORD parseInfo, 
	BPTMS<key_t, Info> *info_ms,
	BPTMS<key_t, Author> *author_ms)
{
	XMLParser *pParser = new XMLParser(parseInfo);
	F0Solver *f = new F0Solver(info_ms, author_ms);
	pParser->ParseFile(xmlfile, f);
	delete f;
	delete pParser;
}