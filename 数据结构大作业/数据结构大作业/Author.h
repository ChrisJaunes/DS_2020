#pragma once
#include "pch.h"
#include "Article.h"
#ifndef AUTHOR_H
#define AUTHOR_H
/*
 * 本类提供了作者的相关信息
 * name 作者的名字
 * articles 作者相关文章的信息，自动初始化
 * Author 空的构造函数
 * Author(STR) 以作者名字进行初始化，不初始化articles 和 collaborators，初始化Author_BPTree文件时使用;
 * Author(STR, std::vector<STR >&, std::vector<pair<STR,W>& ) debug时手动构造Author数据和通过Author_BPtree获得
 * ~Author() delete掉指针
 * 
 */
#define Collaborator Author
template<typename STR, typename W>
class Author {
protected:
	STR name;
	std::vector<STR>* articles;
	std::vector<std::pair<W, STR> >* collaborators;
public:
	Author();
	Author(STR);
	Author(STR, std::vector<STR> &, std::vector<std::pair<W, STR> >& );
	~Author();
	OPRESULT GetNumOfArticle()const;
	OPRESULT AddArticle(STR);
	std::pair<OPRESULT, std::vector<STR> > GetArticles();
	OPRESULT GetCollaboratorByArticle(const STR&, std::map<STR, W>&);
	std::pair<OPRESULT, std::vector<W, STR> > GetCollaborators();
	static std::pair<OPRESULT, std::vector<STR> >GetTopNOfNumOfArticle(int lim = 100);
	static Author<STR,W> getAuthorByArticle(STR&);
};

template<typename STR, typename W>
struct AuthorCmpByNumOfArticle {
	bool operator()(const Author<STR, W> *a, const Author<STR,W> *b) {
		return a->GetNumOfArticle() < b->GetNumOfArticle();
	}
};
#endif