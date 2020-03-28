#include "Article_Detail_Widget.h"
#include "Article.h"
#include "test.h"

Article_Detail_Widget::Article_Detail_Widget(QString parameter, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Article_Detail_Widget();
	ui->setupUi(this);
	initData(parameter);
}

Article_Detail_Widget::~Article_Detail_Widget()
{
	delete ui;
}

void Article_Detail_Widget::initData(const QString& parameter) {
	Article article;
	try {
#ifndef TEST_DEBUG
		article = Article::GetArticle();
#else:
		article = *FST::ARTICLES[rand() % FST::ARTICLESN];
#endif // !ONLINE
	}
	catch(std::exception e) {
	}
}