#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Hotspot_Analysis_Widget.h"
#include "ui_Hotspot_Analysis_Widget.h"
#include "test.h"
#define STR QString
#define W int
Hotspot_Item::Hotspot_Item()
{
}

Hotspot_Item::Hotspot_Item(STR _hotspot_word, W _hotspot_number)
	: hotspot_word(_hotspot_word)
	, hotspot_number(_hotspot_number)
{

}

Hotspot_Item::~Hotspot_Item()
{
}

HotspotDelegate::HotspotDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}
HotspotDelegate::~HotspotDelegate()
{
}
void HotspotDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        QVariant variant = index.data(Qt::UserRole + 1);
        Hotspot_Item data = variant.value<Hotspot_Item>();

        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width() - 1.0);
        rect.setHeight(option.rect.height() - 1.0);

        //QPainterPath画圆角矩形
        const qreal radius = 7;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));


        //绘制数据位置
        QRect wordRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 50, 20);
        QRect numberRect = QRect(rect.left() + 50, rect.top() + 10, rect.width() - 10, 20);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 12, QFont::Bold));
        painter->drawText(wordRect, Qt::AlignLeft, data.hotspot_word);

        painter->setPen(QPen(Qt::gray));
        painter->setFont(QFont("Times", 10));
        painter->drawText(numberRect, Qt::AlignLeft, QString::number(data.hotspot_number));

        painter->restore();
    }
}
QSize HotspotDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}



Hotspot_Analysis_Widget::Hotspot_Analysis_Widget(QString parameter, QWidget *parent)
	: QWidget(parent)
{
#ifdef TEST_DEBUG
    clock_t beg = clock();
#endif

	ui = new Ui::Hotspot_Analysis_Widget();
	ui->setupUi(this);

    initData(parameter);

    hotspot_delegate = new HotspotDelegate(this);
    ui->listView->setItemDelegate(hotspot_delegate);
    ui->listView->setModel(hotspot_model);

#ifdef TEST_DEBUG
    qDebug() << "Hotspot_Analysis_Widget" << clock() - beg << '\n';
#endif
}

Hotspot_Analysis_Widget::~Hotspot_Analysis_Widget()
{
	delete ui;
}

void Hotspot_Analysis_Widget::initData(const QString& parameter)
{
	QVector<Hotspot_Item> data;
#ifndef TEST_DEBUG
	
#else
	data.push_back(Hotspot_Item("A", 10));
	data.push_back(Hotspot_Item("B", 20));
	data.push_back(Hotspot_Item("C", 30));
	data.push_back(Hotspot_Item("D", 40));
    data.push_back(Hotspot_Item("E", 50));
    data.push_back(Hotspot_Item("F", 60));
    data.push_back(Hotspot_Item("G", 70));
    data.push_back(Hotspot_Item("H", 80));
    data.push_back(Hotspot_Item("I", 90));
    data.push_back(Hotspot_Item("J", 00));
#endif

    hotspot_model = new QStandardItemModel(data.size(), 1);
    for (int i = 0; i < data.size(); i++) {
        hotspot_model->setData(hotspot_model->index(i, 0), QVariant::fromValue(data[i]), Qt::UserRole + 1); //整体存取
    }
}


#undef STR
#undef W