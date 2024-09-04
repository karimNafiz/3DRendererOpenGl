#pragma once 
#include<iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



class Vector3D
{

public:
	float components[3] = {0.0 , 0.0 , 0.0};
	Vector3D() {}

	Vector3D(float x, float y, float z)
	{
		this->components[0] = x;
		this->components[1] = y;
		this->components[2] = z;
	}
	// vector additon 
	Vector3D operator +(const Vector3D other)
	{
		return Vector3D(this->components[0] + other.components[0], this->components[1] + other.components[1], this->components[2] + other.components[2]);
	}
	// vector subtraction 
	Vector3D operator -(const Vector3D other)
	{
		return Vector3D(this->components[0] - other.components[0], this->components[1] - other.components[1], this->components[2] - other.components[2]);
	}
	// Dot product
	float dot(const Vector3D& other) {
		return this->components[0] * other.components[0] +
			this->components[1] * other.components[1] +
			this->components[2] * other.components[2];
	}

	// Cross product
	Vector3D cross(const Vector3D& other) {
		return Vector3D(
			this->components[1] * other.components[2] - this->components[2] * other.components[1],
			this->components[2] * other.components[0] - this->components[0] * other.components[2],
			this->components[0] * other.components[1] - this->components[1] * other.components[0]
		);
	}

	Vector3D operator *(float scale) const
	{
		return Vector3D(this->components[0] * scale, this->components[1] * scale, this->components[2] * scale);
	}



	// Length of the vector
	float length() const
	{
		return std::sqrt(this->components[0] * this->components[0] +
			this->components[1] * this->components[1] +
			this->components[2] * this->components[2]);
	}

	// Normalize the vector
	Vector3D normalize() const
	{
		float len = length();
		if (len == 0) return Vector3D(0, 0, 0); // Handle zero-length vector
		return *this * (1.0f / len);
	}
	void Rotate(float angle, const Vector3D& v);

	//void Vector3f::Rotate(float Angle, const Vector3f& V)
	//{
	//	Quaternion RotationQ(Angle, V);

	//	Quaternion ConjugateQ = RotationQ.Conjugate();

	//	Quaternion W = RotationQ * (*this) * ConjugateQ;

	//	x = W.x;
	//	y = W.y;
	//	z = W.z;
	//}

};

class Quaternion
	{
	public:
		float x, y, z, w;

		Quaternion(float Angle, const Vector3D& V)
		{
			float HalfAngleInRadians = (M_PI / 180) * (Angle / 2);

			float SineHalfAngle = sinf(HalfAngleInRadians);
			float CosHalfAngle = cosf(HalfAngleInRadians);

			x = V.components[0] * SineHalfAngle;
			y = V.components[1] * SineHalfAngle;
			z = V.components[2] * SineHalfAngle;
			w = CosHalfAngle;

		}

		Quaternion(const float& _x, const float& _y, const float& _z, const float& _w)
		{
			this->x = _x;
			this->y = _y;
			this->z = _z;
			this->w = _w;
		}


		void Normalize()
		{
			float Length = sqrtf(x * x + y * y + z * z + w * w);

			x /= Length;
			y /= Length;
			z /= Length;
			w /= Length;
		}

		Quaternion Conjugate() const
		{
			Quaternion ret(-x, -y, -z, w);
			return ret;

		}

		Vector3D ToDegrees() {}


		Quaternion operator*(const Quaternion& r)
		{
			float w = (this->w * r.w) - (this->x * r.x) - (this->y * r.y) - (this->z * r.z);
			float x = (this->x * r.w) + (this->w * r.x) + (this->y * r.z) - (this->z * r.y);
			float y = (this->y * r.w) + (this->w * r.y) + (this->z * r.x) - (this->x * r.z);
			float z = (this->z * r.w) + (this->w * r.z) + (this->x * r.y) - (this->y * r.x);

			Quaternion ret(x, y, z, w);

			return ret;



		}
		Quaternion operator*(const Vector3D& v)
		{
			float w = -(this->x * v.components[0]) - (this->y * v.components[1]) - (this->z * v.components[2]);
			float x = (this->w * v.components[0]) + (this->y * v.components[2]) - (this->z * v.components[1]);
			float y = (this->w * v.components[1]) + (this->z * v.components[0]) - (this->x * v.components[2]);
			float z = (this->w * v.components[2]) + (this->x * v.components[1]) - (this->y * v.components[0]);

			Quaternion ret(x, y, z, w);

			return ret;
		}







	//void Vector3f::Rotate(float Angle, const Vector3f& V)
	//{
	//	Quaternion RotationQ(Angle, V);

	//	Quaternion ConjugateQ = RotationQ.Conjugate();

	//	Quaternion W = RotationQ * (*this) * ConjugateQ;

	//	x = W.x;
	//	y = W.y;
	//	z = W.z;
	//}




};






class Matrix4x4 
{
public:
	short no_of_values = 16;
	float matrix[16];

	Matrix4x4(float (*list_of_value_list)[4], short array_size)
	{
		
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++) 
			{

				this->matrix[(i * 4) + j] = *(*(list_of_value_list + i) + j);
			}
		}
	
	
	}
	Matrix4x4()
	{
		for (int i = 0; i < this->no_of_values; i++) 
		{
			this->matrix[i] = 0.0;
		}
	}
	void add_value_at_specified_row_colomn_1_index(float value, short row, short column) 
	{
		row--;
		column--;
		this->matrix[ (4 * row) + column ] = value;
	}

	void add_list_of_values_at_specified_row_1_index(const float* list, short row)
	{
		row--;  // Adjust row to 0-based index
		for (int i = 0; i < 4; i++)
		{
			this->matrix[row * 4 + i] = list[i];
		}
	}



	Matrix4x4 operator *(Matrix4x4& other) 
	{

		Matrix4x4 retMatrix;
		for (int i = 0; i < 4; i++) 
		{
			float temp_list[4] = {0.0 , 0.0 , 0.0 , 0.0};
			
			for (int z = 0; z < 4; z++) {
				
				for (int j = 0; j < 4; j++)
				{
					temp_list[z] += ((float)this->matrix[i * 4 + j]) * ((float)other.matrix[j * 4 + z]);
					/*std::cout << "|| value of z " << z << " index  " << i * 4 + j << " index " << j * 4 + z<<" ||    ";*/

				}
				/*std::cout << temp_list[z] << "     ";*/
			}
			/*std::cout << "\n";*/
			retMatrix.add_list_of_values_at_specified_row_1_index(temp_list, i + 1);


		}


		return retMatrix;
	
	
	
	}

	void show_matrix()
	{
		std::cout << "test" << "\n";
	
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++) 
			{
				std::cout << this->matrix[i * 4 + j] << " - ";
			
			}
			std::cout << "\n";
		}
	
	}
	




};