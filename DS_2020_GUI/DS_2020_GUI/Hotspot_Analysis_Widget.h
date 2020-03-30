#pragma once
#include "pch.h"
#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#define STR QString
#define W int
struct Hotspot_Item {
	STR hotspot_word;
	W hotspot_number;
	Hotspot_Item();
	Hotspot_Item(STR _hotspot_word, W _hotspot_number);
	~Hotspot_Item();
};
Q_DECLARE_METATYPE(Hotspot_Item)

class HotspotDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit HotspotDelegate(QObject* parent = 0);
	~HotspotDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Hotspot_Analysis_Widget; };

class Hotspot_Analysis_Widget : public QWidget
{
	Q_OBJECT

public:
	Hotspot_Analysis_Widget(QString parameter, QWidget *parent = Q_NULLPTR);
	~Hotspot_Analysis_Widget();
	void initData(const QString&);

private:
	Ui::Hotspot_Analysis_Widget *ui;
	QStandardItemModel          *hotspot_model;
	HotspotDelegate             *hotspot_delegate;
};

#undef STR
#undef W