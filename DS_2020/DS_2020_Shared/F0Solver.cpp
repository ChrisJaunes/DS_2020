#include "F0Solver.h"

F0Solver::F0Solver(BPTMS<key_t, Info>* info_ms, BPTMS<key_t, Author>* author_ms)
	: info_ms(info_ms)
	, author_ms(author_ms)
{
}

F0Solver::~F0Solver()
{
}

void F0Solver::GenerateCache()
{
}

void F0Solver::InsertObject(Info& _info)
{
	auto authors = _info.GetProperty(L"author");
	info_ms->insertMulti(key_t(_info.GetProperty(L"title").at(0)), _info);
	for (auto it : authors) {
		key_t key(it);
		auto jt = author_ms->search(key);
		if (jt.size() == 0) jt.push_back(Author(it));
		assert(jt.size() == 1);
		auto kt = jt.at(0);
		kt.AddArticle(it, authors);
		author_ms->insertReplace(key, kt);
	}
}
