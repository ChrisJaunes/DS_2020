#include "pch.h"
#include "xmlhelper.h"
#include "error.h"
#include "CommUtils.h"
XMLParser::XMLParser()
{
	parseInfo = new std::vector<MYSTR>;
	parseInfo->push_back(MYSTR(L"article"));

}
XMLParser::XMLParser(DWORD flag)
{
	parseInfo = new std::vector<MYSTR>;
	if (flag & article) {
		parseInfo->push_back(MYSTR(L"article"));
	}
	if (flag & book) {
		parseInfo->push_back(MYSTR(L"book"));
	}
	if (flag & incollection) {
		parseInfo->push_back(MYSTR(L"incollection"));
	}
	if (flag & inproceedings) {
		parseInfo->push_back(MYSTR(L"inproceedings"));
	}
	if (flag & mastersthesis) {
		parseInfo->push_back(MYSTR(L"mastersthesis"));
	}
	if (flag & phdthesis) {
		parseInfo->push_back(MYSTR(L"phdthesis"));
	}
	if (flag & proceedings) {
		parseInfo->push_back(MYSTR(L"proceedings"));
	}
	if (flag & www) {
		parseInfo->push_back(MYSTR(L"www"));
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
			vector<MYSTR>::iterator ret;
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

OPRESULT XMLParser::ParseAll(ISolver *psolver) {
	HRESULT hr;
	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;

	// 初始化结构
	psolver->InitMemory();

	//while (lstrcmpW(L"dblp", localName) || nodeType != XmlNodeType_Element) {
	//	pReader->Read(&nodeType);
	//	pReader->GetLocalName(&localName, NULL);
	//}

	while (S_OK == (hr = pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			pReader->GetLocalName(&localName, NULL);

			// 解析类型, 用这个来限定解析对象
			vector<MYSTR>::iterator ret;
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

			// 假如没有title的, 将会被忽略
			if (temp.GetProperty(L"title").size()) {
				psolver->InsertObject(temp);
			}
		}
	}
	return 0;
}

wchar_t* XMLMarshal::Marshal(Info inobj)
{
	CComPtr<IStream> pStream;
	CComPtr<IXmlWriter> pWriter;
	::CreateXmlWriter(__uuidof(IXmlWriter),
		reinterpret_cast<void**>(&pWriter),
		0);
	::CreateStreamOnHGlobal(0, TRUE, &pStream);
	CComPtr<IXmlWriterOutput> pOutput;
	CreateXmlWriterOutputWithEncodingName(pStream, nullptr, L"utf-16", &pOutput);
	pWriter->SetOutput(pOutput);
	//pWriter->SetProperty(XmlWriterProperty_Indent, TRUE);

	MYSTR clsid = inobj.GetClsid();
	pWriter->WriteStartElement(0, clsid, 0);
	std::map<MYSTR, std::vector<MYSTR>> props = inobj.GetProperties();
	for (auto i : props) {
		for (auto j : i.second) {
			pWriter->WriteStartElement(0, i.first, 0);
			pWriter->WriteString(j);
			pWriter->WriteEndElement();
		}
	}
	pWriter->WriteEndElement();
	pWriter->Flush();
	STATSTG result;
	pStream->Stat(&result,STATFLAG_DEFAULT);
	ULONG readret;
	WCHAR* pv = new WCHAR[result.cbSize.QuadPart + 1]{0};
	LARGE_INTEGER move;
	move.QuadPart = 0;
	pStream->Seek(move, STREAM_SEEK_SET, NULL);
	pStream->Read(pv, result.cbSize.QuadPart, &readret);
	return pv;
}

Info XMLMarshal::Unmarshal(MYSTR xmlcode, DWORD flag)
{
	std::vector<MYSTR> *parseInfo = new std::vector<MYSTR>;
	if (flag & article) {
		parseInfo->push_back(MYSTR(L"article"));
	}
	if (flag & book) {
		parseInfo->push_back(MYSTR(L"book"));
	}
	if (flag & incollection) {
		parseInfo->push_back(MYSTR(L"incollection"));
	}
	if (flag & inproceedings) {
		parseInfo->push_back(MYSTR(L"inproceedings"));
	}
	if (flag & mastersthesis) {
		parseInfo->push_back(MYSTR(L"mastersthesis"));
	}
	if (flag & phdthesis) {
		parseInfo->push_back(MYSTR(L"phdthesis"));
	}
	if (flag & proceedings) {
		parseInfo->push_back(MYSTR(L"proceedings"));
	}
	if (flag & www) {
		parseInfo->push_back(MYSTR(L"www"));
	}

	CComPtr<IStream> pStream;
	CComPtr<IXmlReader> m_pReader;
	CComPtr<IXmlReaderInput> pInput;
	HRESULT hr=::CreateStreamOnHGlobal(0, TRUE, &pStream);
	CreateXmlReaderInputWithEncodingName(pStream, nullptr, L"utf-16", false, nullptr, &pInput);

	ULONG written;
	wchar_t* pv = (wchar_t*)xmlcode;
	pStream->Write(pv, 2 * wcslen(pv) + 1, &written);
	LARGE_INTEGER move;
	move.QuadPart = 0;
	pStream->Seek(move, STREAM_SEEK_SET, NULL);

	CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pReader, NULL);
	m_pReader->SetInput(pStream);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);

	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;
	Info temp;

	while (S_OK == (hr = m_pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			m_pReader->GetLocalName(&localName, NULL);

			// 解析类型
			vector<MYSTR>::iterator ret;
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
				break;
			}
		}
	}
	delete parseInfo;
	return temp;
}
