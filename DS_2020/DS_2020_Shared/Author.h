#pragma once
#include "pch.h"
#include "error.h"
#include "config.h"
/*@by ChrisJaunes_hjj;
 * �����ṩ�����ߵ������Ϣ
 * �ṩ�˹��캯�������ƹ��캯������ֵ����
 * ���û�а��գ��벻Ҫֱ��ʹ����ָͨ��ָ����
 * ����ʹ��shared_ptr������ָ��ָ����
 * (ʧЧ)model: ������ģʽ����ΪBase��ʱ, ֻ��ʼ��������������ʼ��������
 * SetName: ����������
 * GetName: ���������
 * AdArticle: �������, ��Ӧ������ظ�������
 * GetTitleOfArticles�� �����д���µı���
 * GetArticles: ������¼�ÿƪ���µ�ȫ������
 * GetNumOfArticle: �����������
 * GetCollaboratorByArticle: ������µĺ�����
 * GetCollaborators: ���ȫ�������ߣ���������ϵȨ��
 * GetCollaboratorsNoWeight�� ���ȫ�������ߣ�����Ȩ��
 * serialize�� ���л�
 * deserialize�� �����л�
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