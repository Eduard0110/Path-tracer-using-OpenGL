#pragma once

#include <glad/glad.h>

class FrameBuffer {
public:
	GLuint buf;
	FrameBuffer(int attachments, GLuint tex1, GLuint tex2);

	void Bind();
	void Delete();
};