#include "pch.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "CommUtils.h"

#include <fstream>

F3Solver::F3Solver(const TCHAR* xmlfile)
{
    ImportDataWrapper(xmlfile);

}

void F3Solver::ExportToFile(const TCHAR* filename)
{
    // 1936-2020 868.157s 6G
    std::vector<STR> a = ImportData.f3_pFrequencyRanking->GetYears();
    wofstream ofs;
    ofs.open(filename, ios::out);

    for (auto i : a) {
        ofs << (wchar_t*)i << L"\n";
        for (auto j : ImportData.f3_pFrequencyRanking->Get(i)) {
            ofs << (wchar_t*)j.first <<L"\t"<< j.second << endl;
        }
        ofs << endl;
    }

    ofs.close();
}
