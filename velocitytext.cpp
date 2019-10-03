/* Program name: velocityText.cpp
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: a text object that continually updates to display the megnitude and angle the launcher before ball is launched
	================================================================
	FUNCTIONS:
	+ void update(void) - update the vText display with current info
	+ VelocityText(int&, double&, QGraphicsItem* = nullptr) - Constructor create a QTGraphicsText item, connect angle and magnitude pointers to their memory locations and display initial message
	+ ~VelocityText(void) - Destructor
	============================================================================
	VARIABLES:
	- int *angle - Points to the angle member of game object
	- double *magnitude - Points to the magnitude member of game object
*/

#include <Qfont>
#include <QGraphicsTextItem>
#include "velocitytext.h"

#define MAX_VELOCITY 50.0

//update the vText display with current info
void VelocityText::update()
{
	//redraw the text
	setPlainText(QString("Launch with ") + QString::number(100 * *magnitude / MAX_VELOCITY) + QString("% power at ") + QString::number(360-*angle) + QString(" degrees.")); //e.g. Launch ball: 50% power at 128 degrees.
}

//Constructor create a QTGraphicsText item, connect angle and magnitude pointers to their memory locations and display initial message with formatting
VelocityText::VelocityText(int &angleAddr, double &magnitudeAddr, QGraphicsItem * parent) : QGraphicsTextItem(parent)
{
	magnitude = &magnitudeAddr;
	angle = &angleAddr;

	//draw the text with formatting
	setPlainText(QString("Launch with ") + QString::number(100 * *magnitude / MAX_VELOCITY) + QString("% power at ") + QString::number(*angle) + QString(" degrees.")); //e.g. Launch ball: 50% power at 128 degrees.
	setDefaultTextColor(Qt::blue);
	setFont(QFont("time", 16));
	setPos(150, 0);
	setZValue(100);
}

//Destructor
VelocityText::~VelocityText()
{
	angle = nullptr;
	magnitude = nullptr;
	delete angle;
	delete magnitude;
}
