#include <QString>
#include "pch.h"
#include "test.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"

key_t getKey()
{
	key_t res;
	for (int i = 0; i < 10; i++) res.a[i] = wchar_t(rand() % 96 + 32);
	res.a[11] = L'\0';
	return res;
}

template <typename KEY_T>
int BPTset(BPlusTree<KEY_T>* bpt, KEY_T key, const wchar_t* value) {
	size_t sz = bpt->data_block_size - sizeof(OFF_T) * 2;
	size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
	size_t _value_sz = (value_sz + sz - 1) / sz * sz;
	void* _value = malloc(_value_sz);
	memset(_value, 0, _value_sz);
	memcpy(_value, value, value_sz);
	bpt->insert(key, _value, _value_sz);
	free(_value);
	return 0;
}
template <typename KEY_T>  
int BPTget(BPlusTree<KEY_T>* bpt, KEY_T key, wchar_t*& value) {
	assert(value == NULL);
	void* _value = NULL;
	size_t _value_sz;
	if (bpt->search(key, _value, _value_sz) == BPLUSTRE_FAILED) return BPLUSTRE_FAILED;
	size_t value_sz = _value_sz / sizeof(wchar_t);
	value = new wchar_t[value_sz];
	memset(value, 0, _value_sz);
	wcscat_s(value, value_sz, (wchar_t*)_value);
	free(_value);
	return BPLUSTRE_SUCCESS;
}
void test_BPT0() {
	BPlusTree<key_t>* tmp = new BPlusTree<key_t>(DS_DATABASE_TEST);
	BPTset<key_t>(tmp, key_t{ L"hello, 这是一个测试" }, L"B+ tree 测试!！");
	BPTset<key_t>(tmp, key_t{ L"1. 中文测试" }, L"插入的值是中文");
	BPTset<key_t>(tmp, key_t{ L"2. ASCII" }, L"ASCII is Ok.");
	BPTset<key_t>(tmp, key_t{ L"3. symbol" }, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.");
	BPTset<key_t>(tmp, key_t{ L"4. number" }, L"1234567890");
	BPTset<key_t>(tmp, key_t{ L"5. OK" }, L"OK");

	wchar_t* value = NULL;
	BPTget<key_t>(tmp, key_t{ L"hello, 这是一个测试" }, value);
	assert(wcscmp(value, L"B+ tree 测试!！") == 0);  value = NULL;
	BPTget<key_t>(tmp, key_t{ L"1. 中文测试" }, value);
	assert(wcscmp(value, L"插入的值是中文") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"2. ASCII" }, value);
	assert(wcscmp(value, L"ASCII is Ok.") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"3. symbol" }, value);
	assert(wcscmp(value, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"4. number" }, value);
	assert(wcscmp(value, L"1234567890") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"5. OK" }, value);
	assert(wcscmp(value, L"OK") == 0); value = NULL;
	delete tmp;
	tmp = new BPlusTree<key_t>(DS_DATABASE_TEST, 1);
	BPTget<key_t>(tmp, key_t{ L"hello, 这是一个测试" }, value);
	assert(wcscmp(value, L"B+ tree 测试!！") == 0);  value = NULL;
	BPTget<key_t>(tmp, key_t{ L"1. 中文测试" }, value);
	assert(wcscmp(value, L"插入的值是中文") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"2. ASCII" }, value);
	assert(wcscmp(value, L"ASCII is Ok.") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"3. symbol" }, value);
	assert(wcscmp(value, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"4. number" }, value);
	assert(wcscmp(value, L"1234567890") == 0); value = NULL;
	BPTget<key_t>(tmp, key_t{ L"5. OK" }, value);
	assert(wcscmp(value, L"OK") == 0); value = NULL;
	delete tmp;
}

