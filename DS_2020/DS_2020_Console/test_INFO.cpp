#include "test_INFO.h"
#include "Info.h"

namespace FST {
	Info info_a;
	void test_Info_P(Info& a) {
		FILE* fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		_wsetlocale(0, L"chs");
		fwprintf(fd, L" \n====GetClsid====\n");
		auto clsid = a.GetClsid();
		fwprintf(fd, L"%s; ", (wchar_t*)clsid);

		fwprintf(fd, L" \n====GetProperties====\n");
		auto properties = a.GetProperties();
		for (auto& it : properties) {
			fwprintf(fd, L" %s: ", (wchar_t *)it.first);
			for(auto& jt : it.second)
				fwprintf(fd, L"%s; ", (wchar_t*)jt);
		}
		fclose(fd);
	}
	void test_INFO0() {
		FILE* fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		fwprintf(fd, L" \n====******====\n\n");
		fclose(fd);
		Info a;
		info_a.SetClsid(L"article");
		info_a.AddProperty(L"测试中文", MYSTR(L"这是一个中文测试"));
		info_a.AddProperty(L"test_ASCII", MYSTR(L"ASCII is OK"));
		info_a.AddProperty(L"number_0", MYSTR(L"1234567890"));
		info_a.AddProperty(L"symbol", MYSTR(L"~!@#$%^&*()_+=-`|\\{}[];:'\"<>?/.,"));
		info_a.AddProperty(L"测试", MYSTR("检查0是否有问题"));

		test_Info_P(info_a);

		fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		_wsetlocale(0, L"chs");
		wchar_t* ser = a.serialize();
		fwprintf(fd, L" \n====serialize====\n");
		fwprintf(fd, L"%s; ", (wchar_t*)ser);
		fclose(fd);

		Info b = a.deserialize(ser);
		test_Info_P(b);
		delete[] ser;
	}
	void test_INFO1() {
		FILE* fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		fwprintf(fd, L" \n====******====\n\n");
		fclose(fd);
		Info a;
		a.SetClsid(L"article");
		a.AddProperty(L"mdate", L"2020-03-12");
		a.AddProperty(L"key", L"tr/meltdown/s18");
		a.AddProperty(L"publtype", L"informal");
		a.AddProperty(L"author", L"Paul Kocher");
		a.AddProperty(L"author", L"Daniel Genkin");
		a.AddProperty(L"author", L"Daniel Gruss");
		a.AddProperty(L"author", L"Werner Haas");
		a.AddProperty(L"author", L"Mike Hamburg");
		a.AddProperty(L"author", L"Moritz Lipp");
		a.AddProperty(L"author", L"Stefan Mangard"); 
		a.AddProperty(L"author", L"Thomas Prescher");
		a.AddProperty(L"author", L"Michael Schwarz 0001");
		a.AddProperty(L"author", L"Yuval Yarom");
		a.AddProperty(L"title", L"Spectre Attacks : Exploiting Speculative Execution.");
		a.AddProperty(L"journal", L"meltdownattack.com");
		a.AddProperty(L"year", L"2018");
		a.AddProperty(L"ee", L" https ://spectreattack.com/spectre.pdf");

		test_Info_P(a);

		fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		_wsetlocale(0, L"chs");
		wchar_t* ser = a.serialize();
		fwprintf(fd, L" \n====serialize====\n");
		fwprintf(fd, L"%s; ", (wchar_t*)ser);
		fclose(fd);

		Info b = a.deserialize(ser);
		test_Info_P(b);
		delete[] ser;
	}
}