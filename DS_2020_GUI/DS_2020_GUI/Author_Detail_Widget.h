#pragma once

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include "pch.h"
#include "Info.h"
#include "Author.h"

struct Author_Property_Item{
	QString title;
	QString authors;
	Author_Property_Item();
	Author_Property_Item(QString&, QString&);
	void addAuthor(const QString&);
	~Author_Property_Item();
};
Q_DECLARE_METATYPE(Author_Property_Item)

class AuthorDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit AuthorDelegate(QObject* parent = 0);
	~AuthorDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Author_Detail_Widget; };

class Author_Detail_Widget : public QWidget
{
	Q_OBJECT

public:
	Author_Detail_Widget(QString parameter, QWidget *parent = Q_NULLPTR);
	~Author_Detail_Widget();
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
	Ui::Author_Detail_Widget  *ui;
	QStandardItemModel        *author_model;
	AuthorDelegate            *author_delegate;
	QSortFilterProxyModel     *author_proxyModel;
};
