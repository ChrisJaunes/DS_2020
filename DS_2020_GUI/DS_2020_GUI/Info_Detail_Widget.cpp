#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Info_Detail_Widget.h"
#include "Info.h"
#include "test.h"
#define STR bstr_t

Info_Property_Item::Info_Property_Item()
{

}
Info_Property_Item::Info_Property_Item(QString& _name, QString& _data)
    : property_name(_name)
    , property_data(_data)
{

}
Info_Property_Item::~Info_Property_Item()
{

}

InfoDelegate::InfoDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}
InfoDelegate::~InfoDelegate()
{
}
void InfoDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        QVariant variant = index.data(Qt::UserRole);
        Info_Property_Item data = variant.value<Info_Property_Item>();

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
        QRect NameRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 10, 20);
        QRect authorsRect = QRect(rect.left() + 100, rect.top() + 25, rect.width() - 10, 20);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 12, QFont::Bold));
        painter->drawText(NameRect, Qt::AlignLeft, data.property_name);

        painter->setPen(QPen(Qt::gray));
        painter->setFont(QFont("Times", 10));
        painter->drawText(NameRect, Qt::AlignLeft, data.property_data);

        painter->restore();
    }
}

QSize InfoDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}

Info_Detail_Widget::Info_Detail_Widget(QString& parameter, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Info_Detail_Widget();
	ui->setupUi(this);

	initData(parameter);
    info_delegate = new InfoDelegate(this);
    ui->listView->setItemDelegate(info_delegate);
    ui->listView->setModel(info_model);
    ui->listView->setDragEnabled(false);
}

Info_Detail_Widget::~Info_Detail_Widget()
{
	delete ui;
    delete info_model;
    delete info_delegate;
}

void Info_Detail_Widget::initData(const QString& parameter) {
#ifndef TEST_DEBUG
	
#else:
	Info data = Info();
	data.AddProperty("title", "abc");
	data.AddProperty("date", "20200329");
	data.AddProperty("author", "AA");
	data.AddProperty("author", "BB");
	data.AddProperty("author", "CC");
#endif
	std::map<STR, std::vector<STR>> mp = data.GetProperties();
    info_model = new QStandardItemModel(mp.size(), 1);

    int i = 0;
    for (auto &it : mp) {
        if (it.first == STR(L"titile")) {
            ui->title->setText(QString((QChar*)(wchar_t*)it.second.at(0), wcslen(it.second.at(0))));
        }
        else {
            QString tmp = "";
            for (auto &jt : it.second) {
                tmp += QString((QChar* )(wchar_t*)jt, wcslen(jt)) + QString(" ");
            }
            info_model->setData(info_model->index(i++, 0), QVariant::fromValue(Info_Property_Item(QString((QChar*)(wchar_t*)(it.first), wcslen(it.first)), tmp)), Qt::UserRole);
        }
    }

}
#undef STR