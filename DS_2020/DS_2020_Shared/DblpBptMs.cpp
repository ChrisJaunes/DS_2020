#include "DblpBptMs.h"
#include "InfoMarshal.h"
#include "CommUtils.h"

DblpBptMs::DblpBptMs(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist)
	: info_bpt(info_bpt_file, exist)
	, author_bpt(author_bpt_file, exist)
{
	info_cnt = 0;
	info_data_block_sz = info_bpt.data_block_size - sizeof(OFF_T) - sizeof(OFF_T);
	author_data_block_sz = author_bpt.data_block_size - sizeof(OFF_T) - sizeof(OFF_T);
}

DblpBptMs::~DblpBptMs()
{
}

void DblpBptMs::InitMemory()
{
}

void DblpBptMs::InsertObject(Info& _info)
{
#ifdef TEST_DEBUG
	++info_cnt;
	//if (info_cnt % 1878286 == 0) {
	//	printf("%d\n", info_cnt);
	//	wprintf(L"%s", (const wchar_t*)_info.serialize());
	//}
	//printf("%d\n", info_cnt);
	if(info_cnt % 50000 == 0) printf("%d\n", info_cnt);
#endif

	insertInfo(_info);
	auto authors = _info.GetProperty(L"author");
	auto title = _info.GetProperty(L"title").at(0);
	for (auto it : authors) {
		AuthorKey key(it);
		auto jt = getAuthorByName(key.a);
		Author kt(jt.second);
		if (jt.first == false) kt.SetName(it);
		kt.AddArticle(title, authors);
		updateAuthor(kt);
	}
#ifdef TEST_DEBUG
	//auto title = _info.GetProperty(L"title").at(0);
	//auto jt = getInfoByTitle(title);
	//if (jt.first == false) {
	//	MyLog::e("info:%S ", title);
	//}
#endif
}

void DblpBptMs::insertInfo(Info& _info)
{
	InfoKey key(_info.GetProperty(L"title").at(0));
	wchar_t* value = _info.serialize();
	size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);
	
	size_t _value_sz = (value_sz + info_data_block_sz - 1) / info_data_block_sz * info_data_block_sz;
	void* _value = malloc(_value_sz);
	memset(_value, 0, _value_sz);
	memcpy(_value, value, value_sz);

	info_bpt.insert(key, _value, _value_sz);

	free(_value);
	delete[] value;
}

void DblpBptMs::updateAuthor(Author& _author)
{
	AuthorKey key(_author.GetName().second);
	wchar_t* value = _author.serialize();
	size_t value_sz = (wcslen(value) + 1) * sizeof(wchar_t);

	size_t _value_sz = (value_sz + author_data_block_sz - 1) / author_data_block_sz * author_data_block_sz;
	void* _value = malloc(_value_sz);
	memset(_value, 0, _value_sz);
	memcpy(_value, value, value_sz);

	author_bpt.insert(key, _value, _value_sz);

	free(_value);
	delete[] value;
}

std::pair<OPRESULT, Author> DblpBptMs::getAuthorByName(const MYSTR& key)
{
	AuthorKey _key(key);
	void* _value = nullptr;
	size_t _value_sz;
	if (author_bpt.search(_key, _value, _value_sz) == DS_BPlusTree::BPT_Res::FAILED) return std::make_pair(false, Author());	
	Author author = Author::deserialize((wchar_t*)_value);
	free(_value);
	return std::make_pair(true, author);
}

std::pair<OPRESULT, std::vector<Info> > DblpBptMs::getInfoByTitle(const MYSTR& key)
{
	InfoKey _key(key);
	void* _value = nullptr;
	size_t _value_sz;
	if (info_bpt.search(_key, _value, _value_sz) == DS_BPlusTree::BPT_Res::FAILED) return std::make_pair(false,std::vector<Info>());
	std::vector<Info> info = InfoMarshal::Unmarshal2v((wchar_t*)_value);
	free(_value);
	return std::make_pair(true, info);
}

std::vector<Author> DblpBptMs::getTop100()
{
	std::priority_queue<Author, std::vector<Author>, AuthorCmpByNumOfArticle> q;
	int author_num = 0;
	for (auto it = author_bpt.begin(); it != author_bpt.end(); ++it) {
		MyLog::d("%d\n", ++author_num);
		auto off = *it;
		void* value = nullptr; size_t value_sz = 0;
		author_bpt.valueFromFileBlock(off, value, value_sz);
		Author author = Author::deserialize((wchar_t*)value);
		q.push(author);
		free(value);
		if (q.size() > 100) q.pop();
	}
	std::vector<Author> vec;
	for (; !q.empty();) {
		vec.push_back(q.top()); q.pop();
	}
	return vec;
}
