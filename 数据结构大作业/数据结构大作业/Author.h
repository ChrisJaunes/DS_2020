#pragma once
#include "pch.h"
#include "error.h"

#ifndef AUTHOR_H
#define AUTHOR_H
/*
 * �����ṩ�����ߵ������Ϣ
 * name ���ߵ�����
 * articles ����������µ���Ϣ���Զ���ʼ��
 * Author �յĹ��캯��
 * Author(STR) ���������ֽ��г�ʼ��������ʼ��articles �� collaborators����ʼ��Author_BPTree�ļ�ʱʹ��;
 * Author(STR, std::vector<STR >&, std::vector<pair<STR,W>& ) debugʱ�ֶ�����Author���ݺ�ͨ��Author_BPtree���
 * ~Author() delete��ָ��
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