/* Program name: ball.h
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

#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include "vec2d.h"
#include "velocitytext.h"

class Ball : public QGraphicsEllipseItem
{
public:
	void collide(Vec2D, QGraphicsItem*);
		//Handle collision with another object
	void setInvMass(double);
		//Set the inverse mass (>=0) of the ball
	void setVelocity(double, double);
		//Set the velocity of the ball
	Vec2D getVelocity() const;
		//return velocity vector
	double getRadius() const;
		//return the radius of the ball
	void move(double);
		//Move the ball along its velocity vector, detect/handle collisions, calculate new velocity due to gravity
	Ball(double = 0, double = 0, double = 10, double = 0);
		//Constructor - Pass coordinates and diameter of circle to QGraphicsEllipseItem constructor and set invMass
	~Ball();
		//Destructor
private:
	void gravity(double);
		//Adjust velocity due to acceleration of gravity
	Vec2D velocity;		//represent the x and y components of the ball's velocity
	double inverseMass;	//represent the inverse mass (mass^(-1)) of an object (to be used for moveable object collision)
	double radius;		//radius of the ball
};

#endif //BALL_H
