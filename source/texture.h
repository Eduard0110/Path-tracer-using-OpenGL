#pragma once

#include <glad/glad.h>

class Texture {
public:
	GLuint tex;
	int WIDTH, HEIGHT;
	Texture(int width, int height);

	void ActiveAndBind(GLuint textureUnit);
	void Delete();
};