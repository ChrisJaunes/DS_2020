#include <qdebug.h>
#include "MainWindow.h"
#include "Info_Detail_Widget.h"
#include "Author_Detail_Widget.h"

#include "Hotspot_Analysis_Widget.h"
//#include <xmlhelper.h>
#include <QProgressDialog>
#include <QMessageBox>
#include "test.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->rbn_F1_Info->setChecked(true);
	connect(ui->tet_inputParameter, SIGNAL(returnPressed()), ui->btn_search, SIGNAL(clicked()), Qt::UniqueConnection);
	//ImportData.isDone = false;
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ImportDataWrapper, (LPVOID)L"D:\\Code\\ds_hw\\dblp.xml", 0, 0);
}

void MainWindow::on_btn_loadfile_clicked() {
	//ImportData.isDone = false;
	//QString filepath =ui->tet_loadfile->toPlainText();
	//LPCWSTR rfilepath = charToWChar(filepath.toLatin1());
	// D:\Code\ds_hw\dblp.xml
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ImportDataWrapper, (LPVOID)rfilepath, 0, 0);
}

void MainWindow::on_btn_search_clicked() {
#ifdef TEST_DEBUG
	qDebug() << "on_btn_search_clicked" << '\n';
	clock_t beg = clock();
	/*if (!ImportData.isDone) {
		// 没有完成xml解析工作, 弹出提示框
		QMessageBox::information(this, "Error", "XML not loaded");
		return;
	}
	else {
		qDebug() << "parse done" << '\n';
	}
	*/
#endif // !TEST_DEBUG
	QString parameter = ui->tet_inputParameter->toPlainText();
	if (ui->rbn_F1_Info->isChecked()) {
		Info_Detail_Widget *tmp = new Info_Detail_Widget(parameter);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}else if (ui->rbn_F1_Author->isChecked()) {
		Author_Detail_Widget* tmp = new Author_Detail_Widget(parameter);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}
	else if (ui->rbn_F3) {
		(new Hotspot_Analysis_Widget(parameter))->show();
	}

#ifdef TEST_DEBUG
	qDebug() << "on_btn_search_clicked" << clock() - beg << '\n';
#endif // TEST_DEBU
}