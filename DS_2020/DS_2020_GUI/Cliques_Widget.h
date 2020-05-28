#pragma once
#include "pch.h"
#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "FSolver.h"
extern FSolver fsolver;

#define STR QString
#define W ULONG64

struct Cliques_Item {
	W order;
	W number;
	Cliques_Item();
	Cliques_Item(W, W);
	~Cliques_Item();
};
Q_DECLARE_METATYPE(Cliques_Item)

class CliquesCountDelegate : public QStyledItemDelegate
{
	Q_OBJECT
		signals :

public:
	explicit CliquesCountDelegate(QObject* parent = Q_NULLPTR);
	~CliquesCountDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Cliques_Widget; };

class Cliques_Widget : public QWidget
{
	Q_OBJECT

public:
	Cliques_Widget(QWidget *parent = Q_NULLPTR);
	~Cliques_Widget();
	void initData();

private:
	Ui::Cliques_Widget    *ui;
	QStandardItemModel    *cliques_model;
	CliquesCountDelegate  *cliques_delegate;
};
#undef W
#undef STR