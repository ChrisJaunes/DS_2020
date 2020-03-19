#include <qdebug.h>
#include "MainWindow.h"
#include "Author_Article_Widget.h"
#include "Article_Detail_Widget.h"
#include <xmlhelper.h>
#include <QProgressDialog>
#include <QMessageBox>
_ImportData ImportData;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->rbn_F1_Author->setChecked(true);
	connect(ui->tet_inputParameter, SIGNAL(returnPressed()), ui->btn_search, SIGNAL(clicked()), Qt::UniqueConnection);
}

void MainWindow::on_btn_loadfile_clicked() {
	ImportData.isDone = false;
	QString filepath =ui->tet_loadfile->toPlainText();
	LPCWSTR rfilepath = charToWChar(filepath.toLatin1());
	// D:\Code\ds_hw\dblp.xml
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ImportDataWrapper, (LPVOID)rfilepath, 0, 0);
}

void MainWindow::on_btn_search_clicked() {
	qDebug() << "on_btn_search_clicked" << '\n';
	if (!ImportData.isDone) {
		// 没有完成xml解析工作, 弹出提示框
		QMessageBox::information(this, "Error", "XML not loaded");
		return;
	}
	else {
		qDebug() << "parse done" << '\n';
	}

	QString parameter = ui->tet_inputParameter->toPlainText();
	if (ui->rbn_F1_Author->isChecked()) {
		(new Author_Article_Widget(parameter))->show();
	}
	else;
}

void MainWindow::show_ArticleOfAuthor(QString parameter) {
	qDebug() << "show_ArticleOfAuthor" << parameter << '\n';
	//(new Author_Article_Widget())->show();
}

void MainWindow::show_DetailOfArticle(QString parameter) {
	qDebug() << "show_DetailOfArticle" << parameter << '\n';
	//(new Article_Detail_Widget())->show();
}