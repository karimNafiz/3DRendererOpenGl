#pragma once
#include "Renderer.h"
#include<string>
#include "stb_image.h"


class Texture
{
private:
	std::string filePath;
	unsigned char* mLocalBuffer{};
	unsigned int mRenderedID;
	int mWidth, mHeight, mBitsPerPixel;

public :
	Texture(const std::string& filePath) 
	{
		this->filePath = filePath;
		stbi_set_flip_vertically_on_load(1);
		this->mLocalBuffer = stbi_load(this->filePath.c_str(), &this->mWidth, &this->mHeight, &this->mBitsPerPixel, 4);
		GLCall(glGenTextures(1, &this->mRenderedID));
		GLCall(glBindTexture(GL_TEXTURE_2D, this->mRenderedID));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)); // Wrap horizontally
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)); // Wrap vertically
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // Minify filter
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // Magnify filter

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->mLocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		if (this->mLocalBuffer == nullptr) 
		{
			stbi_image_free(this->mLocalBuffer);
		}
	}
	~Texture() 
	{

		GLCall(glDeleteTexture(1, &this->mRenderedID));
	}


		void Bind(unsigned int slot = 0);
		void unBind();

		inline int getHeight() { return this->mHeight; }
		inline int getWidht() { return this->mWidth; }

};
