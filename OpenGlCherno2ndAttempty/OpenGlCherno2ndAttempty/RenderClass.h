#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Render 
{
public:
    void clear() 
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void draw(VertexArray& va, IndexBuffer& ib, Shader& shader)
    {
        va.bind();
        ib.bind();
        shader.bind();

        GLCall(glDrawElements(GL_TRIANGLES, ib.count, GL_UNSIGNED_INT, nullptr));


    }

};
