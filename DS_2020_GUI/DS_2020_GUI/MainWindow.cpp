#include <qdebug.h>
#include <QProgressDialog>
#include <xmlhelper.h>
#include "MainWindow.h"
#include "Author_Article_Widget.h"
#include "Article_Detail_Widget.h"

_ImportData ImportData;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	//CreateThread(0,0,(LPTHREAD_START_ROUTINE)ImportDataWrapper, L"D:\\Code\\ds_hw\\dblp.xml",0,0);
	ui->setupUi(this);
	ui->rbn_F1_Author->setChecked(true);
	connect(ui->tet_inputParameter, SIGNAL(returnPressed()), ui->btn_search, SIGNAL(clicked()), Qt::UniqueConnection);
}

void MainWindow::on_btn_search_clicked() {
	qDebug() << "on_btn_search_clicked" << '\n';
	/*if (!ImportData.isDone) {
		// 没有完成xml解析工作, 计划弹出一个processbar
		return;
	}else {
		qDebug() << "parse done" << '\n';
	}*/
    QString parameter = ui->tet_inputParameter->toPlainText();
	if (ui->rbn_F1_Author->isChecked()) {
		(new Author_Article_Widget(parameter))->show();
	}
	else {

	}
}

void MainWindow::show_ArticleOfAuthor(QString parameter) {
	qDebug() << "show_ArticleOfAuthor" << parameter << '\n';
}

void MainWindow::show_DetailOfArticle(QString parameter) {
	qDebug() << "show_DetailOfArticle" << parameter << '\n';
}