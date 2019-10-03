/* Program name: game.cpp
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

#include <QGraphicsScene>
#include <QApplication>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include "game.h"

#define MAX_PATH_LENGTH 80
#define MAX_VELOCITY 50.0

//set position, color, font style and size for a text item
void Game::formatText(QGraphicsTextItem * textItem, QString fontName, int fontSize, QColor color, int xPos, int yPos)
{
	textItem->setFont(QFont(fontName, fontSize));
	textItem->setDefaultTextColor(color);
	textItem->setPos(xPos, yPos);
	textItem->setZValue(100);
}

//set position, brush color and style, and pen color and thickness of a rect item
void Game::formatRect(Block * item, int xPos, int yPos, QColor penColor, int thickness, QColor brushColor, Qt::BrushStyle pattern)
{
	item->setPos(xPos, yPos);
	item->setPen(QPen(penColor, thickness));
	item->setBrush(QBrush(brushColor, pattern));
}

//set position, brush color and style, and pen color and thickness of a ellipse item
void Game::formatEllipse(Ball * item, int xPos, int yPos, QColor penColor, int thickness, QColor brushColor, Qt::BrushStyle pattern)
{
	item->setPos(xPos, yPos);
	item->setPen(QPen(penColor, thickness));
	item->setBrush(QBrush(brushColor, pattern));
}

//Set initial values to object values and create level from data file
void Game::buildLevel()
{
	//open file with level data
	std::ifstream levelFile;
	std::string fileName;
	fileName = "lvl" + std::to_string(currentLevel) + ".txt";
	levelFile.open(fileName);

	//check if file successfully opened, then build level based on contained data
	if (levelFile.is_open())
	{
		//create local variables, set initial values of of angle and and magnitude, reset launched flag
		double x1, y1, width, height, rad, invMass;
		angle = 0;
		magnitude = MAX_VELOCITY / 2;
		launchedFlag = false;

		//create velocity text and control text and format them
		vText = new VelocityText(angle, magnitude);
		cText = new QGraphicsTextItem(QString(u8"Controls:   [\u2191][\u2193] \u00b1power   [\u2190][\u2192] \u00b1angle   [Space] launch ball"));
		formatText(cText, "times", 16, Qt::blue, 15, 560);

		//read playerBall data, create playerBall and pathline, and format them
		levelFile >> x1 >> y1 >> rad >> invMass;
		playerBall = new Ball(0, 0, rad, invMass);
		formatEllipse(playerBall, x1 - rad, y1 - rad, Qt::darkBlue, 3, Qt::blue, Qt::Dense3Pattern);
		pathLine = new QGraphicsLineItem(playerBall->x()+rad, playerBall->y()+rad, playerBall->x() +rad + MAX_PATH_LENGTH / 2 + 5 + rad, playerBall->y() +rad);
		pathLine->setPen(QPen(Qt::white, 3));

		//read goalBlock data and create goalBlock and format it
		levelFile >> x1 >> y1 >> width >> height >> invMass;
		goalBlock = new Block(0, 0, width, height);
		formatRect(goalBlock, x1, y1, Qt::darkGreen, 3, Qt::green, Qt::Dense5Pattern);

		//read each line from the rest of the level data file and use it to create the levelBlocks and format them
		for (int i = 0; levelFile.get() != EOF; i++)
		{
			levelFile >> x1 >> y1 >> width >> height >> invMass;
			levelBlocks.push_back(new Block(0, 0, width, height));
			if(i < 4)
				formatRect(levelBlocks.back(), x1, y1, Qt::darkGray, 3, Qt::darkGray);
			else
				formatRect(levelBlocks.back(), x1, y1, Qt::darkGray, 3, Qt::gray);
		}
		levelFile.close();

		//add newly created items to scene
		scene->addItem(vText);
		scene->addItem(cText);
		scene->addItem(pathLine);
		scene->addItem(playerBall);
		scene->addItem(goalBlock);
		for (auto &i : levelBlocks)
			scene->addItem(i);
	}
	//filename matched none, last level cleared, game over
	else
	{
		cText = new QGraphicsTextItem(QString("GAME OVER"));
		formatText(cText, "times", 36, Qt::blue, 225, 245);
		scene->addItem(cText);
	}
}

//Return level to initial state
void Game::resetLevel()
{
	//pause gameplay
	timer->stop();

	//remove all items from the scene
	for (auto &i : levelBlocks)
		scene->removeItem(i);
	scene->removeItem(playerBall);
	scene->removeItem(goalBlock);
	scene->removeItem(vText);
	scene->removeItem(cText);
	scene->removeItem(pathLine);

	//properly delete all pointers
	playerBall = nullptr;
	goalBlock = nullptr;
	vText = nullptr;
	cText = nullptr;
	pathLine = nullptr;
	delete playerBall;
	delete goalBlock;
	delete vText;
	delete cText;
	delete pathLine;

	//Clear levelBlocks vector
	levelBlocks.clear();

	buildLevel();
}

//Handle level winning message and level advancement
void Game::levelWon()
{
	QMessageBox winnerMessageBox;
	winnerMessageBox.setText(QString::fromStdString("Level " + std::to_string(currentLevel) + " completed!!!"));
	winnerMessageBox.setWindowTitle("WINNER");
	winnerMessageBox.exec();
	currentLevel++;
	resetLevel();
}

//Handle user key presses
void Game::keyPressEvent(QKeyEvent *event)
{
	//If ball is unlaunched:
	//	Left/Right: ++/-- angle by 1 degree (0-360)
	//	Up/Down: ++/-- magnitude by 1% power (0-100)
	//	Space: set ball velocity, remove guideline, and launch ball
	//If ball is launched:
	//	Space: pause/unpause timer
	//	R: reset level
	if (!launchedFlag)
	{
		if (event->key() == Qt::Key_Left)
			angle = (angle + 359) % 360;
		else if (event->key() == Qt::Key_Right)
			angle = (angle + 361) % 360;
		else if (event->key() == Qt::Key_Up)
			magnitude += MAX_VELOCITY / 100;
		else if (event->key() == Qt::Key_Down)
			magnitude -= MAX_VELOCITY / 100;
		else if (event->key() == Qt::Key_Space)
		{
			launchedFlag = true;
			playerBall->setVelocity(magnitude*cos(angle*3.14159 / 180), magnitude*sin(angle*3.14159 / 180));
			scene->removeItem(pathLine);
			scene->removeItem(vText);
			cText->setPlainText("CONTROLS   [Space] Pause/Resume   [R] Reset Level");
			formatText(cText, "time", 16, Qt::blue, 80, 560);
			timer->start(0035);
		}
		if (magnitude > MAX_VELOCITY)
			magnitude = MAX_VELOCITY;
		if (magnitude < 0)
			magnitude = 0;

		//update vText and pathLine
		pathLine->setLine(playerBall->x() + playerBall->getRadius(), playerBall->y() + playerBall->getRadius(),
			playerBall->x() + playerBall->getRadius() + (magnitude + 10) * MAX_PATH_LENGTH / MAX_VELOCITY * cos(angle * 3.14159 / 180),
			playerBall->y() + playerBall->getRadius() + (magnitude + 10) * MAX_PATH_LENGTH / MAX_VELOCITY * sin(angle * 3.14159 / 180));
		vText->update();
	}
	else
		if (event->key() == Qt::Key_Space)
			if (timer->isActive())
				timer->stop();
			else
				timer->start(0035);
		else if (event->key() == Qt::Key_R)
			resetLevel();
}

void Game::getItemData(Block *rectItem, Block *retItem, double &xMin, double &yMin, double &xMax, double &yMax)
{
	retItem = rectItem;
	xMin = rectItem->x();
	yMin = rectItem->y();
	xMax = rectItem->boundingRect().width() - 1 + rectItem->x();
	yMax = rectItem->boundingRect().height() - 1 + rectItem->y();
}

//return whether the trajectory puts it passing an object
bool Game::collisionPossible(double centerX, double centerY, double xTrajectory, double yTrajectory, double xMin, double yMin, double xMax, double yMax)
{
	double leftBorder = xMin - playerBall->getRadius();
	double rightBorder = xMax + playerBall->getRadius();
	double topBorder = yMin - playerBall->getRadius();
	double bottomBorder = yMax + playerBall->getRadius();
	double endX = centerX + xTrajectory;
	double endY = centerY + yTrajectory;
	return !((centerX < leftBorder && endX < leftBorder) || (centerX > rightBorder && endX > rightBorder) || (centerY < topBorder && endY < topBorder) || (centerY > bottomBorder && endY > bottomBorder));
}

//set the parameters for the normal of the collision and track projected collision item and that a collision happened
void Game::setNormal(Block* hitItem, double x, double y, double setSubPortion)
{
	projectedCollision = true;
	goalBlockCollision = false;
	collItem = hitItem;
	normal.setXY(x, y);
	subPortion = setSubPortion;
}

//detect any collisions and set normal appropriately
void Game::collisionDetect(Block *item, double portion, double xMin, double yMin, double xMax, double yMax)
{
	double centerX = playerBall->x() + playerBall->getRadius();	//x value of the center point of the playerBall object
	double centerY = playerBall->y() + playerBall->getRadius();	//y value of the center point of the playerBall object
	double leftBorder = xMin - playerBall->getRadius();			//imaginary border wall along the left side of the block one radius away
	double rightBorder = xMax + playerBall->getRadius();		//imaginary border wall along the right side of the block one radius away
	double topBorder = yMin - playerBall->getRadius();			//imaginary border wall along the top of the block one radius away
	double bottomBorder = yMax + playerBall->getRadius();		//imaginary border wall along the bottom of the block one radius away
	double endX, endY;											//coordinates for the endpoints of projected movement
	double xTrajectory, yTrajectory;							//x and y components of playerBall's velocity along it's current trajectory
	double tmpSubPortion;										//temporary measurment of subportion to collision (set to subPortion if determined to be smaller)

	//get trajectory of the playerBall for the remainder of the frame
	playerBall->getVelocity().getVectorComponents(xTrajectory, yTrajectory);
	xTrajectory *= portion;
	yTrajectory *= portion;
	endX = centerX + xTrajectory;
	endY = centerY + yTrajectory;

	//Only check for collision if trajectory puts the ball past the minimum and/or maximum bounds of the block (i.e. if the ball before and after moving are still on the same side of the block, it can't have collided with it)
	if (collisionPossible(centerX, centerY, xTrajectory, yTrajectory, xMin, yMin, xMax, yMax))
		//The following 4 if statements test to see if the ball crosses an edge of the block from the outside,
		//that it's not in contact already, and that the distance to the collision is less than the distance to any previous collisions in this order:
		//1. from the left, 2. from the top, 3. from the right, and 4. from the bottom (NOTE: coordinates on the y axis increase downward in QT framework)
		if (linesCross(centerX, centerY, endX, endY, leftBorder, yMin, leftBorder, yMax)
			&& centerX < leftBorder
			&& (leftBorder - centerX) / xTrajectory <= subPortion)
			setNormal(item, -1, 0, (leftBorder - centerX) / xTrajectory);
		else if (linesCross(centerX, centerY, endX, endY, xMin, topBorder, xMax, topBorder)
			&& centerY < topBorder
			&& (topBorder - centerY) / yTrajectory <= subPortion)
			setNormal(item, 0, -1, (topBorder - centerY) / yTrajectory);
		else if (linesCross(centerX, centerY, endX, endY, rightBorder, yMin, rightBorder, yMax)
			&& centerX > rightBorder
			&& (rightBorder - centerX) / xTrajectory <= subPortion)
			setNormal(item, 1, 0, (rightBorder - centerX) / xTrajectory);
		else if (linesCross(centerX, centerY, endX, endY, xMin, bottomBorder, xMax, bottomBorder)
			&& centerY > bottomBorder
			&& (bottomBorder - centerY) / yTrajectory <= subPortion)
			setNormal(item, 0, 1, (bottomBorder - centerY) / yTrajectory);
	//The following 4 statements test to see if the ball crosses a corner of the block from the outside,
	//that it's not in contact already, and that the distance to the collision is less than the distance to any previous collisions in this order:
	//1. on the top-left, 2. on the top-right, 3. on the bottom-left, 4. on the bottom-right (NOTE: Again, coordinates on the y axis increase downward in QT framework)
		else if (lineCornerCollide(centerX, centerY, endX, endY, xMin, yMin, playerBall->getRadius(), tmpSubPortion)
			&& centerX < xMin && centerY < yMin
			&& tmpSubPortion < subPortion)
			setNormal(item, (centerX + tmpSubPortion * xTrajectory - xMin) / playerBall->getRadius(), (centerY + tmpSubPortion * yTrajectory - yMin) / playerBall->getRadius(), tmpSubPortion);
		else if (lineCornerCollide(centerX, centerY, endX, endY, xMax, yMin, playerBall->getRadius(), tmpSubPortion)
			&& centerX > xMax && centerY < yMin
			&& tmpSubPortion < subPortion)
			setNormal(item, (centerX + tmpSubPortion * xTrajectory - xMax) / playerBall->getRadius(), (centerY + tmpSubPortion * yTrajectory - yMin) / playerBall->getRadius(), tmpSubPortion);
		else if (lineCornerCollide(centerX, centerY, endX, endY, xMin, yMax, playerBall->getRadius(), tmpSubPortion)
			&& centerX < xMin && centerY > yMax
			&& tmpSubPortion < subPortion)
			setNormal(item, (centerX + tmpSubPortion * xTrajectory - xMin) / playerBall->getRadius(), (centerY + tmpSubPortion * yTrajectory - yMax) / playerBall->getRadius(), tmpSubPortion);
		else if (lineCornerCollide(centerX, centerY, endX, endY, xMax, yMax, playerBall->getRadius(), tmpSubPortion)
			&& centerX > xMax && centerY > yMax
			&& tmpSubPortion < subPortion)
			setNormal(item, (centerX + tmpSubPortion * xTrajectory - xMax) / playerBall->getRadius(), (centerY + tmpSubPortion * yTrajectory - yMax) / playerBall->getRadius(), tmpSubPortion);
}

//Detect and identify collisions and resolve them appropriately
void Game::collisionHandler(double portion)
{
	subPortion = 1;								//percentage of the trajectory to collision for the portion of the frame remaining (e.g. subPortion = .85 means that that collision occurs 85% of the way along remaining trajectory)
	projectedCollision = false;					//non-corner collision detected
	goalBlockCollision = false;
	Block *item;
	double xMin, xMax, yMin, yMax;				//x and y coordinates of the bounds of a rectangle

	//check goal block and each level block for a collision with playerBall
	for (int i = 0; i < levelBlocks.size()+1; i++)
	{
		//start with goalblock first, then each levelBlock
		if (i == 0)
			getItemData(goalBlock, item, xMin, yMin, xMax, yMax);
		else
			getItemData(levelBlocks[i-1], item, xMin, yMin, xMax, yMax);
		//detect if playerBall collides with current item, and set collision indo
		collisionDetect(item, portion, xMin, yMin, xMax, yMax);
		//Collision with goalblock, set goalBlockCollision to true; If another collision is detected this frame that hits first, goalBlockCollision will be set to false by setNormal()
		if (projectedCollision && i == 0)
			goalBlockCollision = true;
	}

	//collision with side occured, move ball along subPortion of trajectory, resolve collision, then run collision detect for remaining portion
	if (projectedCollision)
		if (goalBlockCollision)
			levelWon();
		else
		{
			playerBall->move(subPortion*portion);
			playerBall->collide(normal, collItem);
			collisionHandler(portion - subPortion * portion);
		}
	//no collision left to resolve, finish movement for frame
	else
		playerBall->move(portion);

}

//Determines if two line segments cross one another
bool Game::linesCross(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) const
{
	double denom = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
	double numerA = (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3);
	double numerB = (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
	if (denom == 0)
		return numerA == 0 && numerB == 0;
	return (numerA / denom >= 0 && numerA / denom <= 1) && (numerB / denom >= 0 && numerB / denom <= 1);
}

bool Game::lineCornerCollide(double x1, double y1, double x2, double y2, double xCorner, double yCorner, double radius, double &subPortion) const
{
	//equation of circle: (x-xCorner)^2 + (y-yCorner)^2 = radius^2
	//equation of line: y = ((y1-y2)/(x1-x2))*x + y1 - ((y1-y2)/(x1-x2))*x1
	//equation for intersection: (x-xCorner)^2 + (((y1-y2)/(x1-x2))*x+y1-((y1-y2)/(x1-x2))*x1-yCorner)^2 = radius^2
	//                 expanded: [(pow(((y1 - y2) / (x1 - x2)), 2) + 1)*x^2]  +  [(2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner))*x]  +  [pow(xCorner, 2) + pow((y1 - yCorner - ((y1 - y2) / (x1 - x2))*x1), 2) - pow(radius, 2)] = 0
	//quadratic 1: (2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner))+sqrt((2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner))^2-4*(pow(((y1 - y2) / (x1 - x2)), 2) + 1)*(pow(xCorner, 2) + pow((y1 - yCorner - ((y1 - y2) / (x1 - x2))*x1), 2) - pow(radius, 2)))/(2*(pow(((y1 - y2) / (x1 - x2)), 2) + 1))
	//quadratic 2: (2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner))-sqrt((2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner)))^2-4*(pow(((y1 - y2) / (x1 - x2)), 2) + 1)*(pow(xCorner, 2) + pow((y1 - yCorner - ((y1 - y2) / (x1 - x2))*x1), 2) - pow(radius, 2))))/(2*(pow(((y1 - y2) / (x1 - x2)), 2) + 1))
	double A = pow(((y1 - y2) / (x1 - x2)), 2) + 1;
	double B = 2 * (((y1 - y2) / (x1 - x2)) * (y1 - ((y1 - y2) / (x1 - x2))*x1 - yCorner) - xCorner);
	double C = pow(xCorner, 2) + pow((y1 - yCorner - ((y1 - y2) / (x1 - x2))*x1), 2) - pow(radius, 2);
	if (pow(B, 2) - 4 * A * C >= 0)
	{
		double quadratic1 = (-1 * B + sqrt(pow(B, 2) - 4 * A * C)) / (2 * A);
		double quadratic2 = (-1 * B - sqrt(pow(B, 2) - 4 * A * C)) / (2 * A);
		if ((quadratic1 - x1) / (x2 - x1) <= 1 && (quadratic1 - x1) / (x2 - x1) > 0.001 && (quadratic2 - x1) / (x2 - x1) <= 1 && (quadratic2 - x1) / (x2 - x1) > 0.001)
		{
			if (abs(quadratic1 - x1) < abs(quadratic2 - x1))
				subPortion = (quadratic1 - x1) / (x2 - x1);
			else
				subPortion = (quadratic2 - x1) / (x2 - x1);
			return true;
		}
		else if ((quadratic1 - x1) / (x2 - x1) <= 1 && (quadratic1 - x1) / (x2 - x1) > 0.001)
		{
			subPortion = (quadratic1 - x1) / (x2 - x1);
			return true;
		}
		else if ((quadratic2 - x1) / (x2 - x1) <= 1 && (quadratic2 - x1) / (x2 - x1) > 0.001)
		{
			subPortion = (quadratic2 - x1) / (x2 - x1);
			return true;
		}
	}
	return false;
}

//Contructor create/format game scene and items, initialize QTimer
Game::Game()
{	//create scene and build level
	scene = new QGraphicsScene();
	//connect collision detection to timer's timeout
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(detectAndMove()));
	currentLevel = 1;
	buildLevel();

	//format widget and set scene
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setFixedSize(800, 600);
	this->setScene(scene);
	scene->setSceneRect(0, 0, this->width(), this->height());
	scene->setBackgroundBrush(Qt::lightGray);


	//Set scene as the focus of key events
	setFocus();
}

//Destructor
Game::~Game()
{
	scene = nullptr;
	playerBall = nullptr;
	goalBlock = nullptr;
	vText = nullptr;
	cText = nullptr;
	delete scene;
	delete playerBall;
	delete goalBlock;
	delete vText;
	delete cText;
}

//Listens to timer, advances movement and collisions
void Game::detectAndMove()
{
	collisionHandler(1.0);
}