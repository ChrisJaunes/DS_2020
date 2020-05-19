#include "test_DBLP.h"
#include "config.h"
#include "xmlhelper.h"
#include "CommUtils.h"
namespace FST {
	test_xml::test_xml(const DWORD types)
	{
		MyLog::d("\t test : ");
		if (types & XMLPARSETYPE_article) printf("article;");
		if (types & XMLPARSETYPE_book)    printf("book;");
		if (types & XMLPARSETYPE_incollection) printf("incollection");
		if (types & XMLPARSETYPE_inproceedings) printf("inproceedings");
		if (types & XMLPARSETYPE_mastersthesis) printf("mastersthesis");
		if (types & XMLPARSETYPE_phdthesis) printf("phdthesis");
		if (types & XMLPARSETYPE_proceedings) printf("proceedings");
		if (types & XMLPARSETYPE_www) printf("www");
		MyLog::d("\n");
		info_cnt = 0;
		XMLParser* pParser = new XMLParser(types);
		f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author, FILE_Status::EXIST);
		pParser->ParseFile(DS_DBLP, this);
		delete pParser;
	}
	test_xml::~test_xml() {
		MyLog::d("\t count %d:\n", info_cnt);
		delete f;
	}
	void test_xml::InitMemory()
	{
	}

	void test_xml::InsertObject(Info& _info)
	{
		++info_cnt;
		auto title = _info.GetProperty(L"title").at(0);
		printf("%d\n", info_cnt);
		//if (info_cnt % 23790 == 0) {
		//	wprintf(L"%S", (const wchar_t*)title);
		//	wprintf(L"%S", (const wchar_t*)_info.serialize());
		//}
		auto jt = f->getInfoByTitle(title);
		if (jt.first == false) {
			MyLog::ew(L"info:%S ", title);
		}

		auto authors = _info.GetProperty(L"author");
		for (auto &it : authors) {
			auto jt = f->getAuthorByName(it);
			if (jt.first == false) {
				MyLog::ew(L"info:%s, author: %S", _info.GetProperty(L"title").at(0), it);
			}
		}

	}

	static void test_xmlparse() {
		runBlock([&] {
			test_xml((DWORD)XMLPARSETYPE_alltypes);
			}, "test xmlparse");
	};

	static void test_bptfile() {
		//DblpBptMs* f = new DblpBptMs(DS_DBLP_Info, DS_DBLP_Author);

	}
	void test_DBLP(DWORD flag) {
		MyLog::d("start test flag\n");
		if (flag & 1) test_xmlparse();
		if (flag & 2) test_bptfile();
	}
}