#include "pch.h"
#include "config.h"
#include "DblpBptMs.h"
#include "xmlhelper.h"

bool is_bptfile_exist();

void generate_cache(const wchar_t* xmlfile, const DWORD parseInfo)
{	
	if (is_bptfile_exist()) { return; }
	XMLParser *pParser = new XMLParser(parseInfo);
	DblpBptMs*f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author);
	pParser->ParseFile(xmlfile, f);
	delete f;
	delete pParser;
}

bool is_bptfile_exist() {
    WIN32_FIND_DATA  FindFileData, FindFileData1;
    HANDLE hFind, hFind1;
    hFind = FindFirstFile(DS_DBLP_Info, &FindFileData);
    hFind1 = FindFirstFile(DS_DBLP_Author, &FindFileData1);
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
    result ? puts("exist") : puts("not exist");
    FindClose(hFind);
    FindClose(hFind1);
    return result;
}
