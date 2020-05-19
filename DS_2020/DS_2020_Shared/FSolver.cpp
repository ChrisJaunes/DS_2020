#include "FSolver.h"

std::pair<OPRESULT, Author> FSolver::F1_getAuthorByName(const MYSTR& name)
{
	return ms->getAuthorByName(name);
}

std::pair<OPRESULT, Info> FSolver::F1_getInfoByTitle(const MYSTR& title)
{
	return ms->getInfoByTitle(title);
}

std::vector<Author> FSolver::F2_getTop100()
{
	return ms->getTop100();
}
