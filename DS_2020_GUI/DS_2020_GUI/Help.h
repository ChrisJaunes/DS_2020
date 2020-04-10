#pragma once

#include <QWidget>
#include <QString>
namespace Ui { class Help; };

class Help : public QWidget
{
	Q_OBJECT

public:
	Help(QWidget *parent = Q_NULLPTR);
	~Help();

private:
	Ui::Help *ui;
};
