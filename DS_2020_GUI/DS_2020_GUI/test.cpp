#include "test.h"
#include "qdebug.h"
namespace FST {
	Author  *AUTHORS[AUTHORSN];
	Article *ARTICLES[ARTICLESN];
	bstr_t getStr(char ch) {
		return bstr_t((QString(ch) + char(rand() % 26 + 'a') + char(rand() % 26 + 'A')).toStdWString().c_str());
	}
	void create_FST() {
		srand(20190319);
		for (int i = 0; i < AUTHORSN; i++) {
			std::vector<bstr_t> tmp_articles;
			for (int j = 0; j < 5; j++) {
				tmp_articles.push_back(getStr('a'));
			}
			std::vector<std::pair<int, bstr_t>> tmp_collaborator;
			for (int j = 0; j < 5; j++) {
				tmp_collaborator.push_back(std::make_pair(rand() % 1000, getStr('b')));
			}
			AUTHORS[i] = new Author(getStr('c'), tmp_articles, tmp_collaborator);
			qDebug() << AUTHORS[i]->GetNumOfArticle() << '\n';
		}
		for (int i = 0; i < ARTICLESN; i++) {
			std::vector<bstr_t> tmp_authors;
			for (int j = 0; j < 5; j++) {
				tmp_authors.push_back(getStr('d'));
			}
			ARTICLES[i] = new Article(
				getStr('m'), getStr('m'), getStr('m'), getStr('m'),
				getStr('m'), getStr('m'), getStr('m'), getStr('m'),
				getStr('m'), tmp_authors
			);
		}
	}
}