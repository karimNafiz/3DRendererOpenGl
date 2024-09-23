#pragma once
#include "3D_Objects.h"

class RigidBody
{
	WorldTransformation cTransform{};

public:
	RigidBody(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->cTransform.set_position(x, y, z);

	}


	void translate(float x, float y, float z)
	{

		this->cTransform.translate(x, y, z);

	}

	void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) 
	{
		this->cTransform.rotate(x, y, z);
	
	}

	void setRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f) 
	{
		this->cTransform.setRotation(x, y, z);
	
	}


};
