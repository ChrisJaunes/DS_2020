#include "test_AUTHOR.h"
#include "Author.h"

namespace FST {
	void test_Author_P(Author& a) {
		FILE* fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		_wsetlocale(0, L"chs");
		fwprintf(fd, L" \n====GetTitleOfArticles====\n");
		auto titles = a.GetTitleOfArticles();
		for (auto& it : titles.second) {
			fwprintf(fd, L"%s; ", (wchar_t*)it);
		}

		fwprintf(fd, L" \n====GetArticles====\n");
		auto articles = a.GetArticles();
		for (auto& it : articles.second) {
			fwprintf(fd, L" %s:", (wchar_t*)it.first);
			for (auto& jt : it.second) {
				fwprintf(fd, L"%s; ", (wchar_t*)jt);
			}
		}

		fwprintf(fd, L"\n===GetNumOfArticle===\n");
		fwprintf(fd, L"%d", a.GetNumOfArticle());

		fwprintf(fd, L" \n====GetCollaborators====\n");
		auto Collaborators = a.GetCollaborators();
		for (auto& it : Collaborators.second) {
			fwprintf(fd, L" %d: ", it.first);
			fwprintf(fd, L"%s; ", (wchar_t*)it.second);
		}
		fclose(fd);
	}
	void test_AUTHOR0() {
		FILE* fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		fwprintf(fd, L" \n====******====\n");
		fclose(fd);
		Author author_a;
		author_a.SetName(L"测试中文");
		author_a.AddArticle(L"测试中文", std::vector<MYSTR>(2, L"这是一个中文测试"));
		author_a.AddArticle(L"test_ASCII", std::vector<MYSTR>(3, L"ASCII is OK"));
		author_a.AddArticle(L"number_0", std::vector<MYSTR>(4, L"1234567890"));
		author_a.AddArticle(L"symbol", std::vector<MYSTR>(3, L"~!@#$%^&*()_+=-`|\\{}[];:'\"<>?/.,"));
		author_a.AddArticle(L"测试", std::vector<MYSTR>(2, "检查0是否有问题"));

		test_Author_P(author_a);

		fd = _wfopen(DS_BPT_TEST_ALS, L"a");
		_wsetlocale(0, L"chs");
		wchar_t* ser = author_a.serialize();
		fwprintf(fd, L" \n====serialize====\n");
		fwprintf(fd, L"%s; ", (wchar_t*)ser);
		fclose(fd);

		Author b = author_a.deserialize(ser);
		test_Author_P(b);
		delete[] ser;
	}
	void FST::test_AUTHOR(DWORD flag) {
		if (flag & 1) test_AUTHOR0();
	}
}