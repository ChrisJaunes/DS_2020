#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Author_Detail_Widget.h"
#include "ui_Author_Detail_Widget.h"

#include "test.h"

Author_Property_Item::Author_Property_Item()
{

}
Author_Property_Item::Author_Property_Item(QString& _title, QString& _authors) 
    : title(_title)
    , authors(_authors)
{

}
Author_Property_Item::~Author_Property_Item()
{

}
void Author_Property_Item::addAuthor(const QString& _author)
{
    authors += _author;
}

AuthorDelegate::AuthorDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}
AuthorDelegate::~AuthorDelegate()
{
}
void AuthorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        //ItemStatus status = (ItemStatus)(index.data(Qt::UserRole).toInt());
        QVariant variant = index.data(Qt::UserRole + 1);
        Author_Property_Item data = variant.value<Author_Property_Item>();

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
        QRect titleRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 30, 20);
        QRect authorsRect = QRect(rect.left() + 30, rect.top() + 30, rect.width() - 10, 200);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 14, QFont::Bold));
        painter->drawText(titleRect, Qt::AlignLeft, data.title);

        painter->setPen(QPen(Qt::gray));
        painter->setFont(QFont("Times", 12));
        painter->drawText(authorsRect, Qt::AlignLeft, data.authors);

        painter->restore();
    }
}

QSize AuthorDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(200, 300);
}

Author_Detail_Widget::Author_Detail_Widget(QString parameter, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Author_Detail_Widget();
	ui->setupUi(this);
    
    initData(parameter);
    
    author_delegate = new AuthorDelegate(this);
    ui->listView->setItemDelegate(author_delegate);
    ui->listView->setSpacing(15);
    author_proxyModel = new QSortFilterProxyModel(ui->listView);
    author_proxyModel->setSourceModel(author_model);
    author_proxyModel->setFilterRole(Qt::UserRole);
    author_proxyModel->setDynamicSortFilter(true);
    ui->listView->setModel(author_proxyModel);
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setDragEnabled(false);
}

Author_Detail_Widget::~Author_Detail_Widget()
{
	delete ui;
    delete author_model;
    delete author_delegate;
    delete author_proxyModel;
}

void Author_Detail_Widget::initData(QString& parameter) {
#ifndef TEST_DEBUG
    author = Author::getAuthorByName(bstr_t(parameter.toStdWString().c_str()));
#else
    author = *FST::AUTHORS[0];
#endif
    QString tmp_STR;
    auto collaborator = author.GetCollaborators().second;
    std::map<QString, char> main_collaborator;
    ui->allBtn->setText(tr("all"));
    for (int i = min((int)collaborator.size(), 4); i >= 0; i--) {
        tmp_STR = QString((QChar*)(wchar_t*)collaborator[i].second, wcslen(collaborator[i].second));
        main_collaborator[tmp_STR] = char(i + 48);
        if (i == 0) ui->collaboratorBtn0->setText(tmp_STR);
        if (i == 1) ui->collaboratorBtn1->setText(tmp_STR);
        if (i == 2) ui->collaboratorBtn2->setText(tmp_STR);
        if (i == 3) ui->collaboratorBtn3->setText(tmp_STR);
        if (i == 4) ui->collaboratorBtn4->setText(tmp_STR);
    }

    auto articles = author.GetArticles().second;
    author_model = new QStandardItemModel(articles.size(), 1);
    Author_Property_Item tmp_API;
    QString tmp_FG;
    int i = 0;
    for (auto &it : articles) {
        tmp_FG = "";
        tmp_API = Author_Property_Item(QString((QChar*)(wchar_t*)it.first, wcslen(it.first)), QString(""));
        bool fg = 0;
        for (auto &jt : it.second) {
            tmp_STR = QString((QChar*)(wchar_t*)jt, wcslen(jt));
            if (fg) tmp_API.addAuthor("\n"); else fg = 1;
            tmp_API.addAuthor(tmp_STR);
            if (main_collaborator.count(tmp_STR)) tmp_FG += main_collaborator[tmp_STR];
        }
        author_model->setData(author_model->index(i, 0), tmp_FG, Qt::UserRole);  // 单一存取
        author_model->setData(author_model->index(i, 0), QVariant::fromValue(tmp_API), Qt::UserRole + 1); //整体存取
        i++;
    }


}
void Author_Detail_Widget::on_allBtn_clicked()
{
    author_proxyModel->setFilterFixedString(QString());
}
void Author_Detail_Widget::on_collaboratorBtn0_clicked()
{
    author_proxyModel->setFilterFixedString(QString("0"));
}
void Author_Detail_Widget::on_collaboratorBtn1_clicked()
{
    author_proxyModel->setFilterFixedString(QString("1"));
}
void Author_Detail_Widget::on_collaboratorBtn2_clicked()
{
    author_proxyModel->setFilterFixedString(QString("2"));
}
void Author_Detail_Widget::on_collaboratorBtn3_clicked()
{
    author_proxyModel->setFilterFixedString(QString("3"));
}
void Author_Detail_Widget::on_collaboratorBtn4_clicked()
{
    author_proxyModel->setFilterFixedString(QString("4"));
}
