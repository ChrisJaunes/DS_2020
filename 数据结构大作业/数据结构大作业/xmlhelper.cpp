#include "pch.h"
#include "xmlhelper.h"
#include "error.h"
#include "CommUtils.h"
XMLParser::XMLParser()
{
	parseInfo = new std::vector<STR>;
	parseInfo->push_back(STR(L"article"));

}
XMLParser::XMLParser(DWORD flag)
{
	parseInfo = new std::vector<STR>;
	if (flag & article) {
		parseInfo->push_back(STR(L"article"));
	}
	if (flag & book) {
		parseInfo->push_back(STR(L"book"));
	}
	if (flag & incollection) {
		parseInfo->push_back(STR(L"incollection"));
	}
	if (flag & inproceedings) {
		parseInfo->push_back(STR(L"inproceedings"));
	}
	if (flag & mastersthesis) {
		parseInfo->push_back(STR(L"mastersthesis"));
	}
	if (flag & phdthesis) {
		parseInfo->push_back(STR(L"phdthesis"));
	}
	if (flag & proceedings) {
		parseInfo->push_back(STR(L"proceedings"));
	}
	if (flag & www) {
		parseInfo->push_back(STR(L"www"));
	}
}
XMLParser::~XMLParser()
{
	delete parseInfo;
}
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

OPRESULT XMLParser::ParseAll(ISolver *psolver) {
	HRESULT hr;
	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;

	// 初始化结构
	psolver->InitMemory();

	while (lstrcmpW(L"dblp", localName) || nodeType != XmlNodeType_Element) {
		pReader->Read(&nodeType);
		pReader->GetLocalName(&localName, NULL);
	}

	while (S_OK == (hr = pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			pReader->GetLocalName(&localName, NULL);

			// 解析类型
			vector<STR>::iterator ret;
			ret = std::find(parseInfo->begin(), parseInfo->end(), STR(localName));
			if (ret == parseInfo->end()) {
				continue;
			}

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
				pReader->GetLocalName(&localName, NULL);
				if (nodeType == XmlNodeType_Element) {
					if (!lstrcmpW(localName, curSection))
						break;
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

			// 假如没有title的, 将会被忽略
			if (temp.GetProperty(L"title").size()) {
				// TODO: 使用temp
				psolver->InsertObject(temp);
			}
		}
	}
	return 0;
}

DWORD WINAPI ImportDataWrapper(LPCWSTR filename, ISolver *psolver) {
	XMLParser parser;
	OPRESULT hr = parser.OpenFile(filename);
	if (FAILED(hr)) { ImportData.isDone = false; return 0; }
	// 这里将写入到ImportData中
	parser.ParseAll(psolver);
	ImportData.isDone = true;
	return 0;
}

DWORD WINAPI ImportDataWrapperEx(LPCWSTR filename, ISolver* psolver, DWORD parseInfo) {
	XMLParser parser(parseInfo);
	OPRESULT hr = parser.OpenFile(filename);
	if (FAILED(hr)) { ImportData.isDone = false; return 0; }
	parser.ParseAll(psolver);
	ImportData.isDone = true;
	return 0;
}

