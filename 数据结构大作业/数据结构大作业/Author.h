#pragma once
#include "pch.h"
#include "article.h"
#ifndef AUTHOR_H
#define AUTHOR_H
/*
 * 
 */
#define Collaborator Author
class Author {
protected:
	bstr_t name;
	std::vector<bstr_t>* articles;
public:
	Author();
	Author(bstr_t, std::vector<bstr_t>&);
	~Author();
	OPRESULT GetNumOfArticle()const;
	std::pair<OPRESULT, std::vector<bstr_t> > GetArticle();
	std::pair<OPRESULT, std::vector<bstr_t> > GetCollaboratorByArticle(bstr_t*);
	std::pair<OPRESULT, std::vector<bstr_t> > GetCollaborator();
	static std::pair<OPRESULT, std::vector<bstr_t> >GetTopNOfNumOfArticle(int lim = 100);
};
struct AuthorCmpByNumOfArticle {
	bool operator()(const Author *a, const Author *b) {
		return a->GetNumOfArticle() < b->GetNumOfArticle();
	}
};
#endif