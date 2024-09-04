#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum renderType)
{
	GLCall(glGenBuffers(1, &(this->m_Renderer_Id)));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_Renderer_Id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, renderType));
}
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &(this->m_Renderer_Id)));
}

void VertexBuffer::bind() 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_Renderer_Id));
}

void VertexBuffer::unBind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}