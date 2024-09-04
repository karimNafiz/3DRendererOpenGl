#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count , GLenum render_Type) 
{
	this->count = count;

	GLCall(glGenBuffers(1, &(this->m_Renderer_Id)));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (this->m_Renderer_Id)));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, render_Type));


}

IndexBuffer::~IndexBuffer() 
{
	GLCall(glDeleteBuffers(1, &(this->m_Renderer_Id)));
}

void IndexBuffer::bind() 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (this->m_Renderer_Id)));

}

void IndexBuffer::unBind() 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}