/* Program name: velocityText.h
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

#ifndef VELOCITYTEXT_H
#define VELOCITYTEXT_H

#include <QGraphicsTextItem>

class VelocityText : public QGraphicsTextItem
{
public:
	void update(void);
		//update the vText display with current info
	VelocityText(int&, double&, QGraphicsItem* = nullptr);
		//Constructor create a QTGraphicsText item, connect angle and magnitude pointers to their memory locations and display initial message
	~VelocityText(void);
		//Destructor

private:
	int *angle;			//Points to the angle member of game object
	double *magnitude;	//Points to the magnitude member of game object
};

#endif //VELOCITYTEXT_H