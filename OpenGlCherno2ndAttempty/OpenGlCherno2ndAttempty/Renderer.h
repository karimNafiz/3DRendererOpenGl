#pragma once
#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) glClearError;\
    x;\
    glLogCall()


void glClearError();

bool glLogCall();







