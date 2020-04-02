#include <QtWidgets/QApplication>
#include "pch.h"
#include "MainWindow.h"
#include "Article.h"
#include "test.h"
#include <Solver.h>
#include <CommUtils.h>

int main(int argc, char *argv[])
{
	FST::create_FST();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
