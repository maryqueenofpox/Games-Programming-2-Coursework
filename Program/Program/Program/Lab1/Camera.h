#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;
/*/
Constructs the camera that acts as the POV of the player for the game scene
*/
struct Camera
{
public:
	Camera()
	{
	}
	/*/
	Set the specifics of camera using vector3 variables
	*/
	void initCamera(const vec3& pos, float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
	{
		this->pos = pos;
		this->forward = vec3(0.0f, 0.0f, 1.0f);
		this->up = vec3(0.0f, 1.0f, 0.0f);
		this->projection = perspective(fov, aspectRatio, nearClippingPlane, farClippingPlane);
	}
	/*/
	Returns the camera's perspective for use in the game scene
	*/
	inline mat4 GetViewProjection() const
	{
		return projection * lookAt(pos, pos + forward, up);
	}


protected:
private:
	mat4 projection; //matrix projection
	vec3 pos; //camera position
	vec3 forward; //move camera forward
	vec3 up; //move camera up
};


