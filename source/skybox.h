#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <string>
#include <iostream>

class Skybox {
public:
	GLuint Texture;
	std::string skybox_file_name = "small_cave_2k.hdr";
	std::string path_to_skybox = "images/skybox/" + skybox_file_name;
	int widthImg, heightImg, numColCh;

	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float col[3] = { 0.5f, 0.5f, 0.5f };

	Skybox(int WIDTH, int HEIGHT);

	void ChangeTexture(const char*, bool&, bool);

	void ActiveAndBind(GLuint textureUnit);
	void Delete();
};