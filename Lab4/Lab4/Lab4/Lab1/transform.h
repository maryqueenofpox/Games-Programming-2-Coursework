#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"

using namespace glm;
/*/
Constructs the transform of the 3D models, this handles the vector3 variables such as location and rotation
*/
struct Transform
{
public:
	/*/
	Position, rotation, and scale of the 3D model
	*/
	Transform(const vec3& pos = vec3(), const vec3& rot = vec3(), const vec3& scale = vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}
	/*/
	Gets 3D model to define transform of the object
	*/
	inline mat4 GetModel() const
	{
		mat4 posMat = translate(pos); //position matrix
		mat4 scaleMat = glm::scale(scale);
		mat4 rotX = rotate(rot.x, vec3(1.0, 0.0, 0.0));
		mat4 rotY = rotate(rot.y, vec3(0.0, 1.0, 0.0));
		mat4 rotZ = rotate(rot.z, vec3(0.0, 0.0, 1.0));
		mat4 rotMat = rotX * rotY * rotZ; //rotation matrix

		return posMat * rotMat * scaleMat;
	}
	/*/
	Takes 3D model's properties and manipulates them based on the camera's projection matrix
	*/
	inline mat4 GetMVP(const Camera& camera) const
	{
		mat4 VP = camera.GetViewProjection();
		mat4 M = GetModel();

		return VP * M;camera.GetViewProjection() * GetModel();
	}

	inline vec3* GetPos() { return &pos; } //getters
	inline vec3* GetRot() { return &rot; }
	inline vec3* GetScale() { return &scale; }

	inline void SetPos(vec3& pos) { this->pos = pos; } // setters
	inline void SetRot(vec3& rot) { this->rot = rot; }
	inline void SetScale(vec3& scale) { this->scale = scale; }
protected:
private:
	vec3 pos;
	vec3 rot;
	vec3 scale;
};


