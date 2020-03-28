#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Article.h"

#include "pch.h"
#include "test.h"
#include <Solver.h>
#include <CommUtils.h>

int main(int argc, char *argv[])
{
	FST::create_FST();
	F3Solver::ImportFromFile(L"D:\\Code\\ds_hw\\new\\new1\\DS_2020\\Database\\F3\\F3_articles.txt");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
