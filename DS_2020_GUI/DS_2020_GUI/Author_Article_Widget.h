#pragma once

#include <QWidget>
namespace Ui { class Author_Article_Widget; };

class Author_Article_Widget : public QWidget
{
	Q_OBJECT

public:
	Author_Article_Widget(QWidget *parent = Q_NULLPTR);
	~Author_Article_Widget();

private:
	Ui::Author_Article_Widget *ui;
};
