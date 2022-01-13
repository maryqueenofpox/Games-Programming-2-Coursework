#pragma once
#include <string>
#include <GL\glew.h>
/*/
Methods used by "texture.cpp" script
*/
class Texture
{
public:
	Texture(); //constructor

	void Bind(unsigned int unit); // bind upto 32 textures
	void init(const std::string& fileName); // initiates texture from local file

	~Texture(); //deconstructor

protected:
private:

	GLuint textureHandler;
};

