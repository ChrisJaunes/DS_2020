#include "pch.h"
#include "config.h"
#include "BPlusTree.h"
#include "BPlusTreeUtils.h"
namespace FST {
	key_t getKey()
	{
		key_t res;
		for (int i = 0; i < 10; i++) res.a[i] = wchar_t(rand() % 96 + 32);
		res.a[11] = L'\0';
		return res;
	}
	void test_BPT0() {
		FILE* file;

		file = _wfopen(DS_BPT_TEST_DB, L"wb+");

		int sz; wchar_t a_out[100] = L"��Ϊ���Ĳ���,Hello World test,{!@#$%^&*()<>?|[123456789]}";
		sz = sizeof(a_out);
		BPlusTreeUtils::db_write(file, 0, a_out, sizeof(a_out), 1);

		wchar_t a_in[100];
		sz = sizeof(a_in);
		BPlusTreeUtils::db_read(file, 0, a_in, sizeof(a_in), 1);

		assert(wcscmp(a_in, a_out) == 0);

		//key_t b_in = { L"hhh, �������ݽṹ����ҵ��key_t���Ͳ���, 987654321��@#��%����&*����" };
		key_t b_in = { L"hhh, �������ݽṹ����&*����" };
		BPlusTreeUtils::db_write(file, 200, &b_in, sizeof(b_in), 1);

		key_t b_out;
		BPlusTreeUtils::db_read(file, 200, &b_out, sizeof(b_out), 1);

		assert(b_in == b_out);

		char c[100];
		memset(c, 'A', sizeof(c));
		BPlusTreeUtils::db_write(file, 50, c, sizeof(c), 1);
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
	void test_BPT1() {
		BPlusTree<key_t>* tmp = new BPlusTree<key_t>(DS_BPT_TEST_DB);
		BPTset<key_t>(tmp, key_t{ L"hello, ����һ������" }, L"B+ tree ����!��");
		BPTset<key_t>(tmp, key_t{ L"1. ���Ĳ���" }, L"�����ֵ������");
		BPTset<key_t>(tmp, key_t{ L"2. ASCII" }, L"ASCII is Ok.");
		BPTset<key_t>(tmp, key_t{ L"3. symbol" }, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.");
		BPTset<key_t>(tmp, key_t{ L"4. number" }, L"1234567890");
		BPTset<key_t>(tmp, key_t{ L"5. OK" }, L"OK");

		wchar_t* value = NULL;
		BPTget<key_t>(tmp, key_t{ L"hello, ����һ������" }, value);
		assert(wcscmp(value, L"B+ tree ����!��") == 0);  value = NULL;
		BPTget<key_t>(tmp, key_t{ L"1. ���Ĳ���" }, value);
		assert(wcscmp(value, L"�����ֵ������") == 0); value = NULL;
		BPTget<key_t>(tmp, key_t{ L"2. ASCII" }, value);
		assert(wcscmp(value, L"ASCII is Ok.") == 0); value = NULL;
		BPTget<key_t>(tmp, key_t{ L"3. symbol" }, value);
		assert(wcscmp(value, L"~!@#$%^&*()_+`-={}[]|\\:;<>?,.") == 0); value = NULL;
		BPTget<key_t>(tmp, key_t{ L"4. number" }, value);
		assert(wcscmp(value, L"1234567890") == 0); value = NULL;
		BPTget<key_t>(tmp, key_t{ L"5. OK" }, value);
		assert(wcscmp(value, L"OK") == 0); value = NULL;
		delete tmp;
		tmp = new BPlusTree<key_t>(DS_BPT_TEST_DB, 1);
		BPTget<key_t>(tmp, key_t{ L"hello, ����һ������" }, value);
		assert(wcscmp(value, L"B+ tree ����!��") == 0);  value = NULL;
		BPTget<key_t>(tmp, key_t{ L"1. ���Ĳ���" }, value);
		assert(wcscmp(value, L"�����ֵ������") == 0); value = NULL;
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

	static void test_BP_data(std::wstring file_in, int n) {
		FILE* _in = _wfopen(file_in.c_str(), L"w");
		std::vector<int> vec(5, 0);
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
	static void test_BP_std(std::wstring file_in, std::wstring file_ans) {
		FILE* _in = _wfopen(file_in.c_str(), L"r");
		FILE* _ans = _wfopen(file_ans.c_str(), L"w");
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
	static void test_BP_me(std::wstring file_in, std::wstring file_out) {
		BPlusTree<int>* tmp = new BPlusTree<int>(DS_BPT_TEST_DB);
		FILE* _in = _wfopen(file_in.c_str(), L"r");
		FILE* _out = _wfopen(file_out.c_str(), L"w");
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
	void test_BPT2(int n) {
		srand((unsigned int) time(0));
		std::wstring file_als = std::wstring(DS_BPT_TEST_ALS);
		FILE* _als = _wfopen(file_als.c_str(), L"a");
		fprintf(_als, "===============================\nnew n = %d\n", n);
		std::wstring file_in = std::wstring(DS_BPT_TEST_IN);
		std::wstring file_out = std::wstring(DS_BPT_TEST_OUT);
		std::wstring file_ans = std::wstring(DS_BPT_TEST_ANS);
		test_BP_data(file_in, n);

		int begin_time = clock();
		test_BP_std(file_in, file_ans);
		fprintf(_als, "\n  std, time: %d\n", clock() - begin_time);

		begin_time = clock();
		BPlusTreeUtils::bptcntR = BPlusTreeUtils::bptcntW = 0;
		test_BP_me(file_in, file_out);
		fprintf(_als, "  me, time: %d; IO(R): %.6lf ; IO(W): %.6lf\n", clock() - begin_time, BPlusTreeUtils::bptcntR, BPlusTreeUtils::bptcntW);
		fprintf(_als, "Result: \n");
		fclose(_als);
		system("..\\DS_2020_Test\\test_file\\check.bat >> ..\\DS_2020_Test\\test.als");
	}
	void test_BPT3() {

	}
	void test_BPT() {
		test_BPT0();
		test_BPT1();
		test_BPT2(10000);
		test_BPT2(10000);
		test_BPT2(10000);
		test_BPT2(10000);
		test_BPT2(100000);
		test_BPT2(100000);
		test_BPT2(1000000);
		test_BPT3();
	}
}