#include "Author_Article_Widget.h"
#include "ui_Author_Article_Widget.h"

Author_Article_Widget::Author_Article_Widget(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::Author_Article_Widget();
	ui->setupUi(this);
}

Author_Article_Widget::~Author_Article_Widget()
{
	delete ui;
}
