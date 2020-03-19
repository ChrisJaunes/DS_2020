#include "pch.h"
#include "Author.h"
#include "Article.h"
#include "BPTrees.h"
#include <queue>

extern BPTree<Article, bstr_t> Article_BPtree;
extern BPTree<Author<bstr_t, int>, bstr_t> Author_BPtree;

template<typename STR, typename W>
Author<STR, W>::Author() {
	this->name = "";
	this->articles = new std::vector<STR>();
	this->collaborators = nullptr;
}

template<typename STR, typename W>
Author<STR, W>::Author(STR _name) {
	this->name = _name;
	this->articles = new std::vector<STR>();
	this->collaborators = nullptr;
}

template<typename STR, typename W>
Author<STR, W>::Author(STR _name, std::vector<STR>& _articles, std::vector<std::pair<W, STR>>& _collaborators)
{
	this->name = _name;
	this->articles = new std::vector<STR> (_articles);
	this->collaborators = new std::vector<pair<W, STR> >(_collaborators);
}

template<typename STR, typename W>
Author<STR, W>::~Author() {
	delete this->articles;
	delete this->collaborators;
}

template<typename STR, typename W>
OPRESULT Author<STR, W>::GetNumOfArticle() const{
	return articles->size();
}

template<typename STR, typename W>
OPRESULT Author<STR, W>::AddArticle(STR _name)
{
	articles->push_back(_name);
	return OPRESULT();
}

template<typename STR, typename W>
std::pair<OPRESULT, std::vector<STR>> Author<STR, W>::GetArticles()
{
	try {
		return make_pair(true, std::vector<STR>(*(this->articles)));
	}
	catch (std::exception e) {
		return make_pair(false, std::vector<STR>());
	}
}

template<typename STR, typename W>
OPRESULT Author<STR, W>::GetCollaboratorByArticle(const STR& nameOfArticle, std::map<STR, W>& res)
{
	try {
		vector<Article> articles;
		Article_BPtree.search(articles, nameOfArticle);
		for (auto article : articles) {
			//vector<STR> authorByArticle;
			vector<bstr_t> authorByArticle;
			article.Getauthors(&authorByArticle);
			for (auto it : authorByArticle) {
				res[it] += 1;
			}
		}
		return true;
	}
	catch (std::exception e) {
		return false;
	}
}

template<typename STR, typename W>
std::pair<OPRESULT, std::vector<W, STR>> Author<STR, W>::GetCollaborators() {
	try {
		if (collaborators == nullptr) {
			std::map<STR, W> res;
			for (int i = 0; i < (*articles).size(); i++) {
				STR article = articles->at(i);
				if (!GetCollaboratorByArticle(article, res)) throw std::exception;
			}
			res.erase(res.find(this->name));
			collaborators = new vector<pair<W, STR> >();
			for (auto collaborator : res) {
				collaborators->push_back(make_pair(collaborator.second, collaborator.first));
			}
			sort(collaborators->begin(), collaborators->end());
		}
		return make_pair(true, std::vector<STR>(*(this->collaborators)));
	}
	catch (std::exception e) {
		return make_pair(false, std::vector<STR>());
	}
}

//todu
template<typename STR, typename W>
std::pair<OPRESULT, std::vector<STR>> Author<STR, W>::GetTopNOfNumOfArticle(int lim)
{
	try {
		BPLeaf<Author, bstr_t>* author_begin = Author_BPtree.begin();
		BPLeaf<Author, bstr_t>* author_end = Author_BPtree.end();
		std::priority_queue<Author*> q;
		priority_queue<Author*, vector<Author*>, AuthorCmpByNumOfArticle<STR, W> > pq;
		for (; author_begin != author_end; author_begin->next) {
			//author_begin->getvalue();
		}
		vector<bstr_t> res;
		while (!q.empty()) {
			res.emplace_back(q.top()->name); q.pop();
		}
		return make_pair(true, res);
	}
	catch (std::exception e) {
		return make_pair(false, vector<STR>());
	}
}

//todo
template<typename STR, typename W>
Author<STR,W> Author<STR, W>::getAuthorByArticle(STR& _name)
{
	//Author_BPtree.search();
	return Author();
}

