#pragma once
#include "pch.h"
#include "Article.h"
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