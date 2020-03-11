#include <qdebug.h>
#include "MainWindow.h"
#include "Author_Article_Widget.h"
#include "Article_Detail_Widget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->rbn_F1_Author->setChecked(true);
	connect(ui->tet_inputParameter, SIGNAL(returnPressed()), ui->btn_search, SIGNAL(clicked()), Qt::UniqueConnection);
}
void MainWindow::on_btn_search_clicked() {
	qDebug() << "on_btn_search_clicked" << '\n';
    QString parameter = ui->tet_inputParameter->toPlainText();
    if (ui->rbn_F1_Author->isChecked()) show_ArticleOfAuthor(parameter);
    else;
}

void MainWindow::show_ArticleOfAuthor(QString parameter) {
	qDebug() << "show_ArticleOfAuthor" << parameter << '\n';
	(new Author_Article_Widget())->show();
}

void MainWindow::show_DetailOfArticle(QString parameter) {
	qDebug() << "show_DetailOfArticle" << parameter << '\n';
	(new Article_Detail_Widget())->show();
}