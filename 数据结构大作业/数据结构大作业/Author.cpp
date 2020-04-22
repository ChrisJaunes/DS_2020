#include "pch.h"
#include "Info.h"
#include "Author.h"
#include "BPTrees.h"
#include <queue>

#define STR bstr_t
#define W int
extern BPTree<Author, bstr_t> Author_BPtree;

Author::Author() {
	this->name = ""; 
	this->articles      = new std::map<STR, std::vector<STR> >();
	this->collaborators = new std::map<STR, W>();
}

Author::Author(STR& _name, std::map<STR, std::vector<STR> >& _articles)
{
	this->name = _name;
	this->articles      = new std::map<STR, std::vector<STR> >(_articles);
	this->collaborators = new std::map<STR, W>();
	for (auto &it : _articles) {
		for (auto& jt : it.second) (*collaborators)[jt] += 1;
	}
}

Author::Author(Author& _author)
{
	this->name = _author.name;
	this->articles     = new std::map<STR, std::vector<STR> >(*_author.articles);
	this->collaborators = new std::map<STR, W>(*_author.collaborators);
}

Author& Author::operator= (Author& _author)
{
	if (articles)     delete articles;
	if (collaborators) delete collaborators;
	this->articles      = new std::map<STR, std::vector<STR> >(*_author.articles);
	this->collaborators = new std::map<STR, W>(*_author.collaborators);
	return *this;
}

Author::~Author() {
	delete articles;
	delete collaborators;
}

std::pair<OPRESULT, STR > Author::GetName() const {
	return std::make_pair(true, name);
}

void Author::SetName(STR _name)
{
	name = _name;
}

OPRESULT Author::AddArticle(STR& _name, std::vector<STR>& _author)
{
	if (articles->count(_name)) {
		return false;
	}
	for (auto& it : _author) {
		(*articles)[_name].push_back(it);
		(*collaborators)[it]++;
	}
	return true;
}

std::pair<OPRESULT, std::vector<STR>> Author::GetTitleOfArticles()
{
	vector<STR> res;
	for (auto it : *articles) {
		res.push_back(it.first);
	}
	return make_pair(true, res);
}

std::pair<OPRESULT, std::map<STR, std::vector<STR> > > Author::GetArticles()
{
	return std::make_pair(true, *articles);
}

OPRESULT Author::GetNumOfArticle() const
{
	return articles->size();
}


OPRESULT Author::GetCollaboratorByArticle(const STR& titleOfArticle, std::map<STR, W>& res)
{
	if (articles->count(titleOfArticle)) {
		for (auto it : articles->at(titleOfArticle)) {
			res[it] += 1;
		}
	}
	return true;
}

std::pair<OPRESULT, std::vector<pair<W, STR> > > Author::GetCollaborators() {
	vector<pair<W, STR> > res;
	for (auto &it : *collaborators) {
		res.push_back(std::make_pair(it.second, it.first));
	}
	std::sort(res.begin(), res.end());
	std::reverse(res.begin(), res.end());
	return make_pair(true, res);
}

std::pair<OPRESULT, std::vector<STR> > Author::GetCollaboratorsNoWeight() {
	vector<STR> res;
	for (auto& it : *collaborators) {
		res.push_back(it.first);
	}
	return make_pair(true, res);
}

#include "AuthorMarshal.h"
wchar_t* Author::serialize()
{
	return AuthorMarshal::Marshal(*this);
}

Author Author::deserialize(STR xmlcode)
{

	return AuthorMarshal::Unmarshal(xmlcode);
}


#undef STR
#undef W