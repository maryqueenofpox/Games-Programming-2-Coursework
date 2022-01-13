#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "Camera.h"

using namespace std;
/*/
Class for passing shaders from memory to the GPU to display in the game scene.
*/
class Shader
{
public:
	Shader();

	void setShader(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Camera& camera); //Shaders updated placement in relation to camera position
	void init(const string& filename); //Initiated from local file

	string Shader::LoadShader(const string& fileName); //Loaded into game from local file
	void Shader::shaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage); //Displayers error message when shader can't be loaded to program
	GLuint Shader::generateShader(const string& text, unsigned int type); //Shader is generated into the scene

    ~Shader(); //Deconstructor


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables
};
