#include "FSolver.h"

FSolver::FSolver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist)
{
	ms = new DblpBptMs(info_bpt_file, author_bpt_file, exist);
}
FSolver::~FSolver() {
	delete ms;
}
std::pair<OPRESULT, Author> FSolver::F1_getAuthorByName(const MYSTR& name)
{
	return ms->getAuthorByName(name);
}

std::pair<OPRESULT, std::vector<Info> > FSolver::F1_getInfoByTitle(const MYSTR& title)
{
	return ms->getInfoByTitle(title);
}

std::vector<Author> FSolver::F2_getTop100()
{
	return ms->getTop100();
}
