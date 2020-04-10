#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Info.h"
#include "Info_Detail_Widget.h"
#include "ui_Info_Detail_Widget.h"
#include "test.h"

Info_Property_Item::Info_Property_Item()
{
}
void Info_Property_Item::setProperty(const QString& _name, const QString& _data)
{
    property_name = _name;
    property_data = _data;
}
void Info_Property_Item::addPropertyData(const QString& _data) 
{
    property_data += _data;
}
Info_Property_Item::~Info_Property_Item()
{

}

Info_Property_Delegate::Info_Property_Delegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}
Info_Property_Delegate::~Info_Property_Delegate()
{
}
void Info_Property_Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        QVariant variant = index.data(Qt::UserRole);
        Info_Property_Item data = variant.value<Info_Property_Item>();

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width() - 1.0);
        rect.setHeight(option.rect.height() - 1.0);

        //QPainterPath画圆角矩形
        const qreal radius =15;
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
        if (option.state.testFlag(QStyle::State_Selected)) //选中状态
        {
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(229, 241, 255));
            painter->drawPath(path);
        }
        else if (option.state.testFlag(QStyle::State_MouseOver))//鼠标划过状态
        {
            painter->setPen(QPen(Qt::green));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
        else {
            painter->setPen(QPen(Qt::gray));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }

        //绘制数据位置
        QRect NameRect = QRect(rect.left() + 10, rect.top() + 25, rect.width() - 10, 25);
        QRect dataRect = QRect(rect.left() + 150, rect.top() + 25, rect.width(), 25);
        
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Consolas", 14, QFont::Bold));
        painter->drawText(NameRect, Qt::AlignLeft, data.property_name);
        
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Consolas", 14));
        painter->drawText(dataRect, Qt::AlignLeft, data.property_data);

        painter->restore();
    }
}

QSize Info_Property_Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
#ifdef TEST_DEBUG
    //qDebug() << "sizeHint" << option.rect.width() << '\n';
#endif
    return QSize(option.rect.width(), 75);
}

Info_Detail_Widget::Info_Detail_Widget(QString& parameter, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Info_Detail_Widget();
	ui->setupUi(this);

	initData(parameter);
    info_delegate = new Info_Property_Delegate(this);
    ui->listView->setSpacing(5);
    ui->listView->setItemDelegate(info_delegate);
    ui->listView->setModel(info_model);
    ui->listView->setDragEnabled(false);
}

Info_Detail_Widget::~Info_Detail_Widget()
{
#ifdef TEST_DEBUG
    qDebug() << "~Info_Detail_Widget" << '\n';
#endif // TEST_DEBUG

	delete ui;
    delete info_model;
    delete info_delegate;
}

void Info_Detail_Widget::initData(const QString& parameter) {
#ifndef TEST_DEBUG_INFO
	
#else:
    Info data = *FST::INFO[0];
#endif
    ui->title->setFont(QFont("Consolas", 20, QFont::Bold));
    ui->title->setText("Detail about " + parameter);
	auto tmp_mp = data.GetProperties();
    Info_Property_Item tmp_IPI;
    int i = 0;
    if (tmp_mp.empty()) {
        info_model = new QStandardItemModel(1, 1);
        tmp_IPI.setProperty("NULL", "没有关于这个关键词的信息哦");
        info_model->setData(info_model->index(i++, 0), QVariant::fromValue(tmp_IPI), Qt::UserRole);
    }
    else {
        info_model = new QStandardItemModel(tmp_mp.size(), 1);
        for (auto &it : tmp_mp) {
            tmp_IPI.setProperty(QString((QChar*)(wchar_t*)(it.first)), "");
            bool fg = 0;
            for (auto& jt : it.second) {
                if (fg) tmp_IPI.addPropertyData(QString("; ")); else fg = 1;
                tmp_IPI.addPropertyData(QString((QChar*)(wchar_t*)jt, wcslen(jt)));
            }
            info_model->setData(info_model->index(i++, 0), QVariant::fromValue(tmp_IPI), Qt::UserRole);
        }
    }
}