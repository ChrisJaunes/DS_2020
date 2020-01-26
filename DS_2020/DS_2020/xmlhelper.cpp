#include "article.h"
#include "xmlhelper.h"
#include "error.h"

/*
打开xml, 设置权限
*/
OPRESULT XMLParser::OpenFile(LPCSTR filename) 
{
	IStream* pFileStream = NULL;
	SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&pFileStream);
	CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL);
	pReader->SetInput(pFileStream);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);
	return 0;
}

/*
分析dtd, 加入到vector
*/
OPRESULT XMLParser::ParseArticles() 
{
	HRESULT hr;
	LPCWSTR szValue = NULL;
	while (S_OK == (hr = pReader->Read(&nodeType))) {
		pReader->GetLocalName(&szValue, NULL);
		if (nodeType == XmlNodeType_Element) {
			if (!lstrcmpW(szValue, L"article")) {
				//debug:
				if (DEBUG&&members.size()>0) {
					LPCWSTR tmp = new WCHAR[0x1000];
					members.at(members.size() - 1)->Gettitle(&tmp);
					printf("%ls\n", tmp);
					system("pause");
					delete[] tmp;
				}

				members.push_back(new Article);
				pReader->MoveToFirstAttribute();
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setmdate(szValue);

				pReader->MoveToNextAttribute();
				pReader->GetValue(&szValue, NULL); 
				members.at(members.size() - 1)->Setkey(szValue);

				// 从属性回到元素
				pReader->MoveToElement();
			}
			else if (!lstrcmpW(szValue, L"author")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Addauthors(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"title")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Settitle(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"journal")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setjournal(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"volume")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setvolume(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"month")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setmonth(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"year")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setyear(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"cdrom")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setcdrom(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"ee")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setee(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"book")) {
				// 跳过book
				do {
					pReader->Read(&nodeType);
					pReader->GetLocalName(&szValue, NULL);
				} while (nodeType != XmlNodeType_EndElement || lstrcmpW(szValue, L"book"));
			}
		}
	}

	return 0;
}
