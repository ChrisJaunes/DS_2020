#include "test.h"
#ifdef TEST_DEBUG
#include "qdebug.h"
#include "BPIO.h"
#include "BPlusTree.h"

namespace FST {

	Author  *AUTHORS[AUTHORSN];
	Info    *INFO[INFON];
	bstr_t getStr(char ch) {
		return bstr_t((QString(ch) + char(rand() % 26 + 'a') + char(rand() % 26 + 'A')).toStdWString().c_str());
	}
	void create_FST() {
		srand(20190319);
		AUTHORS[0] = new Author();
		std::vector<bstr_t> tmp;
		tmp.push_back(L"B01");
		tmp.push_back(L"B02");
		tmp.push_back(L"B03");
		tmp.push_back(L"数据结构");
		AUTHORS[0]->AddArticle(bstr_t(L"A1"), tmp);
		tmp.clear();
		tmp.push_back(L"B01");
		tmp.push_back(L"张三");
		tmp.push_back(L"C03");
		tmp.push_back(L"C04");
		AUTHORS[0]->AddArticle(bstr_t(L"A2"), tmp);
		tmp.clear();
		tmp.push_back(L"B01");
		tmp.push_back(L"张三");
		tmp.push_back(L"D03");
		tmp.push_back(L"D04");
		AUTHORS[0]->AddArticle(bstr_t(L"A3"), tmp);
		tmp.clear();
		tmp.push_back(L"B01");
		tmp.push_back(L"张三");
		tmp.push_back(L"D03");
		tmp.push_back(L"E04");
		AUTHORS[0]->AddArticle(bstr_t(L"A4"), tmp);

		for (int i = 1; i < AUTHORSN; i++) {
			AUTHORS[i] = new Author();
			for (int j = 0; j < 5; j++) {
				std::vector<bstr_t> tmp;
				bstr_t title = getStr('A');
				for (int k = rand() % 10; k >= 0; k--) {
					tmp.push_back(getStr('A'));
				}
				AUTHORS[i]->AddArticle(title, tmp);
			}
		}


		INFO[0] = new Info();
		INFO[0]->AddProperty("title", "abc");
		INFO[0]->AddProperty("date", "20200329");
		INFO[0]->AddProperty("author", "AA");
		INFO[0]->AddProperty("author", "张三");
		INFO[0]->AddProperty("author", "CC");
		for (int i = 1; i < INFON; i++) {
			INFO[i] = new Info();
			INFO[i]->AddProperty(L"title", getStr('T'));
			int tmp = rand() % 10;
			while (tmp--) {
				INFO[i]->AddProperty(getStr('I'), getStr('I'));
			}
		}
	}
	void test_BPIO() {
		key_t buf;
		FILE* in, * out;
		int sz;

		out = _wfopen(L"DS_test/IO1.txt", L"wb");
		wchar_t a[100] = L"作为中文测试,This_is_English,{!@#$%^&*()<>?|[123456789]}";
		sz = sizeof(a);
		BPIO::db_write(out, 0, a, sizeof(a), 1);

		in = _wfopen(L"DS_test/IO1.txt", L"rb");
		wchar_t b[100];
		sz = sizeof(b);
		BPIO::db_read(out, 0, &buf, sizeof(buf), 1);
		
		assert(wcscmp(a, b) == 0);
	}
	void tmain() {
		test_BPIO();
	}
}
#endif