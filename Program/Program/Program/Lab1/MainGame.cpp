#include "MainGame.h"
#include "Camera.h"
//#include "audio.h"
#include <iostream>
#include <string>

using namespace glm;

Vertex vertices[] = { Vertex(vec3(-0.5, -0.5, 0), vec2(0.0, 0.0)),
					Vertex(vec3(0, 0.5, 0), vec2(0.5, 1.0)),
					Vertex(vec3(0.5, -0.5, 0), vec2(1.0, 0.0)) };

unsigned int indices[] = { 0, 1, 2 };

Transform transform;

MainScene::MainScene()
{
	_gameState = GameState::PLAY; //Sets game state to play
	Display* _gameDisplay = new Display(); //new display
	Mesh* swordMesh(); //Sword mesh, used for collision detection
	Mesh* monekyMesh(); //Monkey mesh, used for collision
	//audio* monkeyScream();
}
/*/
Destructor
*/
MainScene::~MainScene()
{
}
/*/
Processes ran by the game application from when it starts
*/
void MainScene::run()
{
	initSystems();
	gameLoop();
}

void MainScene::initSystems()
{
	_gameDisplay.createDisplay(); 
	monkeyMesh.loadMesh("..\\res\\monkey3.obj");//load monkey mesh
	swordMesh.loadMesh("..\\res\\sword.obj"); //load sword mesh
	bananaMesh.loadMesh("..\\res\\banana.obj"); //load tree mesh
	
	//monkeyScream.addSoundEffect("..\\res\\monkeyscream.wav");

	monkeyTexture.init("..\\res\\bricks.jpg"); //texture for monkey model
	monkeyShader.init("..\\res\\shader"); //monkey shader	

	swordTexture.init("..\\res\\swordTexture.png"); //texture for sword model
	swordShader.init("..\\res\\swordShader.png"); //sword shader

	bananaTexture.init("..\\res\\bananatexture.jpg"); //texture for banana model
	bananaShader.init("..\\res\\shader"); //banana shader

	myCamera.initCamera(vec3(0, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f); //start camera
	counter = 0.0f;
}
/*/
Continuous processes ran while the game application is open
*/
void MainScene::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		drawTheMeshes();
		collision(swordMesh.getSpherePos(), swordMesh.getSphereRadius(), monkeyMesh.getSpherePos(), monkeyMesh.getSphereRadius());
		processInput();	
	}
}

void MainScene::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}
/*/
Detects collision under set criteria. Displays output message and plays audio sound effect when condition is met.
*/
bool MainScene::collision(vec3 m1Pos, float m1Rad, vec3 m2Pos, float m2Rad)
{
	float distance = ((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance * distance < (m1Rad + m2Rad))
	{
		//audio.playSoundEffect();
		cout << "Monkey screamed in pain!!" << '\n';
		return true;
	}
	else
	{
		
		return false;
	}
}
/*/
Draws all the meshes into the scene; binds textures and shaders to each mesh.
*/
void MainScene::drawTheMeshes()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f); //clear screen
	
	//set position in world to spawn monkey mesh
	transform.SetPos(vec3(0.0,0.0, 0.0));
	transform.SetRot(vec3(0.0,counter * 2, 0));
	transform.SetScale(vec3(1.0, 1.0, 1.0));

	monkeyShader.setShader(); //bind shader to mesh
	monkeyShader.Update(transform, myCamera); //update shader in relation to transform
	monkeyTexture.Bind(0); //bind texture to the mesh
	monkeyMesh.spawn(); //draw the mesh in world scene
	monkeyMesh.updateSphereData(*transform.GetPos(), 0.6f);
	
	//set position in world to spawn sword mesh
	transform.SetPos(vec3(-8.0, sinf(counter), -2.0));
	transform.SetRot(vec3(-90, counter * 2, 0));
	transform.SetScale(vec3(0.05, 0.05, 0.05));

	swordShader.setShader(); //bind shader to mesh
	swordShader.Update(transform, myCamera); //update shader in relation to transform
	swordTexture.Bind(0); //bind texture to the mesh
	swordMesh.spawn();//draw the mesh in world scene
	swordMesh.updateSphereData(*transform.GetPos(), 0.6f);
	
	//set position in world to spawn banana mesh
	transform.SetPos(vec3(8.0 + -sinf(counter)* 5, 5.0, 0));
	transform.SetRot(vec3(50, 100, 50));
	transform.SetScale(vec3(0.01, 0.01, 0.01));

	bananaShader.setShader(); //bind shader to mesh
	bananaShader.Update(transform, myCamera); //update shader in relation to transform
	bananaTexture.Bind(0); //bind texture to the mesh
	bananaMesh.spawn();//draw the mesh in world scene

	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.switchBuffer();
} 

//void MainGame::drawSwordMesh()
//{
//	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

//	transform.SetPos(glm::vec3(-8.0, 5.0, 0.0));
//	transform.SetRot(glm::vec3(-90, counter *2, 0));
//	transform.SetScale(glm::vec3(0.05, 0.05, 0.05));

//	swordShader.Bind();
//	swordShader.Update(transform, myCamera);
//	swordTexture.Bind(0);
//	swordMesh.draw();
//	counter = counter + 0.01f;

//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnd();

//	_gameDisplay.swapBuffer();
//}

