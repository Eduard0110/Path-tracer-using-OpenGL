#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <format>

class Skybox {
public:
	GLuint Texture;
	std::string skybox_file_name = "rainforest_trail_2k.hdr";
	std::string path_to_skybox = "images/skybox/" + skybox_file_name;
	int widthImg, heightImg, numColCh;

	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float col[3] = { 0.5f, 0.5f, 0.5f };
	float highestColValue = 100.0f;

	Skybox(int WIDTH, int HEIGHT);

	void ChangeTexture(const char*, bool&, bool);

	void ActiveAndBind(GLuint textureUnit);
	void Delete();
};