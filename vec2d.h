/* Program name: vec2d.h
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

#ifndef VEC2D_H
#define VEC2D_H

class Vec2D
{
public:
	void setX(double);
		//set value of x
	void setY(double);
		//set value of y
	void setXY(double, double);
		//set value of x and y
	void getVectorComponents(double&, double&);
		//return values of x and y
	Vec2D();
		//Constructor, initial vector [0,0]
	~Vec2D();
	//Destructor
private:
	double x;	//x component of the vector
	double y;	//y component of the vector
};

#endif //VEC2D_H