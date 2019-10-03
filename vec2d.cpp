/* Program name: vec2d.cpp
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: store a vector in x and y coordinates
	================================================================
	FUNCTIONS:
	+ void setX(double) - set value of x
	+ void setY(double) - set value of y
	+ void setXY(double, double) - set value of x and y
	+ void getVectorComponents(double&, double&) - return values of x and y
	+ Vec2D() - Constructor, initial vector [0,0]
	+ ~Vec2D() - Destructor
	============================================================================
	VARIABLES:
	- double x - x component of the vector
	- double y - y component of the vector
*/

#include "vec2d.h"

//set value of x
void Vec2D::setX(double newX)
{
	x = newX;
}

//set value of y
void Vec2D::setY(double newY)
{
	y = newY;
}

//set value of x and y
void Vec2D::setXY(double newX, double newY)
{
	x = newX;
	y = newY;
}

//return values of x and y
void Vec2D::getVectorComponents(double& retX, double& retY)
{
	retX = x;
	retY = y;
}

//Constructor, initial vector [0,0]
Vec2D::Vec2D()
{
	x = 0;
	y = 0;
}

//Destructor
Vec2D::~Vec2D()
{
}
