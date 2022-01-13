#include <iostream>
#include "MainGame.h"

int main(int argc, char** argv) //argument used to call SDL main
{
	MainScene mainGame;
	mainGame.run(); //runs game

	return 0;
}