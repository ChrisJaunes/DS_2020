#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>
#include "Author_Article_Widget.h"
#include "ui_Author_Article_Widget.h"

#include "test.h"

Author_Article_Item::Author_Article_Item() 
{

}
Author_Article_Item::Author_Article_Item(Article *article)
{
    article->Gettitle(&title);
    std::vector<bstr_t>* authors_tmp;
    article->Getauthors(authors_tmp);
    authors = *authors_tmp;
    delete authors_tmp;
}
Author_Article_Item::~Author_Article_Item() 
{

}

ArticleDelegate::ArticleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}
ArticleDelegate::~ArticleDelegate()
{
}
void ArticleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid()) {
        painter->save();

        //ItemStatus status = (ItemStatus)(index.data(Qt::UserRole).toInt());
        QVariant variant = index.data(Qt::UserRole + 1);
        Author_Article_Item data = variant.value<Author_Article_Item>();

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
        QRect NameRect = QRect(rect.left() + 10, rect.top() + 10, rect.width() - 30, 20);
        QRect authorsRect = QRect(rect.left() + 10, rect.top() + 25, rect.width() - 10, 50);

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Times", 12, QFont::Bold));
        painter->drawText(NameRect, Qt::AlignLeft, tr(data.title)); 

        painter->setPen(QPen(Qt::gray));
        painter->setFont(QFont("Times", 10));
        QString authors = "authors :\n ";
        for (int i = 0; i < data.authors.size(); i++) {
            authors += tr(data.authors[i]) + tr("\n");
        }
        painter->drawText(authorsRect, Qt::AlignLeft, authors);

        painter->restore();
    }
}

QSize ArticleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(160, 200);
}

Author_Article_Widget::Author_Article_Widget(QString parameter, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Author_Article_Widget();
	ui->setupUi(this);
    
    initData(parameter);
    
    article_delegate = new ArticleDelegate(this);
    ui->listView->setItemDelegate(article_delegate);
    ui->listView->setSpacing(15);
    article_proxyModel = new QSortFilterProxyModel(ui->listView);
    article_proxyModel->setSourceModel(article_model);
    article_proxyModel->setFilterRole(Qt::UserRole);
    article_proxyModel->setDynamicSortFilter(true);
    ui->listView->setModel(article_model);
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setDragEnabled(false);
}

Author_Article_Widget::~Author_Article_Widget()
{
	delete ui;
}

void Author_Article_Widget::initData(QString& parameter) {
#ifndef TEST_DEBUG
    author = Author::getAuthorByName(bstr_t(parameter.toStdWString().c_str()));
#else
    author = *FST::AUTHORS[rand() % FST::AUTHORSN];
#endif
    auto articles = author.GetArticles().second;
    auto collaborator = author.GetCollaborators().second;
    std::map<bstr_t, char> main_collaborator;
    for (int i = min((int)collaborator.size(), 4); i >= 0; i--) {
        main_collaborator[collaborator[i].second] = char(i + 48);
    }

    article_model = new QStandardItemModel(articles.size(), 1);
    for (int i = 0; i < articles.size(); i++) {
        Author_Article_Item article_item;
#ifndef TEST_DEBUG
        article_item(Article::getArticles(articles[i]));
#else
        article_item = Author_Article_Item(FST::ARTICLES[rand() % FST::ARTICLESN]);
#endif
        QString flag = "";
        for (auto c : article_item.authors) {
            if (main_collaborator.count(c)) flag += main_collaborator[c];
        }

        article_model->setData(article_model->index(i, 0), flag, Qt::UserRole);  // 单一存取
        article_model->setData(article_model->index(i, 0), QVariant::fromValue(article_item), Qt::UserRole + 1); //整体存取
    }

    ui->allBtn->setText(tr("all"));
    if ((int)collaborator.size() >= 1) ui->collaboratorBtn0->setText(tr(collaborator[0].second));
    if ((int)collaborator.size() >= 2) ui->collaboratorBtn1->setText(tr(collaborator[1].second));
    if ((int)collaborator.size() >= 3) ui->collaboratorBtn2->setText(tr(collaborator[2].second));
    if ((int)collaborator.size() >= 4) ui->collaboratorBtn3->setText(tr(collaborator[3].second));
    if ((int)collaborator.size() >= 5) ui->collaboratorBtn4->setText(tr(collaborator[4].second));
}
void Author_Article_Widget::on_allBtn_clicked()
{
}
void Author_Article_Widget::on_collaboratorBtn0_clicked()
{
}
void Author_Article_Widget::on_collaboratorBtn1_clicked()
{
}
void Author_Article_Widget::on_collaboratorBtn2_clicked()
{
}
void Author_Article_Widget::on_collaboratorBtn3_clicked()
{
}
void Author_Article_Widget::on_collaboratorBtn4_clicked()
{
}
