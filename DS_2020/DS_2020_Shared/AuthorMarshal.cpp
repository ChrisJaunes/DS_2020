#include "pch.h"
#include "AuthorMarshal.h"
#include "xmlhelper.h"
#include "CommUtils.h"
/*
collaborators:
	����->Ȩ��
	����->����s

<AUTHOR name="xxx">
	<collaborators name="aaa">
		int
	</collaborators>
	...

	<title name="xxx">
		<person>xxx</person>
	</title>
	...

</AUTHOR>

<AUTHOR name="xxx">
<title name="">
	<people>xxx</people>
	...
<title>
...
</AUTHOR>
*/
#define W int

wchar_t* AuthorMarshal::Marshal(Author inobj)
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
	pWriter->WriteStartElement(0, L"AUTHOR", 0);
	pWriter->WriteAttributeString(0,L"name", 0, inobj.GetName().second);

	//std::vector<pair<W, STR> > w_collaborators =inobj.GetCollaborators().second;
	//for (auto i : w_collaborators) {
	//	pWriter->WriteStartElement(0, L"collaborators", 0);
	//	pWriter->WriteAttributeString(0, L"name", 0, i.second);
	//	pWriter->WriteString(STR(i.first));
	//	pWriter->WriteEndElement();
	//}
	std::map<MYSTR, std::vector<MYSTR> > articles = inobj.GetArticles().second;
	for (auto i : articles) {
		pWriter->WriteStartElement(0, L"title", 0);
		pWriter->WriteAttributeString(0, L"name", 0, i.first);
		for (auto j : i.second) {
			pWriter->WriteStartElement(0, L"people", 0);
			pWriter->WriteString(j);
			pWriter->WriteEndElement();
		}
		pWriter->WriteEndElement();
	}
	pWriter->WriteEndElement();
	pWriter->Flush();
	STATSTG result;
	pStream->Stat(&result, STATFLAG_DEFAULT);
	ULONG readret;
	WCHAR* pv = new WCHAR[result.cbSize.QuadPart + 1]{ 0 };
	LARGE_INTEGER move;
	move.QuadPart = 0;
	pStream->Seek(move, STREAM_SEEK_SET, NULL);
	pStream->Read(pv, result.cbSize.QuadPart, &readret);
	return pv;
}

Author AuthorMarshal::Unmarshal(MYSTR xmlcode)
{
	std::vector<MYSTR>* parseInfo = new std::vector<MYSTR>;
	parseInfo->push_back(L"AUTHOR");
	CComPtr<IStream> pStream;
	CComPtr<IXmlReader> m_pReader;
	CComPtr<IXmlReaderInput> pInput;
	HRESULT hr = ::CreateStreamOnHGlobal(0, TRUE, &pStream);
	CreateXmlReaderInputWithEncodingName(pStream, nullptr, L"utf-16", false, nullptr, &pInput);

	ULONG written;
	wchar_t* pv = (wchar_t*)xmlcode;
	pStream->Write(pv, 2*wcslen(pv)+1, &written);
	LARGE_INTEGER move;
	move.QuadPart = 0;
	pStream->Seek(move, STREAM_SEEK_SET, NULL);

	CreateXmlReader(__uuidof(IXmlReader), (void**)&m_pReader, NULL);
	m_pReader->SetInput(pInput);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, TRUE);

	LPCWSTR szValue = NULL, curSection = NULL, localName = NULL;
	XmlNodeType nodeType;
	Author temp;

	while (S_OK == (hr = m_pReader->Read(&nodeType))) {
		if (nodeType == XmlNodeType_Element) {
			m_pReader->GetLocalName(&localName, NULL);

			// ��������
			std::vector<MYSTR>::iterator ret;
			ret = std::find(parseInfo->begin(), parseInfo->end(), MYSTR(localName));
			if (ret == parseInfo->end()) {
				continue;
			}

			// �����ǽ���һ��section
			curSection = localName;
			if (S_OK != m_pReader->MoveToFirstAttribute()) { throw "unk error"; };
			m_pReader->GetValue(&szValue, NULL);
			temp.SetName(MYSTR(szValue));
			m_pReader->MoveToElement();
			
			while (lstrcmpW(localName, curSection) || nodeType != XmlNodeType_EndElement) {
				m_pReader->Read(&nodeType);
				m_pReader->GetLocalName(&localName, NULL);
				if (nodeType == XmlNodeType_Element) {
					if (!lstrcmpW(localName, curSection))
						break;

					if (nodeType == XmlNodeType_Element && !lstrcmpW(localName, L"title")) {
						std::vector<MYSTR> tmpPeople;
						MYSTR tmpTitle;
						if (S_OK != m_pReader->MoveToFirstAttribute()) { throw "unk error1"; };
						m_pReader->GetValue(&szValue, NULL);
						tmpTitle = szValue;
						m_pReader->MoveToElement();

						while (nodeType != XmlNodeType_EndElement || lstrcmpW(localName, L"title")) {
							m_pReader->Read(&nodeType);
							m_pReader->GetLocalName(&localName, NULL);

							if (nodeType== XmlNodeType_Element && !lstrcmpW(localName, L"people")) {
								while (nodeType != XmlNodeType_Text) {
									m_pReader->Read(&nodeType);
								}
								m_pReader->GetValue(&szValue, NULL);
								tmpPeople.push_back(szValue);
							}
						}
						temp.AddArticle(tmpTitle, tmpPeople);
					}
				}
			}

			break;
		}
	}
	delete parseInfo;
	return temp;
}
#undef W