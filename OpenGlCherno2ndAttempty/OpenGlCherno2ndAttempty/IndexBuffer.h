#pragma once
#include <GL/glew.h>

class IndexBuffer
{
	private:
		unsigned int m_Renderer_Id{};
		

	public:
		unsigned int count{};
		IndexBuffer(const unsigned int* data, unsigned int count, GLenum render_Type);
		~IndexBuffer();

		void bind();
		void unBind();


};

