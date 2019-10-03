/* Program name: block.cpp
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: Simulate a block object (currently all stationary)
    ================================================================
    FUNCTIONS:
	+ Vec2D collide() - Handle collision with another object
	+ void setInvMass(double) - Set the inverse mass (>=0) of the block
	+ void setVelocity(double, double) - Set the velocity of the ball
	+ Vec2D getVelocity() - return velocity vector
	+ void move() - /Move the block along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
	+ Block(double = 0, double = 0, double = 10, double = 10, double = 0) - Constructor - Pass coordinates, width and heigth to QGraphicsRectItem constructor and set invMass
	+ ~Block() - Destructor
	- void gravity() - Adjust velocity due to acceleration of gravity
	===================================================================
	VARIABLES:
	- Vec2D velocity - represent the x and y components of the block's velocity
	- double inverseMass - represent the inverse mass (mass^(-1)) of an object (to be used for moveable object collision)
*/

//This object is only stationary. I had plans to make it moveable, but didn't have time before the deadline. Maybe in the future.
//Much of this code will be nearly identical to ball.cpp when I'm done with it.
#include "block.h"
#include <QPainter>

//Handle collision with another object
Vec2D Block::collide()
{
	//hold open
	return Vec2D();
}

//Adjust velocity due to acceleration of gravity
void Block::gravity()
{
	//hold open
}

//Set the inverse mass (>=0) of the block
void Block::setInvMass(double newInvMass)
{
	if (newInvMass >= 0)
		inverseMass = newInvMass;
}

//Set the velocity of the ball
void Block::setVelocity(double xVect, double yVect)
{
	velocity.setX(xVect);
	velocity.setY(yVect);
}

//return velocity vector
Vec2D Block::getVelocity()
{
	//hold open
	return Vec2D();
}

//Move the block along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
void Block::move()
{
	//hold open
}

//Constructor - Pass coordinates, width and heigth to QGraphicsRectItem constructor and set invMass
Block::Block(double xCorner, double yCorner, double width, double height, double invMass) : QGraphicsRectItem(xCorner, yCorner, width, height)
{
	setInvMass(invMass);
}

//Destructor
Block::~Block()
{
	//hold open
}
