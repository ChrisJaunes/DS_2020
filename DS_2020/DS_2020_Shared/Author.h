#pragma once
#include "pch.h"
#include "error.h"
#include "config.h"
/*@by ChrisJaunes_hjj;
 * 本类提供了作者的相关信息
 * 提供了构造函数、复制构造函数、赋值函数
 * 如果没有把握，请不要直接使用普通指针指向本类
 * 可以使用shared_ptr等智能指针指向本类
 * (失效)model: 作者类模式，当为Base版时, 只初始化文章名，不初始化合作者
 * SetName: 设置作者名
 * GetName: 获得作者名
 * AdArticle: 添加文章, 不应该添加重复的文章
 * GetTitleOfArticles： 获得所写文章的标题
 * GetArticles: 获得文章及每篇文章的全部作者
 * GetNumOfArticle: 获得文章数量
 * GetCollaboratorByArticle: 获得文章的合作者
 * GetCollaborators: 获得全部合作者，带合作关系权重
 * GetCollaboratorsNoWeight： 获得全部合作者，不带权重
 * serialize： 序列化
 * deserialize： 反序列化
 */
enum {
	Author_Class_Base,
	Author_Class_Pro,
};
#define Collaborator Author
class Author {
protected:
	MYSTR name;
	std::map<MYSTR, std::vector<MYSTR> > articles;
	std::map<MYSTR, int> collaborators;

public:
	Author();
	Author(const MYSTR&, const std::map<MYSTR, std::vector<MYSTR> >&);
	Author(const Author&);
	Author& operator = (Author&);
	~Author();

	void SetName(MYSTR);
	std::pair<OPRESULT, MYSTR> GetName() const ;

	OPRESULT AddArticle(const MYSTR& _title, const std::vector<MYSTR>& _author);
	std::pair<OPRESULT, std::vector<MYSTR> > GetTitleOfArticles();
	std::pair<OPRESULT, std::map<MYSTR, std::vector<MYSTR> > > GetArticles();
	OPRESULT GetNumOfArticle()const;

	OPRESULT GetCollaboratorByArticle(const MYSTR&, std::map<MYSTR, int>&);
	std::pair<OPRESULT, std::vector<std::pair<int, MYSTR> > > GetCollaborators();
	std::pair<OPRESULT, std::vector<MYSTR> > GetCollaboratorsNoWeight();

	wchar_t* serialize();
	static Author deserialize(MYSTR);
#ifdef TEST_DEBUG_AUTHOR
	static unsigned int Author_cnt;
#endif
};

struct AuthorCmpByNumOfArticle {
	bool operator()(const Author *a, const Author *b) {
		return a->GetNumOfArticle() < b->GetNumOfArticle();
	}
};