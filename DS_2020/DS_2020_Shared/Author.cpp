#include "pch.h"
#include "config.h"
#include "Author.h"
#include "AuthorMarshal.h"

#ifdef TEST_DEBUG_AUTHOR
unsigned int Author::Author_cnt = 0;
#endif

Author::Author() {
#ifdef TEST_DEBUG_AUTHOR
	++Author_cnt;
#endif
	name = ""; 
}

Author::Author(const MYSTR& _name, const std::map<MYSTR, std::vector<MYSTR> >& _articles)
{
#ifdef TEST_DEBUG_AUTHOR
	++Author_cnt;
#endif
	name = _name;
	articles = _articles;
	for (auto &it : _articles) {
		for (auto& jt : it.second) 
			collaborators[jt] += 1;
	}
}

Author::Author(const Author& _author)
{
#ifdef TEST_DEBUG_AUTHOR
	++Author_cnt;
#endif
	name = _author.name;
	articles = _author.articles;
	collaborators = _author.collaborators;
}

Author& Author::operator= (Author& _author)
{
	articles      = _author.articles;
	collaborators = _author.collaborators;
	return *this;
}

Author::~Author() {
#ifdef TEST_DEBUG_AUTHOR
	--Author_cnt;
#endif
}

void Author::SetName(MYSTR _name)
{
	name = _name;
}

std::pair<OPRESULT, MYSTR > Author::GetName() const {
	return std::make_pair(true, name);
}

OPRESULT Author::AddArticle(const MYSTR& _title, const std::vector<MYSTR>& _author)
{
	if (articles.count(_title)) {
		return false;
	}
	for (auto& it : _author) {
		articles[_title].push_back(it);
		collaborators[it]++;
	}
	return true;
}

std::pair<OPRESULT, std::vector<MYSTR>> Author::GetTitleOfArticles()
{
	std::vector<MYSTR> res;
	for (auto it : articles) {
		res.push_back(it.first);
	}
	return make_pair(true, res);
}

std::pair<OPRESULT, std::map<MYSTR, std::vector<MYSTR> > > Author::GetArticles()
{
	return std::make_pair(true, articles);
}

OPRESULT Author::GetNumOfArticle() const
{
	return articles.size();
}


OPRESULT Author::GetCollaboratorByArticle(const MYSTR& titleOfArticle, std::map<MYSTR, int>& res)
{
	if (articles.count(titleOfArticle)) {
		for (auto it : articles.at(titleOfArticle)) {
			res[it] += 1;
		}
	}
	return true;
}

std::pair<OPRESULT, std::vector<std::pair<int, MYSTR> > > Author::GetCollaborators() {
	std::vector<std::pair<int, MYSTR> > res;
	for (auto &it : collaborators) {
		res.push_back(std::make_pair(it.second, it.first));
	}
	std::sort(res.begin(), res.end());
	std::reverse(res.begin(), res.end());
	return make_pair(true, res);
}

std::pair<OPRESULT, std::vector<MYSTR> > Author::GetCollaboratorsNoWeight() {
	std::vector<MYSTR> res;
	for (auto& it : collaborators) {
		res.push_back(it.first);
	}
	return make_pair(true, res);
}

wchar_t* Author::serialize()
{
	return AuthorMarshal::Marshal(*this);
}

Author Author::deserialize(MYSTR xmlcode)
{

	return AuthorMarshal::Unmarshal(xmlcode);
}
