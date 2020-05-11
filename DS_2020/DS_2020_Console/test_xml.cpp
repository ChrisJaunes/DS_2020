#include "test_xml.h"
#include "config.h"
#include "xmlhelper.h"
#include "CommUtils.h"
namespace FST {
	test_xml::test_xml(const DWORD types)
	{
		printf("\t test : ");
		if (types & XMLPARSETYPE_article) printf("article;");
		if (types & XMLPARSETYPE_book)    printf("book;");
		if (types & XMLPARSETYPE_incollection) printf("incollection");
		if (types & XMLPARSETYPE_inproceedings) printf("inproceedings");
		if (types & XMLPARSETYPE_mastersthesis) printf("mastersthesis");
		if (types & XMLPARSETYPE_phdthesis) printf("phdthesis");
		if (types & XMLPARSETYPE_proceedings) printf("proceedings");
		if (types & XMLPARSETYPE_www) printf("www");
		puts("");
		info_cnt = 0;
		XMLParser* pParser = new XMLParser(types);
		DblpBptMs* f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author);
		pParser->ParseFile(DS_DBLP, this);
		delete f;
		delete pParser;
	}
	test_xml::~test_xml() {
		printf("\t count %d:\n", info_cnt);
	}
	void test_xml::InitMemory()
	{
	}

	void test_xml::InsertObject(Info& _info)
	{
#ifdef TEST_DEBUG
		++info_cnt;
#endif
		auto authors = _info.GetProperty(L"author");
		for (auto it : authors) {
		}
	}

	void test_xmlparse() {
		runBlock([&] {
			test_xml((DWORD)XMLPARSETYPE_article);
			}, "test");
	};
}