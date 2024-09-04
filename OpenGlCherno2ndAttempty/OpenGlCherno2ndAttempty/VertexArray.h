#pragma once
#include "Renderer.h"
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <vector>
class VertexArray 
{
private:
	unsigned int m_Renderer_Id{};
public:
	VertexArray() 
	{
		GLCall(glGenVertexArrays(1, &(this->m_Renderer_Id)));
		this->bind();
	}
	~VertexArray() 
	{
		GLCall(glDeleteVertexArrays(1 , &this->m_Renderer_Id));
	}
	void addBuffer(VertexBuffer& vb, VertexBufferLayout& vbLayout) 
	{
		vb.bind();
		const std::vector<VertexBufferElement>& temp = vbLayout.getElements();
		int offset = 0;
		for (int i = 0; i < temp.size(); i++) 
		{
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, temp[i].count, temp[i].type, temp[i].normalized, vbLayout.getStried(), (const void*)offset));
			offset += temp[i].count * VertexBufferElement::getSizeOfType(temp[i].type);
			
		}

	}

	void bind() 
	{
		GLCall(glBindVertexArray(this->m_Renderer_Id));
	}
	void unBind()
	{
		GLCall(glBindVertexArray(0));
	}


};