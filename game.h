/* Program name: game.h
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: create a game window, all game objects and level (from level files), respond to user key presses, handle collisions and gameplay
    ================================================================
    FUNCTIONS:
	+ Game() - Contructor create/format game scene and items, initialize QTimer
	+ ~Game() - Destructor
	- void formatText(QGraphicsTextItem*, QString, int, QColor, int, int) - set position, color, font style and size for a text item
	- void Game::formatRect(Block*, int, int, QColor, int, QColor, Qt::BrushStyle = Qt::SolidPattern) - set position, brush color and style, and pen color and thickness of a rect item
	- void Game::formatEllipse(Ball*, int, int, QColor, int, QColor, Qt::BrushStyle = Qt::SolidPattern) - set position, brush color and style, and pen color and thickness of a ellipse item
	- void buildLevel() - Set initial values to object values and create level from data file
	- void resetLevel() - Return level to initial state
	- void levelWon() - Handle level winning message and level advancement
	- void keyPressEvent(QKeyEvent *event) - Handle user key presses
	- void getItemData(Block*, Block*, double&, double&, double&, double&) - extract data about the rectangle item
	- bool collisionPossible(double, double, double, double, double, double, double, double) - return whether the trajectory puts it passing an object
	- void setNormal(Block*, double, double, double) - set the parameters for the normal of the collision and track projected collision item and that a collision happened
	- void collisionDetect(Block*, double, double, double, double, double) - detect any collisions and set normal appropriately
	- void collisionHandler(double) - Detect and identify collisions and resolve them appropriately
	- bool linesCross(double, double, double, double, double, double, double, double) const - Determines if two line segments cross one another
	- bool lineCornerCollide(double, double, double, double, double, double, double, double&) const - /Determines if a line comes within a radius of a corner point
	============================================================================
	VARIABLES:
	- QGraphicsScene *scene - The scene upon which items exist (part of QT framework)
	- Ball *playerBall - The player's ball that they launch
	- Block *goalBlock - The Block that you're trying to hit
	- std::vector<Block*> levelBlocks - The Blocks that make up the obstacles of the level
	- QGraphicsLineItem *pathLine - The line showing the current direction and magnitude of the launch
	- VelocityText *vText - The text displaying the velocity information
	- QGraphicsTextItem *cText - The test displaying the controls information
	- QTimer *timer - The timer that advances the frame of the simulation/game
	- Vec2D normal - The normal of a collision
	- Block *collItem - Item that is to be hit by playerBall's trajectory
	- bool launchedFlag - Flag to determine if game has begun
	- bool projectedCollision - A collision will occur along projected trajectory
	- bool goalBlockCollision - projected collision is with the goalblock
	- double subPortion - The portion of the current trajorectory until the playerBall collides with an object
	- double magnitude - Magnitude of the launch velocity
	- int angle - Angle of the launch velocity
	- int currentLevel - Level currently playing.
    ==========================================================================
	SLOT:
	+ void detectAndMove() - Listens to timer, advances movement and collisions
*/

#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include <vector>
#include "ball.h"
#include "block.h"
#include "velocitytext.h"

class Game : public QGraphicsView
{
	Q_OBJECT
public:
	Game();
		//Contructor create/format game scene and items, initialize QTimer
	~Game();
		//Destructor
private:
	void formatText(QGraphicsTextItem*, QString, int, QColor, int, int);
		//set position, color, font style and size for a text item
	void Game::formatRect(Block*, int, int, QColor, int, QColor, Qt::BrushStyle = Qt::SolidPattern);
		//set position, brush color and style, and pen color and thickness of a rect item
	void Game::formatEllipse(Ball*, int, int, QColor, int, QColor, Qt::BrushStyle = Qt::SolidPattern);
		//set position, brush color and style, and pen color and thickness of a ellipse item
	void buildLevel();
		//Set initial values to object values and create level from data file
	void resetLevel();
		//Return level to initial state
	void levelWon();
		//Handle level winning message and level advancement
	void keyPressEvent(QKeyEvent *event);
		//Handle user key presses
	void getItemData(Block*, Block*, double&, double&, double&, double&);
		//extract data about the rectangle item
	bool collisionPossible(double, double, double, double, double, double, double, double);
		//return whether the trajectory puts it passing an object
	void setNormal(Block*, double, double, double);
		//set the parameters for the normal of the collision and track projected collision item and that a collision happened
	void collisionDetect(Block*, double, double, double, double, double);
		//detect any collisions and set normal appropriately
	void collisionHandler(double);
		//Detect and identify collisions and resolve them appropriately
	bool linesCross(double, double, double, double, double, double, double, double) const;
		//Determines if two line segments cross one another
	bool lineCornerCollide(double, double, double, double, double, double, double, double&) const;
		//Determines if a line comes within a radius of a corner point
	QGraphicsScene *scene;				//The scene upon which items exist (part of QT framework)
	Ball *playerBall;					//The player's ball that they launch
	Block *goalBlock;					//The Block that you're trying to hit
	std::vector<Block*> levelBlocks;	//The Blocks that make up the obstacles of the level
	QGraphicsLineItem *pathLine;		//The line showing the current direction and magnitude of the launch
	VelocityText *vText;				//The text displaying the velocity information
	QGraphicsTextItem *cText;			//The test displaying the controls information
	QTimer *timer;						//The timer that advances the frame of the simulation/game
	Vec2D normal;						//The normal of a collision
	Block *collItem;					//Item that is to be hit by playerBall's trajectory
	bool launchedFlag;					//Flag to determine if game has begun
	bool projectedCollision;			//A collision will occur along projected trajectory
	bool goalBlockCollision;			//projected collision is with the goalblock
	double subPortion;					//The portion of the current trajorectory until the playerBall collides with an object
	double magnitude;					//Magnitude of the launch velocity
	int angle;							//Angle of the launch velocity
	int currentLevel;					//Level currently playing.
public slots:
	void detectAndMove();
		//Listens to timer, advances movement and collisions
};

#endif // GAME_H
