#include <qdebug.h>
#include <QProgressDialog>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Info_Detail_Widget.h"
#include "Author_Detail_Widget.h"
#include "Hotspot_Detail_Widget.h"
#include "Author_Top_Widget.h"
#include "Info_Title_Widget.h"
#include "Help.h"
#include "pch.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->rbn_Info->setChecked(true);
	//connect(ui->tet_inputParameter, SIGNAL(returnPressed()), ui->btn_search, SIGNAL(clicked()), Qt::UniqueConnection);
}

void MainWindow::on_btn_search_clicked() {
#ifdef TEST_DEBUG
	qDebug() << "on_btn_search_clicked" << '\n';
	clock_t beg = clock();
#endif // !TEST_DEBUG
	QString parameter = ui->tet_inputParameter->toPlainText();
	if (ui->rbn_Info->isChecked()) {
		Info_Detail_Widget *tmp = new Info_Detail_Widget(parameter);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}
	else if (ui->rbn_Title->isChecked()) {
		Info_Title_Widget* tmp = new Info_Title_Widget(parameter);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}
	else if (ui->rbn_Author->isChecked()) {
		Author_Detail_Widget* tmp = new Author_Detail_Widget(parameter);
		tmp->show();
		tmp->setAttribute(Qt::WA_DeleteOnClose);
	}
	else {
	}

#ifdef TEST_DEBUG
	qDebug() << "on_btn_search_clicked" << clock() - beg << '\n';
#endif // TEST_DEBU
}

void MainWindow::on_btn_F3_clicked()
{
	Hotspot_Detail_Widget* tmp = new Hotspot_Detail_Widget();
	tmp->show();
	tmp->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_btn_F2_TopAuthor_clicked()
{
	Author_Top_Widget* tmp = new Author_Top_Widget();
	tmp->show();
	tmp->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_btn_F5_clicked()
{
	QMessageBox::information(NULL, "F5", "F5", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void MainWindow::on_btn_help_clicked()
{
	Help* tmp = new Help();
	tmp->show();
	tmp->setAttribute(Qt::WA_DeleteOnClose);
}
