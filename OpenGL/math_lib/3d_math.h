#pragma once
#include <cmath>
#include <stdexcept>
#include <stdexcept>
#include <iostream>



class Vector4D
{
public:
    float x, y, z, w;

    // Constructor
    Vector4D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
        : x(x), y(y), z(z), w(w) {}

    // Addition of two vectors
    Vector4D operator+(const Vector4D& other) const
    {
        return Vector4D(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Subtraction of two vectors
    Vector4D operator-(const Vector4D& other) const
    {
        return Vector4D(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Scaling a vector
    Vector4D operator*(float scalar) const
    {
        return Vector4D(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Scalar multiplication assignment
    Vector4D& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    // Addition assignment
    Vector4D& operator+=(const Vector4D& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    // Subtraction assignment
    Vector4D& operator-=(const Vector4D& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    // Normalize the vector (ignores w component for normalization)
    Vector4D normalize() const
    {
        float length = std::sqrt(x * x + y * y + z * z);
        return Vector4D(x / length, y / length, z / length, w);  // Keep w unchanged
    }

    // Dot product
    static float dot(const Vector4D& a, const Vector4D& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // Cross product (ignores w component)
    static Vector4D cross(const Vector4D& a, const Vector4D& b)
    {
        return Vector4D(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
            0.0f);  // The result of cross product is a direction vector, so w = 0
    }
};

#pragma once
#include <cmath>  // Needed for std::tan

class Matrix4x4
{
public:
    float m[4][4];  // 4x4 matrix

    // Default constructor: Initialize to identity matrix
    Matrix4x4()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = (i == j) ? 1.0f : 0.0f;  // Identity matrix
            }
        }
    }

    // Overloaded multiplication operator for matrix multiplication
    Matrix4x4 operator*(const Matrix4x4& other) const
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    // Scale matrix
    static Matrix4x4 scale(float x, float y, float z)
    {
        Matrix4x4 scale;
        scale.m[0][0] = x;
        scale.m[1][1] = y;
        scale.m[2][2] = z;
        return scale;
    }

    // Translation matrix
    static Matrix4x4 translate(const Vector4D& position)
    {
        Matrix4x4 translation;
        translation.m[0][3] = position.x;
        translation.m[1][3] = position.y;
        translation.m[2][3] = position.z;
        return translation;
    }

    // Rotation matrices (X, Y, Z)
    static Matrix4x4 rotateX(float angle)
    {
        Matrix4x4 rotation;
        float radians = angle * (3.14159265358979323846f / 180.0f);
        rotation.m[1][1] = std::cos(radians);
        rotation.m[1][2] = -std::sin(radians);
        rotation.m[2][1] = std::sin(radians);
        rotation.m[2][2] = std::cos(radians);
        return rotation;
    }

    static Matrix4x4 rotateY(float angle)
    {
        Matrix4x4 rotation;
        float radians = angle * (3.14159265358979323846f / 180.0f);
        rotation.m[0][0] = std::cos(radians);
        rotation.m[0][2] = std::sin(radians);
        rotation.m[2][0] = -std::sin(radians);
        rotation.m[2][2] = std::cos(radians);
        return rotation;
    }

    static Matrix4x4 rotateZ(float angle)
    {
        Matrix4x4 rotation;
        float radians = angle * (3.14159265358979323846f / 180.0f);
        rotation.m[0][0] = std::cos(radians);
        rotation.m[0][1] = -std::sin(radians);
        rotation.m[1][0] = std::sin(radians);
        rotation.m[1][1] = std::cos(radians);
        return rotation;
    }

    // LookAt matrix
    static Matrix4x4 lookAt(const Vector4D& eye, const Vector4D& center, const Vector4D& up)
    {
        Vector4D f = (center - eye).normalize();  // Forward vector
        Vector4D r = Vector4D::cross(f, up).normalize();  // Right vector
        Vector4D u = Vector4D::cross(r, f);  // Recalculate Up vector

        Matrix4x4 view;
        view.m[0][0] = r.x;
        view.m[0][1] = u.x;
        view.m[0][2] = -f.x;
        view.m[0][3] = 0.0f;

        view.m[1][0] = r.y;
        view.m[1][1] = u.y;
        view.m[1][2] = -f.y;
        view.m[1][3] = 0.0f;

        view.m[2][0] = r.z;
        view.m[2][1] = u.z;
        view.m[2][2] = -f.z;
        view.m[2][3] = 0.0f;

        view.m[3][0] = -Vector4D::dot(r, eye);
        view.m[3][1] = -Vector4D::dot(u, eye);
        view.m[3][2] = Vector4D::dot(f, eye);
        view.m[3][3] = 1.0f;

        return view;
    }

    // Perspective matrix (cleaned)
    static Matrix4x4 perspective(float fov, float aspect, float near_flt, float far_flt)
    {
        Matrix4x4 result = {};  // Initialize all elements to zero
        float tanHalfFov = std::tan(fov * 0.5f * (3.14159265358979323846f / 180.0f));  // Convert FOV to radians




        result.m[0][0] = 1.0f / (aspect * tanHalfFov);
        result.m[1][1] = 1.0f / tanHalfFov;
        result.m[2][2] = -(far_flt + near_flt) / (far_flt - near_flt);
        result.m[2][3] = -1.0f;
        result.m[3][2] = -(2.0f * far_flt * near_flt) / (far_flt - near_flt);
        result.m[3][3] = 0.0f;

        return result;
    }
};
