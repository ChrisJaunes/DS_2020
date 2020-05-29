#pragma once
#include "FSolver.h"
extern FSolver fsolver;

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include "pch.h"

struct Author_Property_Item{
	QString title;
	QString authors;
	Author_Property_Item();
	Author_Property_Item(QString&, QString&);
	void addAuthor(const QString&);
	~Author_Property_Item();
};
Q_DECLARE_METATYPE(Author_Property_Item)

class Author_Property_Delegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit Author_Property_Delegate(QObject* parent = Q_NULLPTR);
	~Author_Property_Delegate();

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
	void showInfo(QModelIndex index);
	void showInfoDetail(QModelIndex index);

private:
	Ui::Author_Detail_Widget  *ui;
	QStandardItemModel        *author_model;
	QStandardItemModel        *collaborator_model;
	Author_Property_Delegate  *author_delegate;
	QSortFilterProxyModel     *author_proxyModel;
};
