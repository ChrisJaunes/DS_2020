#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include "Help.h"
#include "ui_Help.h"
#include "config.h"


Help::Help(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Help();
	ui->setupUi(this);
    QFile file(DS_HELP_FILE);
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
