#include <fstream>
#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
#include "CommUtils.h"
#include "xmlhelper.h"
#include "f4.h"

bool is_bptfile_exist(LPCWSTR info_bptfile, LPCWSTR author_bptfile) {
    WIN32_FIND_DATA  FindFileData, FindFileData1;
    HANDLE hFind, hFind1;
    hFind = FindFirstFile(info_bptfile, &FindFileData);
    hFind1 = FindFirstFile(author_bptfile, &FindFileData1);
    bool result = false;
    if (hFind == INVALID_HANDLE_VALUE || hFind1 == INVALID_HANDLE_VALUE)
    {
        result = false;
        goto Clean;
    }
    else
    {
        if (
            ((FindFileData.nFileSizeHigh == 0) && (FindFileData.nFileSizeLow == 0))
            || ((FindFileData1.nFileSizeHigh == 0) && (FindFileData1.nFileSizeLow == 0))
            ) 
        {
            result = false;
            goto Clean;
        }
        else {
            result = true;
            goto Clean;
        }
    }

Clean:
    result ? MyLog::d("exist") : MyLog::d("not exist");
    FindClose(hFind);
    FindClose(hFind1);
    return result;
}

void generate_bptfile_cache(const wchar_t* xmlfile, const DWORD parseInfo)
{	
	if (is_bptfile_exist(DS_DBLP_Info, DS_DBLP_Author)) { return; }
	XMLParser *pParser = new XMLParser(parseInfo);
	DblpBptMs*f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author);
	pParser->ParseFile(xmlfile, f);
	delete f;
	delete pParser;
}
void generate_f2_cache() {
    std::wfstream ofs;
    ofs.open(DS_F2, std::ios::out);
    DblpBptMs* f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author, FILE_Status::EXIST);
    auto vec = f->getTop100();
    //std::vector<Author> vec;
    //vec.push_back(Author(L"5421", std::map<MYSTR, std::vector<MYSTR>>()));
    //vec.push_back(Author(L"52", std::map<MYSTR, std::vector<MYSTR>>()));
    //vec.push_back(Author(L"5445221", std::map<MYSTR, std::vector<MYSTR>>()));
    for (auto it : vec) {
        ofs << (wchar_t*)it.GetName().second << "\n" << it.GetNumOfArticle() << std::endl;
    }
    delete f;
    ofs.close();
}
void generate_cache(const wchar_t* xmlfile, const DWORD parseInfo) {
    //generate_bptfile_cache(xmlfile, parseInfo);
    generate_f2_cache();
}