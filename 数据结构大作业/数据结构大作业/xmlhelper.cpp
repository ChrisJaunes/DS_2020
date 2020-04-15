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
Info XMLParser::ParseSingle(LPCWSTR filename,size_t position)
{
	CComPtr<IStream> m_pFileStream;
	CComPtr<IXmlReader> m_pReader;
	LARGE_INTEGER pos;
	pos.QuadPart = position;

	SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&m_pFileStream);
	m_pFileStream->Seek(pos,STREAM_SEEK_SET, NULL);

	CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pReader, NULL);
	m_pReader->SetInput(m_pFileStream);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);

	HRESULT hr;
	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;
	Info temp;

	while (S_OK == (hr = m_pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			m_pReader->GetLocalName(&localName, NULL);

			// 解析类型
			vector<STR>::iterator ret;
			ret = std::find(parseInfo->begin(), parseInfo->end(), STR(localName));
			if (ret == parseInfo->end()) {
				continue;
			}

			// 看成是进入一个section
			curSection = localName;
			temp.SetClsid(STR(curSection));

			if (S_OK == m_pReader->MoveToFirstAttribute()) {
				m_pReader->GetLocalName(&localName, NULL);
				m_pReader->GetValue(&szValue, NULL);
				temp.AddProperty(STR(localName), STR(szValue));
				while (S_OK == (hr = m_pReader->MoveToNextAttribute())) {
					m_pReader->GetLocalName(&localName, NULL);
					m_pReader->GetValue(&szValue, NULL);
					temp.AddProperty(STR(localName), STR(szValue));
				}
				m_pReader->MoveToElement();
			}

			while (lstrcmpW(localName, curSection) || nodeType != XmlNodeType_EndElement) {
				m_pReader->Read(&nodeType);
				m_pReader->GetLocalName(&localName, NULL);
				if (nodeType == XmlNodeType_Element) {
					if (!lstrcmpW(localName, curSection))
						break;
					while (nodeType != XmlNodeType_Text) {
						m_pReader->Read(&nodeType);
					}
					m_pReader->GetValue(&szValue, NULL);
					temp.AddProperty(STR(localName), STR(szValue));
					while (nodeType != XmlNodeType_EndElement) {
						m_pReader->Read(&nodeType);
					}
				}
			}

			// 假如没有title的, 将会被忽略
			if (temp.GetProperty(L"title").size()) {
				return temp;
			}
		}
	}


	return Info();
}
OPRESULT XMLParser::ParseFile(LPCWSTR filename, ISolver* pSolver)
{
	OPRESULT hr = OpenFile(filename);
	if (FAILED(hr)) { return hr; }
	hr = ParseAll(pSolver);
	return OPRESULT(hr);
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