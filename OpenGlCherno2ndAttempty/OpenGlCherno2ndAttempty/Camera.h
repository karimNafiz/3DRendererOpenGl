#pragma once
#include "../../LinearAlgebra.h"
#include "../../MeshTransformation.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class Camera
{
public:
	Vector3D world_position;
	Vector3D up;
	Vector3D target;
	float speed = 0.1;
	float h_angle = 0.0;
	float v_angle = 0.0;
	float screen_width = 0.0;
	float screen_height = 0.0;
	float mouse_position[2] = { 0.0 , 0.0 };


	Camera(const int& screen_width , const int& screen_height , const Vector3D& target, const Vector3D& up, const Vector3D& pos)
	{
		this->screen_height = screen_height;
		this->screen_width = screen_width;
		this->target = target.normalize();
		this->world_position = world_position;
		this->up = up.normalize();
		this->init();
	}
	void init() 
	{
		//Vector3D h_target(this->target.components[0] , 0.0, this->target.components[2]);
		//h_target = h_target.normalize();
		//float temp_angle = (180 / M_PI) * asin(abs(h_target.components[2]));



		//if (h_target.components[2] >= 0.0) 
		//{
		//	if(h_target.components[0] >= 0.0)
		//	{
		//		this->h_angle = 360 - temp_angle;

		//	}
		//	else 
		//	{
		//		this->h_angle = 180 + temp_angle;
		//	
		//	}
		//}
		//else 
		//{
		//	if (h_target.components[0] >= 0.0) 
		//	{
		//		this->h_angle = temp_angle;
		//	}
		//	else 
		//	{	
		//		this->h_angle = 180 - temp_angle;
		//	}
		//
		//}

		//this->v_angle = -(180 / M_PI) * asin(this->target.components[1]);
		this->mouse_position[0] = this->screen_width / 2;
		this->mouse_position[1] = this->screen_height / 2;
	
	
	}

	void onMouseMovement(float x, float y) 
	{
		float deltaX = x - this->mouse_position[0];
		float deltaY = y - this->mouse_position[1];
		this->mouse_position[0] = x;
		this->mouse_position[1] = y;
		this->h_angle += ((float)deltaX) /20.0f;
		this->v_angle += ((float)deltaY) / 50.0f;
		std::cout << "value of h angle " << h_angle << std::endl;
		std::cout << "value of v angle " << v_angle << std::endl;
		update();

	
	}
	void update() 
	{
		Vector3D world_up_vector(0.0, 1.0, 0.0);
		Vector3D world_screen_view(0.0, 0.0, 1.0);
		world_screen_view.Rotate(this->h_angle, world_up_vector);
		Vector3D world_U_vector = world_up_vector.cross(world_screen_view);
		// im not sure so if i need it 
		world_U_vector = world_U_vector.normalize();
		world_screen_view.Rotate(this->v_angle, world_U_vector);


		this->target = world_screen_view;
		this->up = target.cross(world_U_vector);
	
	
	}



	void move_camera(bool forward, bool backward, bool left, bool right, bool speed_up, bool speed_down)
	{
		if (speed_up)this->speed += 0.5;
		if (speed_down)this->speed -= 0.5;

		if (forward)
		{
			this->world_position = this->world_position +  this->target * this->speed;

		}
		if (backward)
		{
			this->world_position = this->world_position - this->target * this->speed;
		}
		if (left)
		{
			Vector3D left = this->target.cross(this->up);
			this->world_position = this->world_position + left * this->speed;

		}
		if (right)
		{
			Vector3D right = this->up.cross(this->target);
			this->world_position = this->world_position + right * this->speed;
		}

	}

	Matrix4x4 get_change_of_basis_matrix_view_space()
	{
		WorldTransformation camera_translation;
		camera_translation.set_translation_relative(-1 * this->world_position.components[0], -1 * this->world_position.components[1], -1 * this->world_position.components[2]);
		Matrix4x4 back_to_origin_matrix = camera_translation.get_linear_transformation_matrix();
		Vector3D N = this->target.normalize();
		Vector3D U = this->up.normalize().cross(N);
		Vector3D V = N.cross(U);

		float change_of_basis_matrix_list[4][4] = {
			{U.components[0] , U.components[1] , U.components[2], 0},
			{V.components[0] , V.components[1] , V.components[2], 0},
			{N.components[0] , N.components[1] , N.components[2], 0},
			{0 , 0 , 0 , 1.0}

		};
		Matrix4x4 change_of_basis_matrix(change_of_basis_matrix_list, 4);


		return change_of_basis_matrix * back_to_origin_matrix;




	}


};







