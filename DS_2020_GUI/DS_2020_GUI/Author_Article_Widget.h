#pragma once

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <map>
#include "Author.h"
#include "Article.h"

struct Author_Article_Item{
	bstr_t title;
	std::vector<bstr_t> authors;
	Author_Article_Item();
	Author_Article_Item(Article *article);
	~Author_Article_Item();
};
Q_DECLARE_METATYPE(Author_Article_Item)

class ArticleDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit ArticleDelegate(QObject* parent = 0);
	~ArticleDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Author_Article_Widget; };

class Author_Article_Widget : public QWidget
{
	Q_OBJECT

public:
	Author_Article_Widget(QString parameter, QWidget *parent = Q_NULLPTR);
	~Author_Article_Widget();
	void initData(QString&);

private slots:
	void on_allBtn_clicked();
	void on_collaboratorBtn0_clicked();
	void on_collaboratorBtn1_clicked();
	void on_collaboratorBtn2_clicked();
	void on_collaboratorBtn3_clicked();
	void on_collaboratorBtn4_clicked();

private:
	Author                    author;
	Ui::Author_Article_Widget *ui;
	QStandardItemModel        *article_model;
	ArticleDelegate           *article_delegate;
	QSortFilterProxyModel     *article_proxyModel;
};
