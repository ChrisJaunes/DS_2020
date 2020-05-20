#include <QtWidgets/QApplication>
#include "MainWindow.h"
#include "FSolver.h"
FSolver fsolver(DS_DBLP_Info, DS_DBLP_Author, FILE_Status::EXIST);
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
