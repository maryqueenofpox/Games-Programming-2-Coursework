#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;

/*/
Methods used by the "Display.cpp"
*/
class Display
{
public:
	Display(); //display constructor
	~Display(); //display destructor 
	void createDisplay(); //create display for game scene
	void switchBuffer(); //swaps buffers
	void clearDisplay(float r, float g, float b, float a); //clear display for game scene

	float getWidth(); 
	float getHeight();

private:

	void giveError(string errorString); //error message
	
	SDL_GLContext glContext; //global variable to hold the context
	SDL_Window* sdlWindow; //holds pointer to out window
	float screenWidth; //pixel width of the game scene
	float screenHeight; //pixel height of the game scene
};

