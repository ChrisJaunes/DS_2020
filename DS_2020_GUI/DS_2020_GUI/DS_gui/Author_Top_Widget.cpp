#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Author_Top_Widget.h"
#include "ui_Author_Top_Widget.h"
#include "test.h"

#define W int
Author_Top_Item::Author_Top_Item()
{
}

Author_Top_Item::Author_Top_Item(QString& _name, W& _number)
    : name(_name)
    , number(_number)
{
}

Author_Top_Item::~Author_Top_Item()
{
}

AuthorTopDelegate::AuthorTopDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}
AuthorTopDelegate::~AuthorTopDelegate()
{
}
void AuthorTopDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        QVariant variant = index.data(Qt::UserRole);
        Author_Top_Item data = variant.value<Author_Top_Item>();

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
        QRect nameRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 10, 25);
        QRect numberRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 20, 25);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Consolas", 14, QFont::Bold));
        painter->drawText(nameRect, Qt::AlignLeft, data.name);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 14));
        painter->drawText(numberRect, Qt::AlignRight, QString::number(data.number));

        painter->restore();
    }
}

QSize AuthorTopDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}

Author_Top_Widget::Author_Top_Widget(QWidget* parent)
    : QWidget(parent)
{
#ifdef TEST_DEBUG
    clock_t beg = clock();
#endif

    ui = new Ui::Author_Top_Widget();
    ui->setupUi(this);

    initData();

    top_delegate = new AuthorTopDelegate(this);
    ui->listView->setItemDelegate(top_delegate);
    ui->listView->setModel(top_model);
    ui->listView->setSpacing(5);
    ui->listView->setDragEnabled(false);

#ifdef TEST_DEBUG
    qDebug() << "Hotspot_Analysis_Widget" << clock() - beg << '\n';
#endif
}

Author_Top_Widget::~Author_Top_Widget()
{
    delete ui;
    delete top_model;
    delete top_delegate;
}


void Author_Top_Widget::initData()
{

#ifndef TEST_DEBUG_TOP


#else
    top_model = new QStandardItemModel(100, 1);
    int i = 0;
    for (int j = 0; j < 100; j++) {
        top_model->setData(top_model->index(i, 0), QVariant::fromValue(Author_Top_Item(QString::number(j), j)), Qt::UserRole);
        i++;
    }
#endif
    qDebug() << " Hotspot_Detail_Widget" << "\n";
}

#undef W