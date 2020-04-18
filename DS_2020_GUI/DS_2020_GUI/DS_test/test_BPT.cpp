#include "pch.h"
#include "test.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"
#include "test_BPT.h"

key_t getKey()
{
	key_t res;
	for (int i = 0; i < 10; i++) res.a[i] = wchar_t(rand() % 96 + 32);
	res.a[11] = L'\0';
	return res;
}
/*void test_BPIO() {
	FILE* file;

	file = _wfopen(DS_DATABASE_IO, L"wb+");

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
	file = _wfopen(DS_DATABASE_IO, L"rb+");
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
}*/
void test_BPT0() {
	BPlusTree tmp(DS_DATABASE_TEST, 0);
	tmp.insert(key_t{ L"hello, 这是一个测试" }, L"123456");
	tmp.insert(key_t{ L"1. 中文测试" }, L"插入的值是中文");
	tmp.insert(key_t{ L"2. ASCII" }, L"ASCII is Ok.");
	tmp.insert(key_t{ L"3. symbol" }, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.");
	tmp.insert(key_t{ L"4. number" }, L"1234567890");
	tmp.insert(key_t{ L"5. OK" }, L"OK");

	wchar_t* value = NULL;
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

int test_BPT1() {
	srand(time(0));
	key_t ERKEY = key_t{ L"w\\AL,5,#5+x" };
	int begin_time = clock();
	BPlusTree BPT(DS_DATABASE_TEST, 0);
	std::map<key_t, key_t> mp;
	for (int i = 1; i <= 2; i++) {
		int type = rand() & 1;
		if (type == 1) {
			key_t key = getKey();
			key_t value = getKey();
			if (ERKEY == key) {
				int x = 0;
			}
			mp[key] = value;
			BPT.insert(key, &value.a[0]);
			for (int i = 0; i < 10; i++) if (BPT.used[i] != 0) {
				int x = 0;
			}

		}
	}
	int n = rand() % 100000;
	for (int i = 1; i <= n; i++) {
		int type = rand() & 1;
		if (type == 1) {
			key_t key = getKey();
			key_t value = getKey();
			if (ERKEY == key) {
				int x = 0;
			}
			mp[key] = value;
			BPT.insert(key, &value.a[0]);
		}
		else {
			int index = rand() % mp.size();
			auto it = mp.begin();
			for (; index--;) ++it;
			wchar_t* res = NULL;
			BPT.search(it->first, res);
			assert(wcscmp(res, &(it->second.a[0])) == 0);
			delete[] res;
		}
		for (int i = 0; i < 10; i++) if (BPT.used[i] != 0) {
			int x = 0;
		}
	}
	FILE* file = fopen("DS_database/db/test.res", "wt+");
	fprintf(file, "%d", clock() - begin_time);
	fclose(file);
	return 0;
}