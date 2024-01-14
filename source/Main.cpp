#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "mygui.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <math.h>
#include <ctime>
#include <string>

#include "shader.h"
#include "camera.h"
#include "vao.h"
#include "skybox.h"
#include "scene.h"
#include "texture.h"
#include "framebuffer.h"

void updateRandomSeeds(double[3], double[3]);
void saveTextureAsImage(GLFWwindow*, GLuint, int, int, const char*);
void printOpenGLInfo();
bool checkKey(GLuint, GLuint&, GLFWwindow*);
std::string randomName(int);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// get the resolution of the monitor
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int WIDTH = mode->width;
	int HEIGHT = mode->height;

	// put glfwGetPrimaryMonitor() instead of the first NULL argument to create a full screen window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray tracing", glfwGetPrimaryMonitor(), NULL);
	// Error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);
	printOpenGLInfo();

	// create shaders
	Shader defaultShader("shaders/default.frag", "shaders/default.vert");
	Shader bloomShader("shaders/bloom.frag", "shaders/default.vert");
	Shader postProccesShader("shaders/postProcess.frag", "shaders/default.vert");
	Shader displayShader("shaders/display.frag", "shaders/default.vert");
	// create vao
	RectVAO rectVao;
	// camera
	float cameraPos[3] = { 0.0f, 1.0f, -3.0f };
	float cameraRotation[3] = { 0.0f, 0.0f, 0.0f };
	float cameraSpeed = 0.1f;
	float mouseSensitivity = 1.0f;
	Camera camera(window, cameraPos, cameraRotation, cameraSpeed, mouseSensitivity);

	// random seeds
	srand(static_cast<unsigned int>(time(0))); // init random with time
	double rSeed1[2], rSeed2[2];

	// variables to track if a button was pressed in previous frame
	GLuint keyRlastFrame = GLFW_RELEASE;
	GLuint keyElastFrame = GLFW_RELEASE;
	GLuint keyFlastFrame = GLFW_RELEASE;
	GLuint keyTlastFrame = GLFW_RELEASE;
	GLuint keyEscapelastFrame = GLFW_RELEASE;

	// skybox texture
	Skybox skybox = Skybox(WIDTH, HEIGHT);

	Texture texture1(WIDTH, HEIGHT);
	Texture texture2(WIDTH, HEIGHT);
	Texture bloomTexture(WIDTH, HEIGHT);
	Texture postProcessTexture(WIDTH, HEIGHT);

	FrameBuffer framebuffer1(2, texture1.tex, bloomTexture.tex);
	FrameBuffer framebuffer2(2, texture2.tex, bloomTexture.tex);
	FrameBuffer postProcessFBuffer(1, postProcessTexture.tex, 0);

	Scene scene;
	MyGui mygui(&scene, &skybox, &camera, WIDTH, HEIGHT, window);

	// frames without camera movement
	int framesStill = 0;

	// Create Ping Pong Framebuffers for repetitive blurring
	GLuint pingpongFBO[2];
	GLuint pingpongTexture[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongTexture);
	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
		glGenerateMipmap(GL_TEXTURE_2D);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[i], 0);
	}

	glfwSwapInterval(mygui.vSync);
	while (!glfwWindowShouldClose(window))
	{
		framesStill++;
		if (mygui.isMouseControl) { camera.CheckForUpdate(framesStill); camera.Update(); }

		updateRandomSeeds(rSeed1, rSeed2);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Texture currentTexture = (framesStill % 2 == 1) ? texture1 : texture2;
		Texture lastTexture = (framesStill % 2 == 0) ? texture1 : texture2;
		FrameBuffer currentFramebuffer = (framesStill % 2 == 1) ? framebuffer2 : framebuffer1;

		glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer.buf);
		defaultShader.Active();
		// pass in all the uniforms
		defaultShader.U2f("resolution", WIDTH, HEIGHT);
		defaultShader.U1f("framesStill", framesStill);
		defaultShader.U2f("rSeed1", rSeed1[0], rSeed1[1]);
		defaultShader.U2f("rSeed2", rSeed2[0], rSeed2[1]);
		defaultShader.U3f("CameraPosition", camera.Pos[0], camera.Pos[1], camera.Pos[2]);
		defaultShader.U3f("CameraRotation", camera.Rot[0], camera.Rot[1], camera.Rot[2]);
		defaultShader.U1f("treshHoldIntensity", mygui.tresholdIntensity);
		if (mygui.renderMode) {
			defaultShader.U1f("MAX_REFLECTIONS", mygui.MAX_REFLECTIONS_RENDER_MODE);
			defaultShader.U1f("NUMBER_OF_SAMPLES", mygui.NUMBER_OF_SAMPLES_RENDER_MODE);
		}
		else {
			defaultShader.U1f("MAX_REFLECTIONS", mygui.MAX_REFLECTIONS_PREVIEW_MODE);
			defaultShader.U1f("NUMBER_OF_SAMPLES", mygui.NUMBER_OF_SAMPLES_PREVIEW_MODE);
		}
		defaultShader.U1f("focusDistance", mygui.focusDistance);
		defaultShader.U1f("apertureSize", mygui.apertureSize);
		defaultShader.U1f("zoom", mygui.zoom);
		defaultShader.U1f("colorMultiplierWhenReachedMaxRef", mygui.colourMultiplier);
		defaultShader.U1i("showNormals", mygui.showNormals);
		if (!mygui.renderMode) defaultShader.U1fv("objects", MAX_OBJECTS * 20, scene.sceneData1D);
		defaultShader.U3f("skyboxRotation", mygui.skybox->rotation[0], mygui.skybox->rotation[1], mygui.skybox->rotation[2]);
		defaultShader.U1f("highestColValue", mygui.skybox->highestColValue);
		defaultShader.U1i("planeGrid", mygui.planeGrid);
		defaultShader.U3f("gridCol2", mygui.gridCol2[0], mygui.gridCol2[1], mygui.gridCol2[2]);
		defaultShader.U1f("tileSize", mygui.tileSize);
		defaultShader.U1i("useSkyboxColor", mygui.useSkyboxColour);
		defaultShader.U3f("skyboxColor", skybox.col[0], skybox.col[1], skybox.col[2]);
		defaultShader.U1i("skybox", 0);
		defaultShader.U1i("tex", 1);

		glBindVertexArray(rectVao.VAO);
		skybox.ActiveAndBind(GL_TEXTURE0);
		currentTexture.ActiveAndBind(GL_TEXTURE1);
		glDrawArrays(GL_TRIANGLES, 0, 6); // render


		// bloom
		// Bounce the image data around to blur multiple times
		bool horizontal = true, first_iteration = true;
		if (mygui.doBloom) {
			bloomShader.Active();
			for (int i = 0; i < mygui.numberOfBounces; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
				bloomShader.U1i("horizontal", horizontal);
				bloomShader.U1i("tex", 2);
				bloomShader.U1i("uMaxLod", mygui.bloomMaxLod);

				glActiveTexture(GL_TEXTURE2);
				if (first_iteration)
				{
					glBindTexture(GL_TEXTURE_2D, bloomTexture.tex);
					first_iteration = false;
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, pingpongTexture[!horizontal]);
				}
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glGenerateMipmap(GL_TEXTURE_2D);
				horizontal = !horizontal;
			}
		}

		// post procesing
		postProcessFBuffer.Bind();
		postProccesShader.Active();
		postProccesShader.U1i("tex", 3);
		postProccesShader.U1i("bloomTex", 4);
		postProccesShader.U1f("bloomIntensity", mygui.bloomIntensity);
		postProccesShader.U2f("resolution", WIDTH, HEIGHT);
		postProccesShader.U1f("brightness", mygui.brightness);
		postProccesShader.U1f("expousure", mygui.expousure);
		postProccesShader.U1f("contrast", mygui.contrast);
		lastTexture.ActiveAndBind(GL_TEXTURE3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, pingpongTexture[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// display texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		displayShader.Active();
		displayShader.U1i("tex", 5);
		displayShader.U2f("resolution", WIDTH, HEIGHT);
		postProcessTexture.ActiveAndBind(GL_TEXTURE5);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// update
		mygui.Update();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (mygui.onConstantUpdate && !mygui.renderMode) framesStill = 0;
		if (!mygui.addObjectName && !mygui.isSaveFile)
		{
			// change render mode
			if (checkKey(GLFW_KEY_R, keyRlastFrame, window)) { framesStill = 0; mygui.renderMode = !mygui.renderMode; }
			// check if the Escape button is pressed
			if (checkKey(GLFW_KEY_ESCAPE, keyEscapelastFrame, window)) break;
			// change mouse control
			if (checkKey(GLFW_KEY_E, keyElastFrame, window)) mygui.isMouseControl = !mygui.isMouseControl;
			// hide gui
			if (checkKey(GLFW_KEY_F, keyFlastFrame, window)) mygui.hideGUI = !mygui.hideGUI;
			// make screenshot
			if (checkKey(GLFW_KEY_T, keyTlastFrame, window))
			{
				mygui.showMessage = true;
				std::string path = "images/screenshots/" + randomName(8) + ".png";
				saveTextureAsImage(window, lastTexture.tex, WIDTH, HEIGHT, path.c_str());
			}
		}
	}

	// Delete all the objects we've created
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	rectVao.Delete();  // delete VAO
	defaultShader.Delete();  // delete Shaders
	bloomShader.Delete();
	postProccesShader.Delete();
	skybox.Delete();  // delete Textures
	texture1.Delete();
	texture2.Delete();
	bloomTexture.Delete();
	postProcessTexture.Delete();
	framebuffer1.Delete();  // delete framebuffers
	framebuffer2.Delete();
	postProcessFBuffer.Delete();
	glfwDestroyWindow(window);  // and delete window
	glfwTerminate();

	return 0;
}