static void test_BP_data(QString file_in, int n) {
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"w");
	std::vector<int> vec(5, 0);
	int key, value;
	while (n--) {
		int type = rand() % 2;
		fprintf(_in, "%d ", type);
		if (type == 0) {
			int a = rand(), b = rand();
			fprintf(_in, "%d %d\n", a, b);
			vec.push_back(a);
		}
		else if (type == 1) {
			int a = vec[rand() % vec.size()];
			fprintf(_in, " %d\n", a);
		}
	}
	fclose(_in);
}
static void test_BP_std(QString file_in, QString file_ans) {
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"r");
	FILE* _ans = _wfopen(file_ans.toStdWString().c_str(), L"w");
	std::map<int, int> mp;
	for (int type; ~fscanf(_in, "%d", &type);) {
		int a, b;
		if (type == 0) {
			fscanf(_in, "%d %d", &a, &b);
			mp[a] = b;
		}
		else {
			fscanf(_in, "%d", &a);
			fprintf(_ans, "%d\n", mp[a]);
		}
	}
	fclose(_in);
	fclose(_ans);
}
static void test_BP_me(QString file_in, QString file_out) {
	BPlusTree<int>* tmp = new BPlusTree<int>(DS_DATABASE_TEST);
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"r");
	FILE* _out = _wfopen(file_out.toStdWString().c_str(), L"w");
	int cnt = 0;
	wchar_t* value = new wchar_t[5];
	for (int type; ~fscanf(_in, "%d", &type);) {
		int a, b;
		if (type == 0) {
			fscanf(_in, "%d %d", &a, &b);
			memset(value, 0, 10);
			memmove(value, &b, sizeof(b));
			BPTset<int>(tmp, a, value);
		}
		else {
			int a;
			fscanf(_in, "%d", &a);
			void* res = NULL; size_t res_sz = 0;
			if (tmp->search(a, res, res_sz) == BPLUSTRE_FAILED) {
				fprintf(_out, "0\n");
			}
			else {
				fprintf(_out, "%d\n", *(int*)res);
			}
			free(res);
		}
	}
	delete tmp;
	fclose(_in);
	fclose(_out);

}
void test_BPT1(int n) {
	srand(time(0));
	QString file_als = QString("DS_database/test.als");
	FILE* _als = _wfopen(file_als.toStdWString().c_str(), L"a");
	fprintf(_als, "===============================\nnew n = %d\n", n);
	for (int i = 1; i <= 10; i++) {
		QString file_in = QString("DS_database/db/test%1.in").arg(QString::number(i));
		QString file_out = QString("DS_database/db/test%1.out").arg(QString::number(i));
		QString file_ans = QString("DS_database/db/test%1.ans").arg(QString::number(i));
		test_BP_data(file_in, n);

		int begin_time = clock();
		test_BP_std(file_in, file_ans);
		fprintf(_als, "\n%dth, std, time: %d\n", i, clock() - begin_time);
		
		begin_time = clock();
		test_BP_me(file_in, file_out);
		fprintf(_als, "%dth, me, time: %d\n", i, clock() - begin_time);
	}
	fprintf(_als, "Result: \n");
	fclose(_als);
	system("DS_database\\db\\check.bat >> DS_database/test.als");
}
void test_BPT2() {

}
void test_BPT() {
	test_BPT0();
	test_BPT1(1000);
	test_BPT1(10000);
	test_BPT1(10000);
	test_BPT1(10000);
	test_BPT1(100000);
	test_BPT1(100000);
	test_BPT1(100000);
	//test_BPT1(1000000);
	test_BPT2();
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
}*/
/*
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
}
int test_BPT1() {
	srand(time(0));
	int begin_time = clock();
	BPlusTree *tmp = new BPlusTree(DS_DATABASE_TEST, 0);
	std::map<key_t, key_t> mp;
	for (int i = 1; i <= 2; i++) {
		int type = rand() & 1;
		key_t key = getKey();
		key_t value = getKey();
		mp[key] = value;
		BPTset(tmp, key, &value.a[0]);
	}
	int n = 100000;
	for (int i = 1; i <= n; i++) {
		int type = rand() & 1;
		if (type == 1) {
			key_t key = getKey();
			key_t value = getKey();
			mp[key] = value;
			BPTset(tmp, key, &value.a[0]);
		}
		else {
			int index = rand() % mp.size();
			auto it = mp.begin();
			for (; index--;) ++it;
			wchar_t* res = NULL;
			BPTget(tmp, it->first, res);
			assert(wcscmp(res, &(it->second.a[0])) == 0);
			delete[] res;
		}
	}
	FILE* file = fopen("DS_database/db/test.res", "at+");
	fprintf(file, "%d", clock() - begin_time);
	fclose(file);
	return 0;
}
static void test_BP_data(QString file_in, int n) {
	srand(time(0));
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"w");
	std::vector<int> vec(5, 3);
	int key, value;
	while (n--) {
		int type = rand() % 2;
		fprintf(_in, "%d ", type);
		if (type == 0) {
			int a = rand(),  b = rand();
			fprintf(_in, "%d %d\n", a, b);
			vec.push_back(a);
		}
		else if (type == 1) {
			int a = vec[rand() % vec.size()];
			fprintf(_in, " %d\n", a);
		}
	}
	fclose(_in);
}
static void test_BP_std(QString file_in, QString file_ans) {
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"r");
	FILE* _ans = _wfopen(file_ans.toStdWString().c_str(), L"w");
	std::map<int, int> mp;
	for (int type; ~fscanf(_in, "%d", &type);) {
		 int a, b;
		if (type == 0) {
			fscanf(_in, "%d %d", &a, &b);
			mp[a] = b;
		}
		else {
			fscanf(_in, "%d", &a);
			fprintf(_ans, "%d\n", mp[a]);
		}
	}
	fclose(_in);
	fclose(_ans);
}
static void test_BP_me(QString file_in, QString file_out, QString file_als) {
	BPlusTree BPT(DS_DATABASE_TEST, 0);
	FILE* _in = _wfopen(file_in.toStdWString().c_str(), L"r");
	FILE* _out = _wfopen(file_out.toStdWString().c_str(), L"w");
	key_t key, value;
	int cnt = 0;
	while (!feof(_in)) {
		int type;
		fscanf(_in, "%d", &type);
		int a, b;
		if (type == 0) {
			fscanf(_in, "%d %d", &a, &b);
			memset(key.a, 0, sizeof(key.a));
			memmove(&key.a[0], &a, sizeof(a));
			memset(&value.a[0], 0, sizeof(value.a));
			memmove(&value.a[0], &b, sizeof(b));
			//BPT.insert(key, &value.a[0]);
		}
		else {
			int a;
			fscanf(_in, "%d", &a);
			memset(key.a, 0, sizeof(key.a));
			memmove(&key.a[0], &a, sizeof(a));
			void* res = NULL; size_t res_sz = 0;
			if (BPT.search(key, res, res_sz) == BPLUSTRE_FAILED) {
				fprintf(_out, "0\n");
			}
			else {
				fprintf(_out, "%d\n", *(int*)res);
			}
			free(res);
		}
		f ((++cnt) % 1000000 == 0) {
			fprintf(_out, "%d\n", cnt);
			fflush(_out);
		}
	}
	fclose(_in);
	fclose(_out);
	
}
void test_BPT2() {
	srand(time(0));
	for (int i = 1; i <= 10; i++) {
		QString file_in = QString("DS_database/db/test%1.in").arg(QString::number(1));
		QString file_out = QString("DS_database/db/test%1.out").arg(QString::number(1));
		QString file_ans = QString("DS_database/db/test%1.ans").arg(QString::number(1));
		QString file_als = QString("DS_database/test.als");
		test_BP_data(file_in, 1000000);
		test_BP_std(file_in, file_ans);
		int begin_time = clock();
		test_BP_me(file_in, file_out, file_als);	
		FILE* _als = _wfopen(file_als.toStdWString().c_str(), L"a");
		fprintf(_als, "%dth, time: %d, res ok\n", i, clock() - begin_time); 
		system("fc DS_database/db/test1.out DS_database/db/test1.ans");
		system("pause");
		fclose(_als);
	}
}*/