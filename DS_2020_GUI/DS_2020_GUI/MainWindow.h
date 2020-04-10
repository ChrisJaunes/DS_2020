#pragma once

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindow; };
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private slots:
	void on_btn_search_clicked();
	void on_btn_F3_clicked();
	void on_btn_F2_TopAuthor_clicked();
	void on_btn_F5_clicked();
	void on_btn_help_clicked();

private:
	Ui::MainWindow* ui;
};