void updateRandomSeeds(double rSeed1[3], double rSeed2[3]) {
	rSeed1[0] = rand() % 10000 / 100.0;
	rSeed1[1] = rand() % 10000 / 100.0;

	rSeed2[0] = rand() % 10000 / 100.0;
	rSeed2[1] = rand() % 10000 / 100.0;
}

void saveTextureAsImage(GLFWwindow* window, GLuint textureID, int width, int height, const char* filename) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned char* pixels = new unsigned char[width * height * 3];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Flip the pixel data vertically
	unsigned char* flippedPixels = new unsigned char[width * height * 3];
	for (int y = 0; y < height; ++y) {
		memcpy(flippedPixels + y * width * 3, pixels + (height - y - 1) * width * 3, width * 3);
	}

	stbi_write_png(filename, width, height, 3, flippedPixels, width * 3);

	delete[] pixels;
	delete[] flippedPixels;

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string randomName(int length) {
	const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	const int charsetLength = charset.length();

	std::string name;
	name.reserve(length);

	for (int i = 0; i < length; ++i) {
		name += charset[rand() % charsetLength];
	}

	return name;
}

void printOpenGLInfo() {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* shadingLanguageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;
	std::cout << "Vendor: " << vendor << std::endl;
	std::cout << "GLSL version: " << shadingLanguageVersion << std::endl;
}

bool checkKey(GLuint keyToBePressed, GLuint& keyLastFrame, GLFWwindow* window) {
	bool pressed = false;
	if (glfwGetKey(window, keyToBePressed) == GLFW_PRESS && !keyLastFrame == GLFW_PRESS) {
		pressed = true;
	}
	keyLastFrame = glfwGetKey(window, keyToBePressed);
	return pressed;
}
