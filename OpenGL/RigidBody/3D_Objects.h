#pragma once
#include "..\math_lib\3d_math.h"


#pragma once
#include "..\math_lib\3d_math.h"

class WorldTransformation
{
    Vector4D mPosition{ 0.0f, 0.0f, 0.0f, 1.0f };  // Initialize position to origin
    Vector4D mRotation{ 0.0f, 0.0f, 0.0f, 0.0f };  // Initialize rotation to zero
    float scale = 1.0f;

public:
    // Set position
    void set_position(float x, float y, float z)
    {
        this->mPosition.x = x;
        this->mPosition.y = y;
        this->mPosition.z = z;
    }

    // Translate by x, y, z
    void translate(float x, float y, float z)
    {
        this->mPosition.x += x;
        this->mPosition.y += y;
        this->mPosition.z += z;
    }

    // Set scale factor
    void setScale(float scale)
    {
        this->scale = scale;
    }

    // Set rotation in degrees for x, y, z axes
    void setRotation(float x, float y, float z)
    {
        this->mRotation.x = x;
        this->mRotation.y = y;
        this->mRotation.z = z;
    }

    void rotate(float x, float y, float z) 
    {
        this->mRotation.x += x;
        this->mRotation.y += y;
        this->mRotation.z += z;
    
    
    }

    // Get the final transformation matrix (translation * rotation * scale)
    Matrix4x4 getMatrix()
    {
        // Apply scaling, rotation, and translation transformations
        Matrix4x4 scaleMatrix = Matrix4x4::scale(scale, scale, scale);
        Matrix4x4 xRot = Matrix4x4::rotateX(mRotation.x);  // Rotation around X-axis
        Matrix4x4 yRot = Matrix4x4::rotateY(mRotation.y);  // Rotation around Y-axis
        Matrix4x4 zRot = Matrix4x4::rotateZ(mRotation.z);  // Rotation around Z-axis

        Matrix4x4 rotationMatrix = xRot * yRot * zRot;  // Combined rotation matrix
        Matrix4x4 translationMatrix = Matrix4x4::translate(mPosition);  // Translation matrix

        // Return the combined transformation matrix (order: translation * rotation * scale)
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
};
