/* Program name: main.cpp
*  Author: Cory Knoll
*  Date Last Modified: 5/7/2019
*  Description: Create a game object that will build and run physics simulations
*/

#include "FinalProject.h"
#include <QtWidgets/QApplication>
#include "game.h"

Game *game; //Needed at some point, but can't remember if I used... Deletion TBD

int main(int argc, char *argv[])
{
	//Create QT application
	QApplication a(argc, argv);
	//FinalProject w;

	//Create game and make it visible
	game = new Game();
	game->show();

	//Properly delete game on exit
	game = nullptr;
	delete game;

	return a.exec();
}
