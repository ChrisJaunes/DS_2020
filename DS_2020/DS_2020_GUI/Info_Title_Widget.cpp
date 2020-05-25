#include <QPainter>
#include <qmessagebox.h>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Info_Title_Widget.h"
#include "Info_Detail_Widget.h"
#include "ui_Info_Title_Widget.h"
#include "config.h"
#include "Info.h"
#include "test_GUI.h"

Info_Title_Item::Info_Title_Item()
{
}

Info_Title_Item::Info_Title_Item(QString& _title)
    : title(_title)
{
}

Info_Title_Item::~Info_Title_Item()
{
}

Info_Title_Delegate::Info_Title_Delegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}
Info_Title_Delegate::~Info_Title_Delegate()
{
}
void Info_Title_Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        QVariant variant = index.data(Qt::UserRole);
        Info_Title_Item data = variant.value<Info_Title_Item>();

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
        painter->setFont(QFont("Consolas", 8, QFont::Normal));
        painter->drawText(nameRect, Qt::AlignLeft, data.title);

        painter->restore();
    }
}

QSize Info_Title_Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}

Info_Title_Widget::Info_Title_Widget(QString& parameter, QWidget* parent)
    : QWidget(parent)
{
#ifdef TEST_DEBUG_INFO_TITLE
    clock_t beg = clock();
#endif

    ui = new Ui::Info_Title_Widget();
    ui->setupUi(this);

    initData(parameter);

    title_delegate = new Info_Title_Delegate(this);
    ui->listView->setItemDelegate(title_delegate);
    ui->listView->setModel(title_model);
    ui->listView->setSpacing(5);
    ui->listView->setDragEnabled(false);

	connect(ui->title_to_info, SIGNAL(clicked()), this, SLOT(on_btn_title_to_info_clicked()));

#ifdef TEST_DEBUG
    qDebug() << "Hotspot_Analysis_Widget" << clock() - beg << '\n';
#endif
}

Info_Title_Widget::~Info_Title_Widget()
{
    delete ui;
    delete title_model;
    delete title_delegate;
}


void Info_Title_Widget::initData(const QString& parameter)
{

#ifndef TEST_DEBUG_INFO_TITLE

#else
	titles.clear();
	bool data = fsolver.F4_KeywordSearch(parameter.toStdString(), titles);
    ui->label->setFont(QFont("Consolas", 20, QFont::Bold));
	ui->label->setText("Details");
	if (data == false) {
		title_model = new QStandardItemModel(2, 1);
		title_model->setData(title_model->index(0, 0), QVariant::fromValue(Info_Title_Item(QString::fromStdString("ERROR"))), Qt::UserRole);
		title_model->setData(title_model->index(1, 0), QVariant::fromValue(Info_Title_Item(QString::fromStdString("No result found! "))), Qt::UserRole);
		return;
	}
	title_model = new QStandardItemModel(titles.size(),1);
	for (int i = 0; i < titles.size(); i++) {
		title_model->setData(title_model->index(i, 0), QVariant::fromValue(Info_Title_Item(QString::fromStdString(titles[i]))), Qt::UserRole);
	}
#endif
    qDebug() << " Hotspot_Detail_Widget" << "\n";
}

void Info_Title_Widget::on_btn_title_to_info_clicked() {
	//未找到title时的错误提醒
	if (titles.size() == 0)
		QMessageBox::information(this, "WARNNINIG", "No title found!");
	//将选中的title跳转到对应info界面
	else {
		QModelIndex currIndex = ui->listView->currentIndex();
		QString p = QString::fromStdString(titles[currIndex.row()]);
		Info_Detail_Widget *tmp = new Info_Detail_Widget(p);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}
}