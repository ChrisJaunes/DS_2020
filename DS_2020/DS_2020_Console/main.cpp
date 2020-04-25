#include "pch.h"
#include "config.h"
#include "error.h"

#include "Info.h"
#include "Author.h"
#include "CommUtils.h"

#include "F1Solver.h"

#include "test_BPT.h"
#include "test_AUTHOR.h"
#include "test_INFO.h"

int _tmain()
{
    //INFO的序列化和反序列化对中文支持仍然有问题
    CalcTime timer;
    FST::test_BPT();
    FST::test_AUTHOR0();
    FST::test_INFO0();
    FST::test_INFO1();
    return 0;
}