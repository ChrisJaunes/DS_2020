#pragma once
#include "pch.h"
#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#define STR QString
#define W ULONG64
struct Hotspot_Property_Item {
	QString hotspot_word;
	W hotspot_number;
	Hotspot_Property_Item();
	Hotspot_Property_Item(QString, W);
	~Hotspot_Property_Item();
};
Q_DECLARE_METATYPE(Hotspot_Property_Item)

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

namespace Ui { class Hotspot_Detail_Widget; };

class Hotspot_Detail_Widget : public QWidget
{
	Q_OBJECT

public:
	Hotspot_Detail_Widget(QWidget* parent = Q_NULLPTR);
	~Hotspot_Detail_Widget();
	void initData();

private slots:
	void on_btn_ok_clicked();

private:
	Ui::Hotspot_Detail_Widget* ui;
	QStandardItemModel* hotspot_model;
	HotspotDelegate* hotspot_delegate;
	QSortFilterProxyModel* hotspot_proxyModel;
	static const int HOTSPOTTYPEN = 8;
	bstr_t HOTSPOTTYPE[HOTSPOTTYPEN] = { L"articles",L"books", L"incollection", L"inproceedings", L"mastersthesis",L"phdthesis",L"proceedings", L"www" };
};

#undef STR
#undef W