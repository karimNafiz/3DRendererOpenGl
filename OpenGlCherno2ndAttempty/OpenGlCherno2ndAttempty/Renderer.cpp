
#include <iostream>
#include "Renderer.h"

void glClearError()
{

    while (glGetError != GL_NO_ERROR);
}

bool glLogCall()
{

    GLenum error = glGetError();
    while (error != GL_NO_ERROR)
    {
        std::cout << "GLERROR code " << error << std::endl;
        return false;
    }
    return true;
}


