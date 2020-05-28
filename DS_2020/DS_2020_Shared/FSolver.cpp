#include "FSolver.h"
#include "f4.h"

FSolver::FSolver(const wchar_t* info_bpt_file, const wchar_t* author_bpt_file, FILE_Status exist)
{
	ms = new DblpBptMs(info_bpt_file, author_bpt_file, exist);
	f4p = new f4func();
	f4p->initial();
	pf5 = new CliquesCounting();
	//pf5->initial();
}
FSolver::~FSolver() {
	delete ms;
	delete f4p;
	delete pf5;
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

bool FSolver::F4_KeywordSearch(std::string&keywords,std::vector<std::string>&titles) 
{
	return f4p->keyword_search(keywords,titles);
}

std::map<W, W> FSolver::F5_getResult()
{
	pf5->set_ms(ms);
	pf5->readfile_f5result();
	return pf5->GetResult();
}
std::map<W, MYSTR> FSolver::F5_getResult_2()
{
	return pf5->GetResult_2();
}
