#pragma once
#include <GL/glew.h>
class VertexBuffer
{
	private:
		unsigned int m_Renderer_Id{};
	public:
		VertexBuffer(const void* data, unsigned int size, GLenum renderType);
		~VertexBuffer();

		void bind();
		void unBind();
		

};

