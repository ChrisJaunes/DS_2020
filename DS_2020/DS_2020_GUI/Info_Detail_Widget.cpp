#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include <QListView>
#include <QTabWidget>
#include "Info_Detail_Widget.h"
#include "ui_Info_Detail_Widget.h"
#include "config.h"
#include "Info.h"
#include "test_GUI.h"

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
        QRect dataRect = QRect(rect.left() + 150, rect.top() + 25, rect.width(), 75);
        
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
}

Info_Detail_Widget::~Info_Detail_Widget()
{
#ifdef TEST_DEBUG
    qDebug() << "~Info_Detail_Widget" << '\n';
#endif // TEST_DEBUG

	delete ui;
    for (auto it : mvc) {
        delete it.first;
        delete it.second;
    }
}

void Info_Detail_Widget::initData(const QString& parameter) {
#ifdef TEST_DEBUG_GUI_INFO
    Info data = *FST::INFO[0];
#else:
    auto data = fsolver.F1_getInfoByTitle((MYSTR)parameter.toStdWString().c_str());
#endif
    int font_size = min(20, (int)sqrt(600 * 10 / (parameter.size() + 10)));
    ui->title->setFont(QFont("Consolas", font_size, QFont::Bold));
    ui->title->setText("Detail About " + parameter);
    //ui->title->setGeometry(QRect(100, 30, 600, 30 * 4));
    //ui->title->adjustSize();
    ui->title->setWordWrap(true);

    if (data.first == false) {
        QLabel* label = new QLabel();
        label->setGeometry(QRect(100, 30, 600, 30 * 4));
        label ->setStyleSheet(QString::fromUtf8("border-image:url();\n""background-color:rgba(244,244,244,100)"));
        label->setFont(QFont("Consolas", 20, QFont::Bold));
        label->setText("no article");
        ui->tabWidget->addTab(label, QIcon(":/DS_2020_GUI/image/delete.png"), QString("error"));
        return;
    }
    auto info_vec = data.second;
    for (auto info : info_vec) {
        auto tmp_mp = info.GetProperties();
        int i = 0;
        for (auto& it : tmp_mp) {
            for (auto& jt : it.second) {
                ++i;
            }
        }
        QStandardItemModel* info_model = new QStandardItemModel(i, 1);
        Info_Property_Delegate* info_delegate = new Info_Property_Delegate(this);
        mvc.push_back(std::make_pair(info_model, info_delegate));
        QListView* listView = new QListView();
        listView->setSpacing(5);
        listView->setItemDelegate(info_delegate);
        listView->setModel(info_model);
        listView->setDragEnabled(false);
        listView->setObjectName(QString::fromUtf8("listView"));
        //listView->setGeometry(QRect(100, 100, 600, 420));
        listView->setStyleSheet(QString::fromUtf8("border-image:url();\n""background-color:rgba(244,244,244,100)"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tabWidget->addTab(listView, QIcon(":/DS_2020_GUI/image/test.png"), parameter);

        Info_Property_Item tmp_IPI;
        i = 0;
        for (auto& it : tmp_mp) {
            for (auto& jt : it.second) {
                QString text = QString((QChar*)(wchar_t*)jt, wcslen(jt));
                if (wcslen(jt) > 30) {
                    text.insert(30, "\n");
                }
                tmp_IPI.setProperty(QString((QChar*)(wchar_t*)it.first, wcslen(it.first)), text);
                info_model->setData(info_model->index(i++, 0), QVariant::fromValue(tmp_IPI), Qt::UserRole);
            }
        }
    }
}