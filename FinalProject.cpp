//This file only exists because a QMainWindow object is required to exist for QT to properly compile the Qtimer... I don't know why
#include "FinalProject.h"

FinalProject::FinalProject(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}
