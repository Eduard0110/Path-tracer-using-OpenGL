#pragma once

#include <format>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "scene.h"
#include "skybox.h"
#include "GLFW/glfw3.h"
#include "camera.h"


class MyGui {
public:
	// camera settings
	bool renderMode = false; // false stands for performance mode, when true for render mode
	int MAX_REFLECTIONS_RENDER_MODE = 12;
	int NUMBER_OF_SAMPLES_RENDER_MODE = 5;
	int MAX_REFLECTIONS_PREVIEW_MODE = 1;
	int NUMBER_OF_SAMPLES_PREVIEW_MODE = 1;

	float colourMultiplier = 1.0f;  // colour multiplier when the a ray has reaches the maximum number of reflections
	float focusDistance = 5.0f;
	float apertureSize = 0.1f;
	float zoom = 1.0f;

	bool showNormals = false;
	bool onConstantUpdate = true;

	// post process
	float expousure = 0.0f;
	float brightness = 0.0f;
	float contrast = 0.0f;

	bool doBloom = false;
	float bloomIntensity = 2.0f;
	int bloomMaxLod = 4;
	int numberOfBounces = 4;
	float tresholdIntensity = 1.0f;

	// plane grid settings
	bool planeGrid = false;
	float gridCol2[3] = { 0.0f, 0.0f, 0.0f };
	float tileSize = 2.0f;

	// imgui
	bool vSync = true;
	bool objectProperties = false;
	bool addObjectName = false;
	bool hideGUI = false;
	bool showMessage = false;
	bool loadImageError = false;
	bool changeSkybox = false;
	bool postProcessSettings = false;
	bool isSaveFile = false;
	bool isReadFile = false;
	bool isMouseControl = true;
	bool useSkyboxColour = false;
	const char* items[3] = { "Sphere", "Cube", "Plane" };
	const char* materials[3] = { "Matte - Specular", "Light source", "Lens" };
	int selectedItemFromCombo = 0;
	int nextObjectToAdd = 0;
	int selectedObjectFromCollection = 0;
	int selectedMaterialFromCombo = 0;
	char inputBuffer[20] = "";  // buffer for an object name
	char inputBufferSaveFile[20] = "";  // buffer for a scene name
	int WIDTH, HEIGHT, fps, frameCount = 0;

	// for FPS measurements
	double previousTime = glfwGetTime();

	GLFWwindow* windowP;
	Scene* scene;
	Skybox* skybox;
	Camera* camera;

	MyGui(Scene* sceneP, Skybox* skyboxP, Camera* camera, int WIDTH, int HEIGHT, GLFWwindow* window);

	void calcFps();

	void sceneCollectionWindow();
	void inputWindow();
	void skyboxWindow();
	void objectPropertiesWindow();
	void cameraSettingsWindow();
	void postProcessWindow();
	void screenshotInfo();
	void skyboxError();
	void readFileWindow();
	void saveFileWindow();

	void renderMyGui();

	void Update();

};
