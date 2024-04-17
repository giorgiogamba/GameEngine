// Copyright (c) Giorgio Gamba

#include "Public/Texture.h"

Texture::Texture(const char* texturePath)
{
	this->texturePath = texturePath;
	CreateTexture();
}

void Texture::CreateTexture()
{
	int texwidth, textheight;
	unsigned char* teximage = SOIL_load_image(this->texturePath, &texwidth, &textheight, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (teximage)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, textheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, teximage);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR: error while loading texture" << std::endl;
		return;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(teximage);
}

void Texture::ApplyTexture(GLuint program)
{
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unuse()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

