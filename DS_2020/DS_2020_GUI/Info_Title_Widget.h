#pragma once
#include "FSolver.h"
extern FSolver fsolver;

#include <QWidget>
#include <QObjectUserData>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QSortFilterProxyModel>

struct Info_Title_Item {
	QString title;
	Info_Title_Item();
	Info_Title_Item(QString&);
	~Info_Title_Item();
};

Q_DECLARE_METATYPE(Info_Title_Item)

class Info_Title_Delegate : public QStyledItemDelegate
{
	Q_OBJECT
signals:

public:
	explicit Info_Title_Delegate(QObject* parent = Q_NULLPTR);
	~Info_Title_Delegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
namespace Ui { class Info_Title_Widget; };

class Info_Title_Widget : public QWidget
{
	Q_OBJECT

public:
	Info_Title_Widget(QString&, QWidget *parent = Q_NULLPTR);
	~Info_Title_Widget();
	void initData(const QString&);

public slots:
	void on_btn_title_to_info_clicked();
private:
	Ui::Info_Title_Widget *ui;
	QStandardItemModel    *title_model;
	Info_Title_Delegate   *title_delegate;
	std::vector<std::string> titles;
};
