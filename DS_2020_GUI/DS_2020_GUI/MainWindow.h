#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	void show_ArticleOfAuthor(QString);
	void show_DetailOfArticle(QString);



private slots:
	void on_btn_search_clicked();

private:
	Ui::MainWindow* ui;
};
