#include "pch.h"
#include "article.h"
#include "xmlhelper.h"
#include "error.h"

/*
��xml, ����Ȩ��
*/
OPRESULT XMLParser::OpenFile(LPCWSTR filename) 
{
	IStream* pFileStream = NULL;
	OPRESULT hr = SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&pFileStream);
	if (FAILED(hr))return hr;
	hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL);
	if (FAILED(hr))return hr;
	pReader->SetInput(pFileStream);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);
	return 0;
}

/*
����dtd, ���뵽vector, ����ط�����ѡ�����b+��
*/
OPRESULT XMLParser::ParseArticlesToVector() 
{
	HRESULT hr;
	LPCWSTR szValue = NULL;
	while (S_OK == (hr = pReader->Read(&nodeType))) {
		pReader->GetLocalName(&szValue, NULL);
		if (nodeType == XmlNodeType_Element) {
			if (!lstrcmpW(szValue, L"article")) {
				// debug:
				if (GDEBUG && members.size()>0) {
					bstr_t tmp;
					members.at(members.size() - 1)->Gettitle(&tmp);
					std::wcout << tmp << std::endl;
					system("pause");
				}

				members.push_back(new Article);
				pReader->MoveToFirstAttribute();
				pReader->GetValue(&szValue, NULL);
				members.at(members.size() - 1)->Setmdate(szValue);

				pReader->MoveToNextAttribute();
				pReader->GetValue(&szValue, NULL); 
				members.at(members.size() - 1)->Setkey(szValue);

				// �����Իص�Ԫ��
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
				// ����book
				do {
					pReader->Read(&nodeType);
					pReader->GetLocalName(&szValue, NULL);
				} while (nodeType != XmlNodeType_EndElement || lstrcmpW(szValue, L"book"));
			}
		}
	}

	return 0;
}

/// ������е�����Ĵ��붼������
OPRESULT XMLParser::ParseAll () {
	HRESULT hr;
	LPCWSTR szValue = NULL;
	
	Article *temp=NULL;
	while (S_OK == (hr = pReader->Read(&nodeType))) {
		pReader->GetLocalName(&szValue, NULL);
		if (nodeType == XmlNodeType_Element) {
			if (!lstrcmpW(szValue, L"article")) {
				if (temp) {
					/// ����ط������һ��������Articleָ��temp, ���������԰����������뵽�������ݽṹ.



				}
				temp = new Article;
				pReader->MoveToFirstAttribute();
				pReader->GetValue(&szValue, NULL);
				temp->Setmdate(szValue);

				pReader->MoveToNextAttribute();
				pReader->GetValue(&szValue, NULL);
				temp->Setkey(szValue);

				// �����Իص�Ԫ��
				pReader->MoveToElement();
			}
			else if (!lstrcmpW(szValue, L"author")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Addauthors(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"title")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Settitle(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"journal")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setjournal(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"volume")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setvolume(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"month")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setmonth(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"year")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setyear(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"cdrom")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setcdrom(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"ee")) {
				pReader->Read(&nodeType);
				pReader->GetValue(&szValue, NULL);
				temp->Setee(szValue);
				pReader->Read(&nodeType);
			}
			else if (!lstrcmpW(szValue, L"book")) {
				// ����book
				do {
					pReader->Read(&nodeType);
					pReader->GetLocalName(&szValue, NULL);
				} while (nodeType != XmlNodeType_EndElement || lstrcmpW(szValue, L"book"));
			}
		}
	}
	return 0;
}


DWORD WINAPI ImportDataWrapper(LPCWSTR filename) {
	XMLParser parser;
	OPRESULT hr = parser.OpenFile(filename);
	if (FAILED(hr)) { ImportData.isDone = false; return 0; }
	// ���ｫд�뵽ImportData��
	parser.ParseAll();
	ImportData.isDone = true;
	return 0;
}

wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}