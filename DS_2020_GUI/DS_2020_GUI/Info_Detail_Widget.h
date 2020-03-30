#pragma once

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include "ui_Info_Detail_Widget.h"

struct Info_Property_Item {
	QString property_name;
	QString property_data;
	Info_Property_Item();
	Info_Property_Item(QString& _name, QString& _data);
	~Info_Property_Item();
};
Q_DECLARE_METATYPE(Info_Property_Item)

class InfoDelegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit InfoDelegate(QObject* parent = 0);
	~InfoDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

namespace Ui { class Info_Detail_Widget; };

class Info_Detail_Widget : public QWidget
{
	Q_OBJECT

public:
	Info_Detail_Widget(QString& parameter, QWidget *parent = Q_NULLPTR);
	~Info_Detail_Widget();
	void initData(const QString&);

private:
	Ui::Info_Detail_Widget *ui;
	QStandardItemModel     *info_model;
	InfoDelegate           *info_delegate;
};
