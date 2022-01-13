#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"

using namespace std;
using namespace glm;
/*/
Constructs the physical geometric properties of the mesh
*/
struct Vertex
{
public:
	Vertex(const vec3& pos, const vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	vec3* GetPos() { return &pos; }
	vec2* GetTexCoord() { return &texCoord; }
	vec3* GetNormal() { return &normal; }

private:
	vec3 pos;
	vec2 texCoord;
	vec3 normal;
};
/*/
Constructs the sphere mesh around the 3D model mesh that is used for collision detection
*/
struct Sphere
{
public:

	Sphere() {}

	vec3 GetPos() { return pos; }
	float GetRadius() { return radius; }

	void SetPos(vec3 pos)
	{
		this->pos = pos;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	vec3 pos;
	float radius;
};
/*/
Holds the functions and methods used by the mesh script
*/
class Mesh
{
public:
	Mesh();
	~Mesh();

	void spawn(); //Draws mesh
	void init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices); //Initiates
	void loadMesh(const string& filename); //Loads mesh from local file
	void initMesh(const IndexedModel& model); //Initiates mesh
	void updateSphereData(vec3 pos, float radius); //Updates sphere data to match location in scene
	vec3 getSpherePos() { return meshSphere.GetPos(); }
	float getSphereRadius() { return meshSphere.GetRadius(); }

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	Sphere meshSphere; // Sphere mesh for collision
	GLuint vertexArrayObject; // Create array of object
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};

