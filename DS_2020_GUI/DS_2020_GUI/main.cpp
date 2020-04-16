#include <QtWidgets/QApplication>
#include "pch.h"
#include "MainWindow.h"

#include "test.h"

int main(int argc, char *argv[])
{
	FST::tmain();
	//FST::test();
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
