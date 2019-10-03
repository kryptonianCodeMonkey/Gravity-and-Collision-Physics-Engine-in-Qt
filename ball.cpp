/* Program name: ball.cpp
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: Simulate a bounding ball object
	====================================================================
	FUNCTIONS:
	+ void collide(Vec2D, QGraphicsItem*) - Handle collision with another object
	+ void setInvMass(double) - Set the inverse mass (>=0) of the ball
	+ void setVelocity(double, double) - Set the velocity of the ball
	+ Vec2D getVelocity() const - return velocity vector
	+ double getRadius() const - return the radius of the ball
	+ void move(double) - Move the ball along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
	+ Ball(double = 0, double = 0, double = 10, double = 0) - Constructor - Pass coordinates and diameter of circle to QGraphicsEllipseItem constructor and set invMass
	+ ~Ball() - Destructor
	- void gravity(double) - Adjust velocity due to acceleration of gravity
	====================================================================
	VARAIBLES:
	- Vec2D velocity - represent the x and y components of the ball's velocity
	- double inverseMass - represent the inverse mass (mass^(-1)) of an object (to be used for moveable object collision)
	- double radius - radius of the ball
*/

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QString>
#include <iostream>
#include "block.h"
#include "ball.h"
#include "game.h"

extern Game *game; //Needed at some point, but can't remember if I still use... Deletion TBD
# define PI 3.14159265358979323846

//Handle collision with another object
void Ball::collide(Vec2D normal, QGraphicsItem *collidingItem)
{
	double xVelocity, yVelocity;	//the x and y components of ball velocity
	double xNormal, yNormal;		//the x and y components of collision normal
	double flippedVelocityAngle;	//the angle of the ball's velocity flipped 180 degrees as first step of collision reflection
	double normalAngle;				//the angle of the collision normal

	//get the data for the velocity and normal vector components
	velocity.getVectorComponents(xVelocity, yVelocity);
	normal.getVectorComponents(xNormal, yNormal);

	//determine angles of flipped velocity and collision normal
	if (xVelocity < 0)
		flippedVelocityAngle = atan(yVelocity / xVelocity);
	else
		flippedVelocityAngle = atan(yVelocity / xVelocity) + PI;
	if (xNormal < 0)
		normalAngle = atan(yNormal / xNormal) + PI;
	else
		normalAngle = atan(yNormal / xNormal);

	//reflect velocity angle across the normal
	flippedVelocityAngle += 2 * (normalAngle - flippedVelocityAngle);

	//find new x and y components of velocity with diminished momentum from collision and set it to ball
	xVelocity = (1 - 0.15 * abs(xNormal)) * cos(flippedVelocityAngle) * sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
	yVelocity = (1 - 0.15 * abs(yNormal)) * sin(flippedVelocityAngle) * sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
	setVelocity(xVelocity, yVelocity);
}

//Adjust velocity due to acceleration of gravity
void Ball::gravity(double portion)
{
	double x, y;
	velocity.getVectorComponents(x, y);
	y += portion*2;
	velocity.setY(y);
}

//Set the inverse mass (>=0) of the ball
void Ball::setInvMass(double newInvMass)
{
	if (newInvMass >= 0)
		inverseMass = newInvMass;
}

//Set the velocity of the ball
void Ball::setVelocity(double xVect, double yVect)
{
	velocity.setX(xVect);
	velocity.setY(yVect);
}

//return velocity vector
Vec2D Ball::getVelocity() const
{
	return velocity;
}

//return the radius of the ball
double Ball::getRadius() const
{
	return radius;
}

//Move the ball along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
void Ball::move(double portion)
{
	double dx, dy;
	velocity.getVectorComponents(dx, dy);
	this->setPos(this->x() + portion * dx, this->y() + portion * dy);
	gravity(portion);
}

//Constructor - Pass coordinates and diameter of circle to QGraphicsEllipseItem constructor and set invMass
Ball::Ball(double xCenter, double yCenter, double rad, double invMass) : QGraphicsEllipseItem(xCenter, yCenter, 2*rad, 2*rad)
{
	setInvMass(invMass);
	radius = rad;
}

//Destructor
Ball::~Ball()
{
}
