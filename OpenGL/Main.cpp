#include <glew.h>
#include <freeglut.h>
#include <stdio.h>
#include <cstdlib> // For random colors

#include "./math_lib/3d_math.h"
#include "./RigidBody/3D_Objects.h"
#include "./RigidBody/Camera.h"

GLuint VBO, VAO, IBO;
GLuint ShaderProgram;
GLuint gWorldLocation;

Camera camera; // Global camera object for the scene
Matrix4x4 projectionMatrix{};
WorldTransformation cubeTransform{};
Matrix4x4 cubeTransformationMatrix{};

float rotation_scale = 0.0f;

struct Vertex {
    Vector4D pos;
    Vector4D color;

    Vertex() {}

    Vertex(float x, float y, float z) {
        pos = Vector4D(x, y, z, 1.0f);  // Homogeneous coordinates for positions

        // Random color generation
        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = Vector4D(red, green, blue, 1.0f);  // Homogeneous coordinates for colors
    }
};

// Cube vertices
Vertex Vertices[8];

// Cube indices for drawing
unsigned int Indices[] = {
    0, 1, 2, 1, 3, 4, 5, 6, 3, 7, 3, 6,
    2, 4, 7, 0, 7, 6, 0, 5, 1, 1, 5, 3,
    5, 0, 6, 7, 4, 3, 2, 1, 4, 0, 2, 7
};

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 fragColor;

uniform mat4 gWorld;

void main() {
    gl_Position = gWorld * position;
    fragColor = color;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec4 fragColor;
out vec4 color;

void main() {
    color = fragColor;
}
)";

// Compile and link shaders into a shader program
static void CompileShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check Vertex Shader Compilation
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex Shader Compilation Error: %s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check Fragment Shader Compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment Shader Compilation Error: %s\n", infoLog);
    }

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);

    // Check Program Linking
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        printf("Shader Program Linking Error: %s\n", infoLog);
    }

    glUseProgram(ShaderProgram);
    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Create the vertex buffer for the cube
static void CreateVertexBuffer() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

// Create the index buffer for the cube
static void CreateIndexBuffer() {
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

// Rendering callback function
static void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply transformations using WorldTransformation and Camera
    cubeTransform.rotate(0.0f, 0.001f, 0.0f);
    cubeTransformationMatrix = cubeTransform.getMatrix();
    Matrix4x4 worldMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * cubeTransformationMatrix;

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &(worldMatrix.m[0][0]));

    glBindVertexArray(VAO);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Initialize cube vertices with random colors
    Vertices[0] = Vertex(0.5f, 0.5f, 0.5f);
    Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f);
    Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f);
    Vertices[3] = Vertex(0.5f, -0.5f, -0.5f);
    Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f);
    Vertices[5] = Vertex(0.5f, 0.5f, -0.5f);
    Vertices[6] = Vertex(0.5f, -0.5f, 0.5f);
    Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f);

    // Set up camera and cube transformations
    camera = Camera();  // Assuming Camera is properly set up in Camera.h
    cubeTransform.translate(0.0f, 0.0f, 3.0f);  // Move cube in front of the camera

    // Initialize GLUT and GLEW
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("OpenGL Cube with VBO and VAO");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    CreateVertexBuffer();
    CreateIndexBuffer();
    CompileShaders();

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glutDisplayFunc(RenderSceneCB);
    glutMainLoop();

    return 0;
}
