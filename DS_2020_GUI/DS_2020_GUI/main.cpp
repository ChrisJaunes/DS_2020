#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Article.h"

#include "pch.h"
#include "test.h"

int main(int argc, char *argv[])
{
	FST::create_FST();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
