#pragma once
#include "pch.h"
#include "error.h"

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
#define STR bstr_t
#define W int
class Author {
protected:
	STR name;
	std::map<STR, std::vector<STR> > *articles;
	std::map<STR, W> *collaborators;

public:
	Author();
	Author(STR&, std::map<STR, std::vector<STR> >&);
	Author(Author&);
	Author& operator = (Author&);
	~Author();

	OPRESULT AddArticle(STR&, std::vector<STR>&);
	std::pair<OPRESULT, std::vector<STR> > GetTitleOfArticles();
	std::pair<OPRESULT, std::map<STR, std::vector<STR> > > GetArticles();
	OPRESULT GetNumOfArticle()const;

	OPRESULT GetCollaboratorByArticle(const STR&, std::map<STR, int>&);
	std::pair<OPRESULT, std::vector<std::pair<int, STR> > > GetCollaborators();

};


struct AuthorCmpByNumOfArticle {
	bool operator()(const Author *a, const Author *b) {
		return a->GetNumOfArticle() < b->GetNumOfArticle();
	}
};
#undef STR
#undef W
#endif