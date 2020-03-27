#include "pch.h"
#include "xmlhelper.h"
#include "FrequencyRanking.h"
#include "Info.h"
#include "Author.h"
#include "BPTrees.h"

#define STR bstr_t
int _tmain()
{
    DWORD start_time = GetTickCount64();
    ImportDataWrapper(L"D:\\Code\\ds_hw\\dblp.xml");
    std::vector<STR> a=ImportData.f3_pFrequencyRanking->Get(STR(L"1988"));
    for (auto i : a) {
        wcout << (wchar_t*)i << endl;
    }
    DWORD end_time = GetTickCount64();
    std::cout << "The run time is:" << (end_time - start_time) * 1.00 / 1000 << "s!" << std::endl;//输出运行时间
	getc(stdin);
	return 0;
}