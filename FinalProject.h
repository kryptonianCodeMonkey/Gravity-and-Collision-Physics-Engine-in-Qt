//This file only exists because a QMainWindow object is required to exist for QT to properly compile the Qtimer... I don't know why
#ifndef FINALPROJECT_H
#define FINALPROJECT_H

#include <QtWidgets/QMainWindow>
#include "ui_FinalProject.h"

class FinalProject : public QMainWindow
{
	Q_OBJECT

public:
	FinalProject(QWidget *parent = Q_NULLPTR);

private:
	Ui::FinalProjectClass ui;
};

#endif