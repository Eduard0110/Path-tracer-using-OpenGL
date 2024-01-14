#include "skybox.h"


Skybox::Skybox(int WIDTH, int HEIGHT) {
	float* newBytes = stbi_loadf(std::format("images/skybox/{}", skybox_file_name).c_str(), &widthImg, &heightImg, &numColCh, 0);
	glGenTextures(1, &Texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, widthImg, heightImg, 0, GL_RGB, GL_FLOAT, newBytes);
	stbi_image_free(newBytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Skybox::ChangeTexture(const char* inputBuffer, bool& loadImageError, bool isHdrFileType) {
	// Unload the previous texture if it exists
	if (Texture != 0) {
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}
	skybox_file_name = inputBuffer;
	path_to_skybox = "images/skybox/" + skybox_file_name;
	// Load the new texture
	if (isHdrFileType) {
		float* newBytes = stbi_loadf(path_to_skybox.c_str(), &widthImg, &heightImg, &numColCh, 0);
		if (newBytes == NULL) {
			loadImageError = true;
		}
		else {
			glGenTextures(1, &Texture);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, widthImg, heightImg, 0, GL_RGB, GL_FLOAT, newBytes);
			stbi_image_free(newBytes);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	else {
		unsigned char* newBytes = stbi_load(path_to_skybox.c_str(), &widthImg, &heightImg, &numColCh, 0);
		if (newBytes == NULL) {
			loadImageError = true;
		}
		else {
			glGenTextures(1, &Texture);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, newBytes);
			stbi_image_free(newBytes);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Skybox::Delete() {
	glDeleteTextures(1, &Texture);
}

void Skybox::ActiveAndBind(GLuint textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, Texture);
}