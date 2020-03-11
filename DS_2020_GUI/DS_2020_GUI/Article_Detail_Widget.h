#pragma once

#include <QWidget>
#include "ui_Article_Detail_Widget.h"

class Article_Detail_Widget : public QWidget
{
	Q_OBJECT

public:
	Article_Detail_Widget(QWidget *parent = Q_NULLPTR);
	~Article_Detail_Widget();

private:
	Ui::Article_Detail_Widget ui;
};
