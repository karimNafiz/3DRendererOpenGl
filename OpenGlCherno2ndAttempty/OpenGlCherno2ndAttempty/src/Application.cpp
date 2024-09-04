
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../LinearAlgebra.h"
#include "../../MeshTransformation.h"
#include "../../Camera.h"
#include <Windows.h>


void Vector3D::Rotate(float angle, const Vector3D& v)
{
    Quaternion rotation_q(angle, v);
    Quaternion conjugate_rotation_q = rotation_q.Conjugate();
    Quaternion W = rotation_q * (*this) * conjugate_rotation_q;

    this->components[0] = W.x;
    this->components[1] = W.y;
    this->components[2] = W.z;


}
int width = GetSystemMetrics(SM_CXSCREEN);
int height = GetSystemMetrics(SM_CYSCREEN);

Vector3D initial_camera_position(0.0, 0.0, -1.0);
Vector3D initial_camera_target(0.0, 0.0, 1.0);
Vector3D initial_camera_up(0.0, 1.0, 0.0);





Camera world_camera(width, height, initial_camera_target, initial_camera_up, initial_camera_position);


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Handle mouse movement here
    std::cout << "Mouse Position: (" << xpos << ", " << ypos << ")" << std::endl;
    world_camera.onMouseMovement(xpos , ypos);
}

//int main() 
//{
//
//    float matrix1[4][4] = {
//    { 0.0012f, -0.0008f, 0.0005f, -0.0003f },
//    { -0.0007f, 0.0009f, -0.0010f, 0.0004f },
//    { 0.0006f, 0.0003f, -0.0009f, 0.0011f },
//    { -0.0004f, 0.0007f, 0.0002f, -0.0012f }
//    };
//    float matrix2[4][4] = {
//    { -0.0010f, 0.0005f, -0.0007f, 0.0008f },
//    { 0.0003f, -0.0004f, 0.0002f, -0.0009f },
//    { -0.0006f, 0.0001f, 0.0004f, -0.0002f },
//    { 0.0007f, -0.0003f, 0.0009f, 0.0001f }
//    };
//
//    Matrix4x4 mat1(matrix1, 4);
//    Matrix4x4 mat2(matrix2, 4);
//
//    Matrix4x4 result = mat1 * mat2;
//    result.show_matrix();
//
//
//
//
//
//}


//



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool key_press[6] = {false , false , false , false , false , false};
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // Close window on ESC key press

    if (key == GLFW_KEY_UP)
        key_press[0] = (action == GLFW_PRESS);

    if (key == GLFW_KEY_DOWN)
        key_press[1] = (action == GLFW_PRESS);

    if (key == GLFW_KEY_RIGHT)
        key_press[3] = (action == GLFW_PRESS);

    if (key == GLFW_KEY_LEFT)
        key_press[2] = (action == GLFW_PRESS);

    if (key == GLFW_KEY_PAGE_UP)
        key_press[4] = (action == GLFW_PRESS);

    if (key == GLFW_KEY_PAGE_DOWN)
        key_press[5] = (action == GLFW_PRESS);

    world_camera.move_camera(key_press[0] , key_press[1] , key_press[2] , key_press[3] , key_press[4] , key_press[5]);

}












// Vertex Shader Source Code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
uniform mat4 gWorld;
out vec4 fragColor;
void main() {
    gl_Position = gWorld * vec4(aPos , 1.0);
    fragColor = vec4(aColor , 1.0);
}
)";

// Fragment Shader Source Code
const char* fragmentShaderSource = R"(
#version 330 core
in vec4 fragColor;
out vec4 FragColor;

void main() {
    FragColor = fragColor;
}
)";


GLint all_matrix_multiplied_location;
void multiplyMatrices(const float* mat1, const float* mat2, float* result) {
    for (int i = 0; i < 4; ++i) {          // Iterate over the rows of mat1
        for (int j = 0; j < 4; ++j) {      // Iterate over the columns of mat2
            result[i * 4 + j] = 0;         // Initialize the result matrix element
            for (int k = 0; k < 4; ++k) {  // Multiply the corresponding elements and sum them up
                result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
            }
        }
    }
}






