#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class RectVAO {
public:
	GLuint VAO, VBO;

	RectVAO();

	void Delete();
};