#include "test.h"
#ifdef TEST_DEBUG
#include "qdebug.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

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
		FILE* file;

		file = _wfopen(L"DS_test/IO.dstest", L"rb+");
		
		int sz;wchar_t a_out[100] = L"作为中文测试,Hello World test,{!@#$%^&*()<>?|[123456789]}";
		sz = sizeof(a_out);
		BPlusTreeUtils::db_write(file, 0, a_out, sizeof(a_out), 1);

		wchar_t a_in[100];
		sz = sizeof(a_in);
		BPlusTreeUtils::db_read(file, 0, a_in, sizeof(a_in), 1);
		
		assert(wcscmp(a_in, a_out) == 0);

		key_t b_in = { L"hhh, 这是数据结构大作业的key_t类型测试, 987654321！@#￥%……&*（）" };
		BPlusTreeUtils::db_write(file, 200, &b_in, sizeof(b_in), 1);
		
		key_t b_out;
		BPlusTreeUtils::db_read(file, 200, &b_out, sizeof(b_out), 1);

		assert(b_in == b_out);

		char c[100];
		memset(c, 'A', sizeof(c));
		BPlusTreeUtils::db_write(file, 50, c, sizeof(c), 1);
	}
	bool test_BPlusTreeNode_check(BPlusTreeNode* a, BPlusTreeNode* b) {
		assert(a->type == b->type);
		assert(a->self == b->self);
		assert(a->parent == b->parent);
		assert(a->prev == b->prev);
		assert(a->next == b->next);
		assert(a->ch_cnt == b->ch_cnt);
		for (int i = 0; i < BPT_MAX_ENTRIES; i++) {
			assert(a->keys[i] == b->keys[i]);
			assert(a->chds[i] == b->chds[i]);
		}
		return true;
	}
	void test_BPlusTreeNode() {
		void* buf = malloc(6000);
		free(buf);
		FILE* file;
		file = _wfopen(L"DS_test/IO.dstest", L"rb+");
		fseek(file, 0, SEEK_END);
		int off = ftell(file);

		BPlusTreeNode a_out;
		a_out.type = 1;
		a_out.self = off_t(off);
		a_out.parent = off_t(0);
		a_out.prev = off_t(0);
		a_out.next = off_t(0);
		a_out.ch_cnt = 5;
		a_out.keys[0] = key_t{ L"ASCII test" };
		a_out.keys[1] = key_t{ L"中文测试" };
		a_out.keys[2] = key_t{ L"1234567890" };
		a_out.keys[3] = key_t{ L"!@#$%^&*()_+-=~`|[]{}:;\"'<>,.?'" };
 		a_out.chds[0] = 1;
		a_out.chds[1] = 10;
		a_out.chds[2] = 100;
		a_out.chds[3] = 1000;
		a_out.chds[4] = 10000;
		a_out.chds[5] = 10000000000;
		a_out.chds[6] = 8589934592;
		int sz = sizeof(a_out);
		a_out.toFileBlock(file, off, sizeof(a_out));
		BPlusTreeNode a_in;
		a_in.fromFileBlock(file, off, sizeof(a_in));
		assert(test_BPlusTreeNode_check(&a_in, &a_out));
		fclose(file);

		
	}
	void test_BPlusTree() {
		BPlusTree tmp(L"test", 0);
		tmp.insert(key_t{ L"hello, 这是一个测试" }, L"123456");
		tmp.insert(key_t{ L"1. 中文测试" }, L"插入的值是中文");
		tmp.insert(key_t{ L"2. ASCII" }, L"ASCII is Ok.");
		tmp.insert(key_t{ L"3. symbol" }, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.");
		tmp.insert(key_t{ L"4. number" }, L"1234567890");
		tmp.insert(key_t{ L"5. OK" }, L"OK");

		wchar_t *value = NULL;
		tmp.search(key_t{ L"hello, 这是一个测试" }, value);
		value = NULL;
		tmp.search(key_t{ L"1. 中文测试" }, value);
		value = NULL;
		tmp.search(key_t{ L"2. ASCII" }, value);
		value = NULL;
		tmp.search(key_t{ L"3. symbol" }, value);
		value = NULL;
		tmp.search(key_t{ L"4. number" }, value);
		value = NULL;
		tmp.search(key_t{ L"5. OK" }, value);
	}
	void tmain() {
		create_FST();
		test_BPIO();
		test_BPlusTreeNode();
		test_BPlusTree();
	}
}
#endif