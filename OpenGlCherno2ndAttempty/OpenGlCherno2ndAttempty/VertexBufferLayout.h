#pragma once
#include <vector>
#include <GL/glew.h>


struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) 
	{
		this->type = type;
		this->count = count;
		this->normalized = normalized;
	}

	static unsigned int getSizeOfType(unsigned int type) 
	{
		switch (type) 
		{
		case GL_UNSIGNED_INT:
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		return 0;
	}
};

class VertexBufferLayout 
{
private:
	unsigned int m_stride{};
	std::vector<VertexBufferElement> m_Elements;

public:
	template<typename T>
	void push(unsigned int count) { static_assert(false); }

	template<>
	void push<float>(unsigned int count) 
	{
		this->m_Elements.push_back(VertexBufferElement(GL_FLOAT, count, GL_FALSE));
		this->m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);

	}
	template<>
	void push<unsigned int>(unsigned int count)
	{
		this->m_Elements.push_back(VertexBufferElement(GL_UNSIGNED_INT, count, GL_FALSE));
		this->m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);

	}
	template<>
	void push<unsigned char>(unsigned int count)
	{
		this->m_Elements.push_back(VertexBufferElement(GL_UNSIGNED_BYTE, count, GL_TRUE));
		this->m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);

	}
	inline const std::vector<VertexBufferElement>& getElements() const { return this->m_Elements; }
	inline unsigned int getStried() { return this->m_stride; }



};