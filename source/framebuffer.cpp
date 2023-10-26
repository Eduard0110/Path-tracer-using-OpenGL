#include "framebuffer.h"

FrameBuffer::FrameBuffer(int attachments, GLuint tex1, GLuint tex2) {
	glGenFramebuffers(1, &buf);
	glBindFramebuffer(GL_FRAMEBUFFER, buf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);
	if (attachments == 2) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex2, 0);
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, buf);
}

void FrameBuffer::Delete() {
	glDeleteFramebuffers(1, &buf);
}