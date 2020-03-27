#include "pch.h"
#include "xmlhelper.h"
#include "error.h"
#include "CommUtils.h"
/*
��xml, ����Ȩ��
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

	// ��ʼ���ṹ

	ImportData.f3_pFrequencyRanking = new FrequencyRanking(ignoresWords());

	while (lstrcmpW(L"dblp", localName) || nodeType != XmlNodeType_Element) {
		pReader->Read(&nodeType);
		pReader->GetLocalName(&localName, NULL);
	}

	while (S_OK == (hr = pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			pReader->GetLocalName(&localName, NULL);

			// ֻ����article
			if (lstrcmpW(L"article", localName)) continue;  // 154.829s

			// �����ǽ���һ��section
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

			// TODO: ʹ��temp
			ImportData.f3_pFrequencyRanking->Insert(temp);
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

