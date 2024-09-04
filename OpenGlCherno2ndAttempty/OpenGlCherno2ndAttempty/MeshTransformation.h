#pragma once
#include "../../LinearAlgebra.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class WorldTransformation
{
public:
	float translation[3] = { 0.0 , 0.0 , 0.0 };
	float rotation[3] = { 0.0 , 0.0 , 0.0 };
	float scale[3] = { 1.0 , 1.0 , 1.0 };

	void set_rotation_relative(float x_axis_anchor_rotation_angle, float y_axis_anchor_rotation_angle, float z_axis_anchor_rotation_angle)
	{
		this->rotation[0] += x_axis_anchor_rotation_angle;
		this->rotation[1] += y_axis_anchor_rotation_angle;
		this->rotation[2] += z_axis_anchor_rotation_angle;


	}
	void set_translation_relative(float x_axis_translation, float y_axis_translation, float z_axis_translation)
	{
		this->translation[0] += x_axis_translation;
		this->translation[1] += y_axis_translation;
		this->translation[2] += z_axis_translation;


		//temporary code needs to be removed 
		// state after setting translation_vector



	}
	void set_scale(float x_scale, float y_scale, float z_scale) 
	{
		this->scale[0] += x_scale;
		this->scale[1] += y_scale;
		this->scale[2] += z_scale;
	
	
	}
	Matrix4x4 get_linear_transformation_matrix ()  
	{
		Matrix4x4 scale_trans = this->get_scale_matrix();
		Matrix4x4 rotation_trans = this->get_rotation_matrix();
		Matrix4x4 translation_trans = this->get_translation_matrix();




		return translation_trans * rotation_trans * scale_trans;
	
	
	
	}


	Matrix4x4 get_scale_matrix() 
	{
		float temp[4][4] = { {this->scale[0] , 0 , 0 , 0} , {0 , this->scale[1] , 0 , 0} , {0 , 0 , this->scale[2] , 0} , {0 , 0 , 0 , 1}};
		return Matrix4x4(temp, 4);
	
	}

	Matrix4x4 get_translation_matrix() 
	{
		float temp[4][4] = { {1 , 0 , 0 , this->translation[0]} , {0 , 1 , 0 , this->translation[1]} , {0 , 0 , 1 , this->translation[2]} , {0 , 0 , 0 , 1}};
		return Matrix4x4(temp, 4);
	
	}

	Matrix4x4 get_rotation_matrix()
	{
		/* Convert degrees to radians*/
		float radX = rotation[0] * (M_PI / 180.0f);
		float radY = rotation[1] * (M_PI / 180.0f);
		float radZ = rotation[2] * (M_PI / 180.0f);

		//float radX = rotation[0];
		//float radY = rotation[1];
		//float radZ = rotation[2];


		// X-axis rotation matrix
		float rotX[4][4] = {
			{1, 0, 0, 0},
			{0, cos(radX), -sin(radX), 0},
			{0, sin(radX), cos(radX), 0},
			{0, 0, 0, 1}
		};

		// Y-axis rotation matrix
		float rotY[4][4] = {
			{cos(radY), 0, -sin(radY), 0},
			{0, 1, 0, 0},
			{sin(radY), 0, cos(radY), 0},
			{0, 0, 0, 1}
		};

		// Z-axis rotation matrix
		float rotZ[4][4] = {
			{cos(radZ), -sin(radZ), 0, 0},
			{sin(radZ), cos(radZ), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		};

		// Combine the rotation matrices (assuming order Z * Y * X)
		Matrix4x4 rotMatX(rotX, 4);
		Matrix4x4 rotMatY(rotY, 4);
		Matrix4x4 rotMatZ(rotZ, 4);

		// The final rotation matrix is Z * Y * X
		Matrix4x4 finalRotMat = rotMatZ * rotMatY * rotMatX;

		return finalRotMat;
	}

	


};




