#include "pch.h"
#include "xmlhelper.h"
#include "error.h"
#include "CommUtils.h"
XMLParser::XMLParser()
{
	parseInfo = new std::vector<MYSTR>;
	parseInfo->push_back(MYSTR(L"article"));
}
XMLParser::XMLParser(const DWORD flag)
{
	parseInfo = new std::vector<MYSTR>;
	if (flag & XMLPARSETYPE_article) {
		parseInfo->push_back(MYSTR(L"article"));
	}
	if (flag & XMLPARSETYPE_book) {
		parseInfo->push_back(MYSTR(L"book"));
	}
	if (flag & XMLPARSETYPE_incollection) {
		parseInfo->push_back(MYSTR(L"incollection"));
	}
	if (flag & XMLPARSETYPE_inproceedings) {
		parseInfo->push_back(MYSTR(L"inproceedings"));
	}
	if (flag & XMLPARSETYPE_mastersthesis) {
		parseInfo->push_back(MYSTR(L"mastersthesis"));
	}
	if (flag & XMLPARSETYPE_phdthesis) {
		parseInfo->push_back(MYSTR(L"phdthesis"));
	}
	if (flag & XMLPARSETYPE_proceedings) {
		parseInfo->push_back(MYSTR(L"proceedings"));
	}
	if (flag & XMLPARSETYPE_www) {
		parseInfo->push_back(MYSTR(L"www"));
	}
}
XMLParser::~XMLParser()
{
	delete parseInfo;
}
/*Info XMLParser::ParseSingle(LPCWSTR filename,size_t position)
{
	CComPtr<IStream> m_pFileStream;
	CComPtr<IXmlReader> m_pReader;
	LARGE_INTEGER pos;
	pos.QuadPart = position;

	HRESULT hr=SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&m_pFileStream);
	if (FAILED(hr)) {
		throw L"Invalid filename";
		return Info();
	}
	m_pFileStream->Seek(pos,STREAM_SEEK_SET, NULL);

	CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pReader, NULL);
	m_pReader->SetInput(m_pFileStream);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);

	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;
	Info temp;

	while (S_OK == (hr = m_pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			m_pReader->GetLocalName(&localName, NULL);

			// 解析类型
			std::vector<MYSTR>::iterator ret;
			ret = std::find(parseInfo->begin(), parseInfo->end(), MYSTR(localName));
			if (ret == parseInfo->end()) {
				continue;
			}

			// 看成是进入一个section
			curSection = localName;
			temp.SetClsid(MYSTR(curSection));

			if (S_OK == m_pReader->MoveToFirstAttribute()) {
				m_pReader->GetLocalName(&localName, NULL);
				m_pReader->GetValue(&szValue, NULL);
				temp.AddProperty(MYSTR(localName), MYSTR(szValue));
				while (S_OK == (hr = m_pReader->MoveToNextAttribute())) {
					m_pReader->GetLocalName(&localName, NULL);
					m_pReader->GetValue(&szValue, NULL);
					temp.AddProperty(MYSTR(localName), MYSTR(szValue));
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
					temp.AddProperty(MYSTR(localName), MYSTR(szValue));
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
}*/
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
	OPRESULT hr;
	pFileStream = NULL;
	hr = SHCreateStreamOnFile(
		filename,
		STGM_READ,
		&pFileStream);
	if (FAILED(hr))return hr;
	LARGE_INTEGER move;
	move.QuadPart = 0;
	pFileStream->Seek(move, STREAM_SEEK_SET, NULL);

	CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL);
	pReader->SetInput(pFileStream);
	pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);
	return 0;
}

OPRESULT XMLParser::ParseAll(ISolver*psolver) {
	HRESULT hr;
	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;

	//while (lstrcmpW(L"dblp", localName) || nodeType != XmlNodeType_Element) {
	//	pReader->Read(&nodeType);
	//	pReader->GetLocalName(&localName, NULL);
	//}

	while (S_OK == (hr = pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			pReader->GetLocalName(&localName, NULL);

			// 解析类型, 用这个来限定解析对象
			std::vector<MYSTR>::iterator ret;
			ret = std::find(parseInfo->begin(), parseInfo->end(), MYSTR(localName));
			if (ret == parseInfo->end()) {
				continue;
			}

			// 看成是进入一个section
			curSection = localName;
			Info temp;
			temp.SetClsid(MYSTR(curSection));

			pReader->MoveToFirstAttribute();
			pReader->GetLocalName(&localName, NULL);
			pReader->GetValue(&szValue, NULL);
			temp.AddProperty(MYSTR(localName), MYSTR(szValue));
			while (S_OK == (hr = pReader->MoveToNextAttribute())) {
				pReader->GetLocalName(&localName, NULL);
				pReader->GetValue(&szValue, NULL);
				temp.AddProperty(MYSTR(localName), MYSTR(szValue));
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
					temp.AddProperty(MYSTR(localName), MYSTR(szValue));
					while (nodeType != XmlNodeType_EndElement) {
						pReader->Read(&nodeType);
					}
				}
			}
			// 假如没有title的,将会被忽略
			if (temp.GetProperty(L"title").size()) {
				psolver->InsertObject(temp);
			}
		}
	}
	return 0;
}