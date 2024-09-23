#pragma once
#include "math_lib/3d_math.h"

class Camera
{
private:
    Vector4D cPosition{ 0.0f, 0.0f, 3.0f, 1.0f };  // Camera position
    Vector4D target{ 0.0f, 0.0f, -1.0f, 0.0f };    // Forward direction by default
    Vector4D up{ 0.0f, 1.0f, 0.0f, 0.0f };         // Up vector
    Vector4D right{ 1.0f, 0.0f, 0.0f, 0.0f };      // Right vector

    // Recalculate the forward, right, and up vectors
    void updateCameraVectors()
    {
        Vector4D forward = (target - cPosition).normalize();  // Forward direction
        right = Vector4D::cross(forward, up).normalize();     // Recalculate right vector
        up = Vector4D::cross(right, forward).normalize();     // Recalculate up vector
    }

public:
    // Handle camera movement using keyboard input
    void handleKeyboardInput(unsigned char key)
    {
        const float movementSpeed = 0.05f;

        switch (key)
        {
        case GLUT_KEY_UP:
            cPosition += (target - cPosition).normalize() * movementSpeed;  // Move forward
            break;
        case GLUT_KEY_DOWN:
            cPosition -= (target - cPosition).normalize() * movementSpeed;  // Move backward
            break;
        case GLUT_KEY_LEFT:
            cPosition -= right * movementSpeed;   // Move left
            break;
        case GLUT_KEY_RIGHT:
            cPosition += right * movementSpeed;   // Move right
            break;
        default:
            break;
        }
        updateCameraVectors();  // Update the camera vectors after movement
    }

    // Get the view matrix (based on lookAt function)
    Matrix4x4 getViewMatrix()
    {
        // LookAt matrix: cPosition is the eye, target + cPosition is the forward direction
        return Matrix4x4::lookAt(cPosition, cPosition + (target - cPosition).normalize(), up);
    }

    // Get the projection matrix (perspective projection)
    Matrix4x4 getProjectionMatrix()
    {
        return Matrix4x4::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    }
};
