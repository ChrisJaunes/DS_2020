#include "pch.h"
#include "xmlhelper.h"
#include "error.h"
_ImportData ImportData;

/*
打开xml, 设置权限
*/
OPRESULT XMLParser::OpenFile(LPCWSTR filename)
{
	pFileStream = NULL;
	SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&pFileStream);
	CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL);
	pReader->SetInput(pFileStream);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);
	return 0;
}

OPRESULT XMLParser::ParseAll() {
	HRESULT hr;
	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;

	// 初始化结构
	ImportData.f3_pFrequencyRanking = new FrequencyRanking;

	while (lstrcmpW(L"dblp", localName) || nodeType != XmlNodeType_Element) {
		pReader->Read(&nodeType);
		pReader->GetLocalName(&localName, NULL);
	}

	while (S_OK == (hr = pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			pReader->GetLocalName(&localName, NULL);

			// 只解析article
			if (lstrcmpW(L"article", localName)) continue;  // 154.829s

			// 看成是进入一个section
			curSection = localName;
			Info temp;
			temp.SetClsid(STR(curSection));

			pReader->MoveToFirstAttribute();
			pReader->GetLocalName(&localName, NULL);
			pReader->GetValue(&szValue, NULL);
			temp.AddProperty(STR(localName), STR(szValue));
			while (S_OK == (hr = pReader->MoveToNextAttribute())) {
				pReader->GetLocalName(&localName, NULL);
				pReader->GetValue(&szValue, NULL);
				temp.AddProperty(STR(localName), STR(szValue));
			}
			pReader->MoveToElement();

			while (lstrcmpW(localName, curSection) || nodeType != XmlNodeType_EndElement) {
				pReader->Read(&nodeType);
				if (nodeType == XmlNodeType_Element) {
					pReader->GetLocalName(&localName, NULL);
					while (nodeType != XmlNodeType_Text) {
						pReader->Read(&nodeType);
					}
					pReader->GetValue(&szValue, NULL);
					temp.AddProperty(STR(localName), STR(szValue));
					while (nodeType != XmlNodeType_EndElement) {
						pReader->Read(&nodeType);
					}
				}
			}

			// TODO: 使用temp
			ImportData.f3_pFrequencyRanking->Insert(temp);
		}
	}
	return 0;
}

DWORD WINAPI ImportDataWrapper(LPCWSTR filename) {
	XMLParser parser;
	OPRESULT hr = parser.OpenFile(filename);
	if (FAILED(hr)) { ImportData.isDone = false; return 0; }
	// 这里将写入到ImportData中
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

