/* Program name: block.h
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

#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsRectItem>
#include "vec2d.h"

class Block : public QGraphicsRectItem
{
public:
	Vec2D collide();
		//Handle collision with another object
	void setInvMass(double);
		//Set the inverse mass (>=0) of the block
	void setVelocity(double, double);
		//Set the velocity of the ball
	Vec2D getVelocity();
		//return velocity vector
	void move();
		//Move the block along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
	Block(double = 0, double = 0, double = 10, double = 10, double = 0);
		//Constructor - Pass coordinates, width and heigth to QGraphicsRectItem constructor and set invMass
	~Block();
		//Destructor
private:
	void gravity();
		//Adjust velocity due to acceleration of gravity
	Vec2D velocity;		//represent the x and y components of the block's velocity
	double inverseMass;//represent the inverse mass (mass^(-1)) of an object (to be used for moveable object collision)
};

#endif //BLOCK_H