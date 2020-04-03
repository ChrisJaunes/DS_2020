#pragma once

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#define W int
struct Author_Top_Item {
	QString name;
	W number;
	Author_Top_Item();
	Author_Top_Item(QString&, W&);
	~Author_Top_Item();
};
Q_DECLARE_METATYPE(Author_Top_Item)

class TopDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit TopDelegate(QObject* parent = Q_NULLPTR);
	~TopDelegate();

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
	TopDelegate           *top_delegate;
};
#undef W