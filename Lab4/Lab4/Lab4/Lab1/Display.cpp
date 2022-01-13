#include "Display.h"

using namespace std;
/*/
Display constructor
*/
Display::Display()
{
	sdlWindow = nullptr; //initialise to generate null access violation for debugging. 
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}
/*/
Display decstructor
*/
Display::~Display()
{
	SDL_GL_DeleteContext(glContext); // delete context
	SDL_DestroyWindow(sdlWindow); // detete window 
	SDL_Quit();
}

float Display::getWidth() { return screenWidth; } //getters
float Display::getHeight() { return screenHeight; }
/*/
Displays error message
*/
void Display::giveError(string errorString)
{
	cout << errorString << endl;
	cout << "press any  key to quit...";
	int in;
	cin >> in;
	SDL_Quit();
}
/*/
Buffer swapper for display
*/
void Display::switchBuffer()
{
	SDL_GL_SwapWindow(sdlWindow); //swap buffers
}
/*/
Clears display
*/
void Display::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}
/*/
Creates display for the game scene
*/
void Display::createDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	if (sdlWindow == nullptr)
	{
		giveError("window failed to create");
	}

	glContext = SDL_GL_CreateContext(sdlWindow);

	if (glContext == nullptr)
	{
		giveError("SDL_GL context failed to create");
	}
	/*/
	Error message when GLEW fails
	*/
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		giveError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}