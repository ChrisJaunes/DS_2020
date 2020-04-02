#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Hotspot_Detail_Widget.h"
#include "ui_Hotspot_Detail_Widget.h"
#include "test.h"
#include "Solver.h"
#define W int
Hotspot_Property_Item::Hotspot_Property_Item()
{
}

Hotspot_Property_Item::Hotspot_Property_Item(QString _hotspot_word, W _hotspot_number)
    : hotspot_word(_hotspot_word)
    , hotspot_number(_hotspot_number)
{

}

Hotspot_Property_Item::~Hotspot_Property_Item()
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
        Hotspot_Property_Item data = variant.value<Hotspot_Property_Item>();

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
        QRect wordRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 10, 20);
        QRect numberRect = QRect(rect.left() + 50, rect.top() + 10, rect.width() - 60, 20);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 14, QFont::Bold));
        painter->drawText(wordRect, Qt::AlignLeft, data.hotspot_word);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 14));
        painter->drawText(numberRect, Qt::AlignRight, QString::number(data.hotspot_number));

        painter->restore();
    }
}

QSize HotspotDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 50);
}

Hotspot_Detail_Widget::Hotspot_Detail_Widget(QWidget* parent)
    : QWidget(parent)
{
#ifdef TEST_DEBUG
    clock_t beg = clock();
#endif

    ui = new Ui::Hotspot_Detail_Widget();
    ui->setupUi(this);

    initData();
    hotspot_delegate = new HotspotDelegate(this);
    ui->listView->setItemDelegate(hotspot_delegate);
    ui->listView->setSpacing(5);
    hotspot_proxyModel = new QSortFilterProxyModel(ui->listView);
    hotspot_proxyModel->setSourceModel(hotspot_model);
    hotspot_proxyModel->setFilterRole(Qt::UserRole);
    hotspot_proxyModel->setDynamicSortFilter(true);
    ui->listView->setModel(hotspot_proxyModel);
    ui->listView->setDragEnabled(false);
    hotspot_proxyModel->setFilterFixedString("2020|articles");

#ifdef TEST_DEBUG
    qDebug() << "Hotspot_Analysis_Widget" << clock() - beg << '\n';
#endif
}

Hotspot_Detail_Widget::~Hotspot_Detail_Widget()
{
    delete ui;
}


void Hotspot_Detail_Widget::initData()
{

#ifndef TEST_DEBUG_HOTSPOT
    QStringList years, types;
    int i = 0;
    std::map < STR, std::map<STR, ULONG64> > hotspot[HOTSPOTTYPEN];
    for (int j = 0; j < HOTSPOTTYPEN; j++) {
        hotspot[j] = F3Solver::ImportFromFile(L"database/f3/F3_" + HOTSPOTTYPE[j] + L".txt");
        types.push_back(QString(HOTSPOTTYPE[j]));
        for (auto it : hotspot[j]) i += it.second.size();
}
    hotspot_model = new QStandardItemModel(i, 1);
    i = 0;
    
    for (int j = 0; j < HOTSPOTTYPEN; j++) {
        for (auto it : hotspot[j]) {
            years.push_back(QString(it.first));
            for (auto jt : it.second) {
                hotspot_model->setData(hotspot_model->index(i, 0), QString(it.first) + "|" + QString(HOTSPOTTYPE[j]), Qt::UserRole);
                hotspot_model->setData(hotspot_model->index(i, 0), QVariant::fromValue(Hotspot_Property_Item(QString(jt.first), jt.second)), Qt::UserRole + 1); //整体存取
                i++;
            }
        }
    }
    std::sort(years.begin(), years.end());
    years.erase(std::unique(years.begin(), years.end()), years.end());
    std::reverse(years.begin(), years.end());
    ui->cbx_year->addItems(years);
    ui->cbx_type->addItems(types);
    
#else
    QVector<Hotspot_Property_Item> data;
    data.push_back(Hotspot_Property_Item(QString("A"), 10));
    data.push_back(Hotspot_Property_Item(QString("B"), 20));
    data.push_back(Hotspot_Property_Item(QString("C"), 30));
    data.push_back(Hotspot_Property_Item(QString("D"), 40));
    data.push_back(Hotspot_Property_Item(QString("E"), 50));
    data.push_back(Hotspot_Property_Item(QString("F"), 60));
    data.push_back(Hotspot_Property_Item(QString("G"), 70));
    data.push_back(Hotspot_Property_Item(QString("H"), 80));
    data.push_back(Hotspot_Property_Item(QString("I"), 90));
    data.push_back(Hotspot_Property_Item(QString("J"), 00));
    hotspot_model = new QStandardItemModel(data.size(), 1);
    for (int i = 0; i < data.size(); i++) {
        hotspot_model->setData(hotspot_model->index(i, 0), QVariant::fromValue(data[i]), Qt::UserRole + 1); //整体存取
    }
#endif
    qDebug() << " Hotspot_Detail_Widget" << "\n";
}

void Hotspot_Detail_Widget::on_btn_ok_clicked()
{
    QString year = ui->cbx_year->currentText();
    QString type = ui->cbx_type->currentText();
    hotspot_proxyModel->setFilterFixedString(year + "|" + type);
}


#undef W