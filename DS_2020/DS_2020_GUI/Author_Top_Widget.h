#pragma once
#include "FSolver.h"
extern FSolver fsolver;

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>

struct Author_Top_Item {
	QString name;
	MYW number;
	Author_Top_Item();
	Author_Top_Item(QString&, MYW&);
	~Author_Top_Item();
};
Q_DECLARE_METATYPE(Author_Top_Item)

class AuthorTopDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit AuthorTopDelegate(QObject* parent = Q_NULLPTR);
	~AuthorTopDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Author_Top_Widget; };

class Author_Top_Widget : public QWidget
{
	Q_OBJECT

public:
	Author_Top_Widget(QWidget *parent = Q_NULLPTR);
	~Author_Top_Widget();
	void initData();

private:
	Ui::Author_Top_Widget *ui;
	QStandardItemModel    *top_model;
	AuthorTopDelegate     *top_delegate;
};