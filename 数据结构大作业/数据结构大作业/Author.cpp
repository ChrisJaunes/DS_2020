#include "Author.h"
#include "Article.h"
#include "BPTrees.h"
#include <queue>

extern BPTree<Article, bstr_t> Article_BPtree;

Author::Author() {
	this->name = "";
	this->articles = new std::vector<bstr_t>();
}

Author::Author(bstr_t _name, std::vector<bstr_t>& _articles) {
	this->name = _name;
	this->articles = new std::vector<bstr_t> (_articles);
}

Author::~Author() {
	delete this->articles;
}

OPRESULT Author::GetNumOfArticle() const{
	return articles->size();
}

std::pair<OPRESULT, std::vector<bstr_t> > Author::GetArticle() {
	try {
		return make_pair(true, std::vector<bstr_t>(*(this->articles)));
	}
	catch (std::exception e) {
		return make_pair(false, std::vector<bstr_t>());
	}
}

//todo
std::pair<OPRESULT, std::vector<bstr_t> > Author::GetCollaboratorByArticle(bstr_t* nameOfArticle) {
	if (!nameOfArticle) {
		CHK_OR(E_InvalidParameter);
		return make_pair(false, std::vector<bstr_t>());
	}
	//获取BPtree的文章节点,并且存到article里
	Article article;// Article_BPtree.serch(nameOfArtcile); 
	//发生意外返回make_pair(false, vector<bstr_t>());
	vector<bstr_t>* authors;
	article.Getauthors(authors);
	return make_pair(true, *authors);
}

std::pair<OPRESULT, std::vector<bstr_t> > Author::GetCollaborator() {
	try {
		vector<bstr_t> authors;
		for (auto article : *articles) {
			std::pair<OPRESULT, std::vector<bstr_t> > res = GetCollaboratorByArticle(&article);
			if (res.first) throw std::exception();
			authors.insert(authors.end(), res.second.begin(), res.second.end());
		}
		std::sort(authors.begin(), authors.end());
		authors.erase(std::unique(authors.begin(), authors.end()), authors.end());
		return make_pair(true, authors);
	}
	catch (std::exception e) {
		return make_pair(false, std::vector<bstr_t>());
	}
}

//todo
std::pair<OPRESULT, std::vector<bstr_t> > Author::GetTopNOfNumOfArticle(int lim = 100) {
	Author *author_begin;//从BPtree中获得
	Author *author_end;  //从BPtree中获得
	std::priority_queue<Author*> q;
	priority_queue<Author*, vector<Author*>, AuthorCmpByNumOfArticle> pq;
	//for (;author_begin != author_end; author_begin->next) {
	//
	//}
	vector<bstr_t> res;
	while (!q.empty()) {
		res.emplace_back(q.top()->name); q.pop();
	}
	return make_pair(true, res);
}