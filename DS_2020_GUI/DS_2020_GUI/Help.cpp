#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include "Help.h"
#include "ui_Help.h"


Help::Help(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Help();
	ui->setupUi(this);
    QFile file("database/help.txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream textStream(&file);
        textStream.setCodec(QTextCodec::codecForName("utf-8"));
        while (!textStream.atEnd()) {
            ui->txb_help->append(textStream.readLine());
        }
    }
    file.close();
}

Help::~Help()
{
	delete ui;
}