int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Get the primary monitor */
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        glfwTerminate();
        return -1;
    }

    /* Get the video mode of the primary monitor */
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    /* Create a full-screen window and its OpenGL context */
    window = glfwCreateWindow(mode->width, mode->height, "Hello World", monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Set the key callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error loading glew" << std::endl;
        return -1;
    }

    // Build and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Build and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    all_matrix_multiplied_location = glGetUniformLocation(shaderProgram, "gWorld");
    if (all_matrix_multiplied_location == -1) {
        printf("Error getting uniform location of 'gScale'\n");
        exit(1);
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers and configure vertex attributes
    /*
        float vertices[] = {
     0.5f, -0.5f, 0.0f,  // Bottom-right
    -0.5f, -0.5f, 0.0f,  // Bottom-left
     0.0f,  0.5f, 0.0f   // Top-center
    };

    */
 




    float vertices[] = {
        0.5f, 0.5f, 0.5f, 1.0 , 1.0 , 1.0,
        -0.5f, 0.5f, -0.5f,1.0 , 1.0 , 1.0,
        -0.5f, 0.5f, 0.5f, 1.0 , 0.0, 0.0,
        0.5f, -0.5f, -0.5f,  1.0 , 0.0, 0.0,
        -0.5f, -0.5f, -0.5f,  1.0 , 0.0, 0.0,
        0.5f, 0.5f, -0.5f,  1.0 , 0.0, 0.0,
        0.5f, -0.5f, 0.5f,  1.0 , 0.0, 0.0,
        -0.5f, -0.5f, 0.5f,  1.0 , 0.0, 0.0,


    };
    unsigned int Indices[] = {
                          0, 1, 2,
                          1, 3, 4,
                          5, 6, 3,
                          7, 3, 6,
                          2, 4, 7,
                          0, 7, 6,
                          0, 5, 1,
                          1, 5, 3,
                          5, 0, 6,
                          7, 4, 3,
                          2, 1, 4,
                          0, 2, 7
    };






    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object
    glBindVertexArray(VAO);

    // Bind the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);



    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    static float Scale = 0.0f;
    static float Delta = 1.0f;


    const double PI = 3.14159265358979323846;

    float fieldOfView = 90;
    float tanOfHalfAlpha = tanf((PI / 180) * (fieldOfView / 2));
    float d = 1 / tanOfHalfAlpha;

    //float projection_matrix[16] = {
    //    d, 0.0f, 0.0f, 0.0f,
    //    0.0f, d, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f
    //};

    float projection_matrix_list[4][4] = {
        {d, 0.0f, 0.0f, 0.0f},
        {0.0f, d, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}
    };
    Matrix4x4 projection_matrix(projection_matrix_list , 4);

    std::cout << "projection matrix " << std::endl;
    projection_matrix.show_matrix();


    //float translation_matrix[16] = {
    //1.0, 0.0f, 0.0f, 0.0f,
    //0.0f, 1.0, 0.0f, 0.0f,
    //0.0f, 0.0f, 1.0f, 3.0f,
    //0.0f, 0.0f, 0.0f, 1.0f
    //};



    // temporary code 
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // need to highlight this section 

    WorldTransformation temp_transformation;

    temp_transformation.set_translation_relative(0, 0, 3.0);
    //std::cout << "translation matrix" << "\n";
    //temp_transformation.get_linear_transformation_matrix().show_matrix();
   


    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        Scale = Delta;

        temp_transformation.set_rotation_relative(0, Scale, 0);

        Matrix4x4 world_transformations = temp_transformation.get_linear_transformation_matrix();
        Matrix4x4 view_space = world_camera.get_change_of_basis_matrix_view_space();

        Matrix4x4 clip_space = projection_matrix * view_space * world_transformations;
        
       
        glUniformMatrix4fv(all_matrix_multiplied_location, 1, GL_TRUE, clip_space.matrix);
        /*glUniform1f(gScaleLocation, 1);*/
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}






