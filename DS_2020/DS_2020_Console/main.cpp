#define CRTDBG_MAP_ALLOC  //放在程序最前
#include <stdlib.h>    
#include <crtdbg.h>

#include "pch.h"
#include "config.h"
#include "error.h"

#include "Info.h"
#include "Author.h"
#include "CommUtils.h"

#include "generate_cache.h"
#include "F1Solver.h"

#include "test_xml.h"
#include "test_BPT.h"
#include "test_AUTHOR.h"
#include "test_INFO.h"

int _tmain()
{
    CalcTime timer;
    //FST::test_xmlparse();
    generate_cache(DS_DBLP, XMLPARSETYPE_alltypes);

    //FST::test_BPT();
    //FST::test_AUTHOR0();
    //FST::test_INFO1();
    //FST::test_INFO1
    _CrtDumpMemoryLeaks();
    return 0;
}