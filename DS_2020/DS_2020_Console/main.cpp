#define CRTDBG_MAP_ALLOC  //���ڳ�����ǰ
#include <stdlib.h>    
#include <crtdbg.h>

#include "pch.h"
#include "config.h"
#include "error.h"

#include "Info.h"
#include "Author.h"
#include "DblpBptMs.h"
#include "CommUtils.h"

#include "generate_cache.h"

#include "test_BPT.h"
#include "test_AUTHOR.h"
#include "test_INFO.h"
#include "test_dblp.h"

int _tmain()
{
    CalcTime timer;
    generate_cache(DS_DBLP, XMLPARSETYPE_alltypes);
    FST::test_BPT(0);
    FST::test_AUTHOR(0);
    FST::test_INFO(0);
    FST::test_DBLP(0);
    
    //FST::test_xmlparse();

    //FST::test_BPT();
    //FST::test_AUTHOR0();
    //FST::test_INFO1();
    //FST::test_INFO1
    _CrtDumpMemoryLeaks();
    return 0;
}