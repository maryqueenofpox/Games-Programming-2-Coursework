#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
//#include "audio.h"

enum class GameState{PLAY, EXIT};
using namespace glm;
/*/
Methods used by the "MainGame.cpp" script
*/
class MainScene
{
public:
	MainScene();
	~MainScene();

	void run();

private:

	void initSystems(); //Methods for starting systems
	void processInput(); //Processing player input
	void gameLoop(); //Game loop that handles reocurring processes while the game is running
	void drawTheMeshes(); //Draws the 3D model meshes into the game scene
	bool collision(vec3 m1Pos, float m1Rad, vec3 m2Pos, float m2Rad);
	//void drawSwordMesh();
	//void drawTreeMesh();

	Display _gameDisplay; //Game's display
	GameState _gameState; //Game state: true on game start
	Mesh monkeyMesh; //Mesh of the monkey 3D model
	Mesh swordMesh; //Mesh of the sword 3D model
	Mesh bananaMesh; //Mesh of the banana 3D model
	Camera myCamera; //Camera, the player's view of the scene
	Texture monkeyTexture; //Texture for the monkey 3D model
	Texture swordTexture; //Texture for the sword 3D model
	Texture bananaTexture; //Texture for the banana 3D model
	Shader monkeyShader; //Shader for the monkey 3D model
	Shader swordShader; //Shader for the sword 3D model
	Shader bananaShader; //Shader for the banana 3D model
	//audio monkeyScream; //Sound effect of monkey screaming when collision is detected



	float counter;


};

