#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Cliques_Widget.h"
#include "ui_Cliques_Widget.h"
#include "config.h"
#include "test_GUI.h"

#define STR QString

Cliques_Item::Cliques_Item()
{
}

Cliques_Item::Cliques_Item(MYW _order, MYW _number)
	: order(_order)
	, number(_number)
{
}

Cliques_Item::~Cliques_Item()
{
}

CliquesCountDelegate::CliquesCountDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}
CliquesCountDelegate::~CliquesCountDelegate()
{
}
void CliquesCountDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid()) {
		painter->save();

		//QVariant variant = index.data(Qt::UserRole);
		//Cliques_Item data = variant.value<Cliques_Item>();

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

		//选中状态
		if (option.state.testFlag(QStyle::State_Selected))
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

		QString text = index.model()->data(index, Qt::DisplayRole).toString();
		painter->setPen(QPen(Qt::black));
		painter->setFont(QFont("Times", 18));
		//获取项风格设置  
		QStyleOptionViewItem myOption = option;
		myOption.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;
		//绘制文本  
		QApplication::style()->drawItemText(painter, myOption.rect, myOption.displayAlignment, QApplication::palette(), true, text);

		/*
		//绘制数据位置
		QRect nameRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 10, 25);
		QRect numberRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 20, 25);

		painter->setPen(QPen(Qt::black));
		painter->setFont(QFont("Times", 14, QFont::Bold));
		painter->drawText(nameRect, Qt::AlignCenter, QString::number(data.order));

		painter->setPen(QPen(Qt::black));
		painter->setFont(QFont("Times", 14));
		painter->drawText(numberRect, Qt::AlignRight, QString::number(data.number));
		*/
		painter->restore();
	}

}

QSize CliquesCountDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), 50);
}

Cliques_Widget::Cliques_Widget(QWidget* parent)
	: QWidget(parent)
{
#ifdef TEST_DEBUG
	clock_t beg = clock();
#endif

	ui = new Ui::Cliques_Widget();
	ui->setupUi(this);

	initData();

	cliques_delegate = new CliquesCountDelegate(this);
	ui->f5_tableView->setItemDelegate(cliques_delegate);
	ui->f5_tableView->setModel(cliques_model);
	ui->f5_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	//ui->f5_tableView->setSpacing(5);
	ui->f5_tableView->setDragEnabled(false);

	//列宽自适应
	//ui->f5_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//手动设置列宽
	ui->f5_tableView->setColumnWidth(0, 180);
	ui->f5_tableView->setColumnWidth(1, 650);
	//修改行高
	ui->f5_tableView->verticalHeader()->setDefaultSectionSize(60);
	//隐藏行号
	ui->f5_tableView->verticalHeader()->hide();
	//设置表头样式
	ui->f5_tableView->horizontalHeader()->setStyleSheet("\
			QHeaderView::section:first  {\
					color:black;height: 30px;\
					font: 16pt \"微软雅黑\";\
			}\
			QHeaderView::section:middle  {\
					color:black;height: 30px;\
					font: 16pt \"微软雅黑\";\
			}\
			QHeaderView::section:last  {\
					color:black;height: 30px;\
					font: 16pt \"微软雅黑\";\
			}");

#ifdef TEST_DEBUG
	qDebug() << "Cliques_Widget" << clock() - beg << '\n';
#endif
}

Cliques_Widget::~Cliques_Widget()
{
	delete ui;
	delete cliques_model;
	delete cliques_delegate;
}


void Cliques_Widget::initData()
{

#ifdef TEST_DEBUG_TOP


#else
	cliques_model = new QStandardItemModel();
	cliques_model->setColumnCount(2);
	cliques_model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("完全子图的阶数"));
	cliques_model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("数量"));

	int i = 0;
	//std::map<W, W> f5_result = fsolver.F5_getResult();
	std::map<MYW, MYSTR> f5_result = fsolver.F5_getResult_2();

	//QVector<Cliques_Item> data;
	for (auto it : f5_result)
	{
		QStandardItem *itemID1 = new QStandardItem(QString("%0").arg(it.first));
		//QStandardItem *itemID2 = new QStandardItem(QString("%0").arg(it.second));
		QString text = QString((QChar*)(wchar_t*)it.second, wcslen(it.second));
		if (wcslen(it.second) > 40) {
			text.insert(wcslen(it.second) / 2, "\n");
		}
		QStandardItem *itemID2 = new QStandardItem(text);
		cliques_model->setItem(i, 0, itemID1);
		cliques_model->setItem(i, 1, itemID2);
		//cliques_model->setData(cliques_model->index(i, 0), QVariant::fromValue(Cliques_Item(it.first, it.second)), Qt::UserRole + 1);
		i++;
	}
	/*
	for (W i = 0; i < 100; i++)
	{
		for (W j = 0; j < 2; j++)
		{
		QStandardItem *itemID = new QStandardItem(QString("%0").arg(i));
		cliques_model->setItem(i, j, itemID);//每次都要重新new QStandardItem();
		}
	}
	*/
#endif
	qDebug() << " Cliques_Widget" << "\n";
}

#undef W