#include "mygui.h"
#include <format>
#if !defined(_MSC_VER)
    auto strcpy_s = strcpy;
#endif 

MyGui::MyGui(Scene* sceneP, Skybox* skyboxP, Camera* cameraP, int width, int height, GLFWwindow* window) {
	WIDTH = width;
	HEIGHT = height;
	windowP = window;
	scene = sceneP;
	skybox = skyboxP;
	camera = cameraP;

	// initialising ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;
		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f); //
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f);//
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f);//
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f);//
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f);//
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.19f, 0.22f, 1.00f);//
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 0.70f, 0.00f, 0.43f);
	}
}

void MyGui::calcFps() {
	double currentTime = glfwGetTime();
	frameCount++;
	if (currentTime - previousTime >= 1.0)
	{
		fps = frameCount / (currentTime - previousTime);
		previousTime = currentTime;
		frameCount = 0;
	}
}

void MyGui::sceneCollectionWindow() {
	// scene collection window
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Scene collection"), NULL, ImGuiWindowFlags_NoCollapse) {
		ImGui::Text(std::format("FPS: {}", fps).c_str());
		ImVec2 SceneWindowSize = ImGui::GetWindowContentRegionMax();
		float offset = 8.0f;

		for (int i = 0; i < MAX_OBJECTS; i++) {
			if (!scene->objects[i].name.empty()) {
				if (ImGui::Button(scene->objects[i].name.c_str(), ImVec2(SceneWindowSize.x - offset, 30))) {
					if (selectedObjectFromCollection == i) objectProperties = !objectProperties;
					else objectProperties = true;
					selectedObjectFromCollection = i;
					selectedItemFromCombo = (int)scene->objects[selectedObjectFromCollection].type - 1;
					selectedMaterialFromCombo = 0;
					if (scene->objects[selectedObjectFromCollection].isLight == 1.0) selectedMaterialFromCombo = 1;
					else if (scene->objects[selectedObjectFromCollection].refractionIndex != -1.0) selectedMaterialFromCombo = 2;
				}
			}
		}
		if (ImGui::Button("Add object", ImVec2(SceneWindowSize.x - offset, 30))) {
			addObjectName = !addObjectName;
		}
	} ImGui::End();
}

void MyGui::inputWindow() {
	ImGui::SetNextWindowPos(ImVec2(411, 230), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(384, 105), ImGuiCond_FirstUseEver);
	// input window
	if (addObjectName) {
		ImGui::Begin("Add a new object");
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();

		ImGui::InputText("Object name", inputBuffer, IM_ARRAYSIZE(inputBuffer));
		if (ImGui::Button("Submit", ImVec2(windowSize.x/2-10, 20))) {
			scene->objects[nextObjectToAdd].name = inputBuffer;
			scene->objects[nextObjectToAdd].type = 1.0;
			nextObjectToAdd++;
			// clean input buffer
			strcpy_s(inputBuffer, "");
			// close window
			addObjectName = false;
		}
		// close button
		ImGui::SameLine();
		if (ImGui::Button("Close", ImVec2(windowSize.x / 2 - 10, 20))) addObjectName = false;

		ImGui::End();
	}
}

void MyGui::skyboxWindow() {
	ImGui::SetNextWindowPos(ImVec2(405, 714), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(395, 360), ImGuiCond_FirstUseEver);
	if (changeSkybox) {
		if (ImGui::Begin("Skybox settings")) {
			ImGui::Text(std::format("Current image used: {}", skybox->skybox_file_name).c_str());
			if (ImGui::CollapsingHeader("Skybox rotation")) {
				ImGui::SliderFloat("Pitch (x)", &skybox->rotation[0], 0.0f, 360.0f);
				ImGui::SliderFloat("Yaw (y)", &skybox->rotation[1], 0.0f, 360.0f);
				ImGui::SliderFloat("Roll (z)", &skybox->rotation[2], 0.0f, 360.0f);
			}

			if (ImGui::CollapsingHeader("Use just a color")) {
				ImGui::Checkbox("Use a colour", &useSkyboxColour);
				if (useSkyboxColour) ImGui::ColorEdit3("Sky color", skybox->col);
			}
			ImGui::Text("Highest colour value");
			ImGui::SliderFloat(" ", &skybox->highestColValue, 1.0f, 100.0f);

			ImGui::Spacing();
			ImGui::Text("All images in 'images/skybox'");

			ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
			for (const auto& entry : std::filesystem::directory_iterator("images/skybox/")) {
				std::string fileName = entry.path().filename().string();
				// get the extension
				size_t pos = fileName.rfind('.');
				std::string fileType;
				if (pos != std::string::npos) {
					fileType = fileName.substr(pos + 1);
				}
				// if the extension is allowed
				if (fileType == "jpg" || fileType == "png" || fileType == "hdr"){
					if (ImGui::Button(fileName.c_str(), ImVec2(windowSize.x - 8, 30))) {
						skybox->ChangeTexture(fileName.c_str(), loadImageError, fileType == "hdr" ? true : false);
					}
				}
			}
			if (ImGui::Button("Close", ImVec2(windowSize.x - 8, 30))) changeSkybox = false;
			
		} ImGui::End();
	}
}

void MyGui::objectPropertiesWindow() {
	ImGui::SetNextWindowPos(ImVec2(1420, 376), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
	// object properties window
	if (objectProperties && !scene->objects[selectedObjectFromCollection].name.empty()) {
		std::string name = scene->objects[selectedObjectFromCollection].name;
		if (ImGui::Begin("Object properties", NULL)) {
			ImGui::Text(name.c_str());
			ImGui::Text("Object type");
			ImGui::Combo("  ", &selectedItemFromCombo, items, IM_ARRAYSIZE(items));
			ImGui::Text("Material type");
			ImGui::Combo(" ", &selectedMaterialFromCombo, materials, IM_ARRAYSIZE(materials));
			// sphere
			if (items[selectedItemFromCombo] == "Sphere") {
				scene->objects[selectedObjectFromCollection].type = 1.0;
				ImGui::Text("Size settings");
				ImGui::SliderFloat("Radius", &scene->objects[selectedObjectFromCollection].radius, 0.1f, 10.0f);
				ImGui::Text("Position settings");
				ImGui::SliderFloat("X", &scene->objects[selectedObjectFromCollection].pos[0], -10.0f, 10.0f);
				ImGui::SliderFloat("Y", &scene->objects[selectedObjectFromCollection].pos[1], -10.0f, 10.0f);
				ImGui::SliderFloat("Z", &scene->objects[selectedObjectFromCollection].pos[2], -10.0f, 10.0f);

			}
			// cube
			else if (items[selectedItemFromCombo] == "Cube") {
				scene->objects[selectedObjectFromCollection].type = 2.0;
				ImGui::Text("Size settings");
				ImGui::SliderFloat("Width", &scene->objects[selectedObjectFromCollection].cubeSize[0], 0.1f, 10.0f);
				ImGui::SliderFloat("Height", &scene->objects[selectedObjectFromCollection].cubeSize[1], 0.1f, 10.0f);
				ImGui::SliderFloat("Length", &scene->objects[selectedObjectFromCollection].cubeSize[2], 0.1f, 10.0f);
				ImGui::Text("Position settings");
				ImGui::SliderFloat("X", &scene->objects[selectedObjectFromCollection].pos[0], -10.0f, 10.0f);
				ImGui::SliderFloat("Y", &scene->objects[selectedObjectFromCollection].pos[1], -10.0f, 10.0f);
				ImGui::SliderFloat("Z", &scene->objects[selectedObjectFromCollection].pos[2], -10.0f, 10.0f);
				ImGui::Text("Rotation");
				ImGui::SliderFloat("Pitch (X)", &scene->objects[selectedObjectFromCollection].rot[0], 0.0f, 360.0f, "%.0f");
				ImGui::SliderFloat("Yaw   (Y)", &scene->objects[selectedObjectFromCollection].rot[1], 0.0f, 360.0f, "%.0f");
				ImGui::SliderFloat("Roll  (Z)", &scene->objects[selectedObjectFromCollection].rot[2], 0.0f, 360.0f, "%.0f");
			}
			// plane
			else if (items[selectedItemFromCombo] == "Plane") {
				scene->objects[selectedObjectFromCollection].type = 3.0;
				ImGui::Text("Position settings");
				ImGui::SliderFloat("Y", &scene->objects[selectedObjectFromCollection].pos[1], -10.0f, 10.0f);
			}
			// material
			ImGui::Text("Material settings");
			// Matte - Specular 
			if (materials[selectedMaterialFromCombo] == "Matte - Specular") {
				scene->objects[selectedObjectFromCollection].set_default_settings(0);
				ImGui::SliderFloat("Roughness", &scene->objects[selectedObjectFromCollection].roughness, 0.0f, 1.0f);
				ImGui::SliderFloat("percentSpecular", &scene->objects[selectedObjectFromCollection].percentSpecular, 0.0f, 1.0f);
			}
			// Light source
			else if (materials[selectedMaterialFromCombo] == "Light source") {
				scene->objects[selectedObjectFromCollection].set_default_settings(1);
				ImGui::SliderFloat("Power of light", &scene->objects[selectedObjectFromCollection].powerOfLight, 0.0f, 20.0f);
			}
			// Lens
			else if (materials[selectedMaterialFromCombo] == "Lens") {
				scene->objects[selectedObjectFromCollection].set_default_settings(2);
				ImGui::SliderFloat("Index of refraction", &scene->objects[selectedObjectFromCollection].refractionIndex, 1.0f, 4.0f);
				ImGui::SliderFloat("Reflectivity", &scene->objects[selectedObjectFromCollection].reflectivity, 0.0f, 1.0f);
			}
			// grid
			if (items[selectedItemFromCombo] == "Plane") ImGui::Checkbox("Plane grid", &planeGrid);
			if (planeGrid && items[selectedItemFromCombo] == "Plane") {
				ImGui::SliderFloat("Tile size", &tileSize, 0.0f, 5.0f);
				ImGui::ColorEdit3("Colour 1", scene->objects[selectedObjectFromCollection].colour);
				ImGui::ColorEdit3("Colour 2", gridCol2);
			} else ImGui::ColorEdit3("Colour", scene->objects[selectedObjectFromCollection].colour);
			if (items[selectedItemFromCombo] != "Plane") ImGui::ColorEdit3("Specular colour", scene->objects[selectedObjectFromCollection].specularColour);
			// delete button
			if (ImGui::Button("Delete")) {
				scene->objects[selectedObjectFromCollection].set_default_settingsALL();
				scene->rearrangeObjects();
				nextObjectToAdd -= 1;
			}
			// update
			scene->convertTo1D();
		} ImGui::End();
	}
}

void MyGui::cameraSettingsWindow() {
	ImGui::SetNextWindowPos(ImVec2(0, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(400, 470), ImGuiCond_FirstUseEver);
	// camera settings menu
	if (ImGui::Begin("Camera settings")) {
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
		ImVec2 buttonSize = ImVec2(windowSize.x - 8, 30);

		if (ImGui::CollapsingHeader("Path tracing settings")) {
			ImGui::Text("Number of reflections");
			ImGui::SliderInt("Render mode ", &MAX_REFLECTIONS_RENDER_MODE, 1, 30);
			ImGui::SliderInt("Preview mode", &MAX_REFLECTIONS_PREVIEW_MODE, 1, 30);
			ImGui::Text("Number of samples");
			ImGui::SliderInt("Redner mode", &NUMBER_OF_SAMPLES_RENDER_MODE, 1, 30);
			ImGui::SliderInt("Preview mode ", &NUMBER_OF_SAMPLES_PREVIEW_MODE, 1, 30);
			ImGui::Text("Color multiplier when reached max relfections");
			ImGui::SliderFloat("Multiplier", &colourMultiplier, 0.0, 1.0);
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Camera position and direction")) {
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &camera->Pos[0], -10.0f, 10.0f);
			ImGui::SliderFloat("Y", &camera->Pos[1], -10.0f, 10.0f);
			ImGui::SliderFloat("Z", &camera->Pos[2], -10.0f, 10.0f);
			ImGui::Text("Direction");
			ImGui::SliderFloat("Pitch (X)", &camera->Rot[0], -360.0f, 360.0f);
			ImGui::SliderFloat("Yaw   (Y)", &camera->Rot[1], -360.0f, 360.0f);
			ImGui::SliderFloat("Roll  (Z)", &camera->Rot[2], -360.0f, 360.0f);
			ImGui::Text("Other");
			ImGui::SliderFloat("Speed", &camera->Speed, 0.0f, 5.0f);
			ImGui::SliderFloat("Mouse sensitivity", &camera->Sensitivity, 0.1f, 5.0f);
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Camera settings")) {
			ImGui::Text("Lens settings");
			ImGui::SliderFloat("Focus distance", &focusDistance, 0.0, 30.0);
			ImGui::SliderFloat("Aperture size", &apertureSize, 0.0, 1.0);
			ImGui::SliderFloat("Zoom", &zoom, 0.1, 2.0);
		}
		ImGui::Spacing();
		ImGui::Checkbox("Show normals", &showNormals);
		ImGui::Checkbox("Camera movement", &isMouseControl);
		ImGui::Checkbox("Render mode  ", &renderMode);
		ImGui::Checkbox("On constant update", &onConstantUpdate);
		if (ImGui::Checkbox("v-Sync", &vSync)) glfwSwapInterval(vSync);;
		// change skybox
		ImGui::Spacing();
		if (ImGui::Button("Skybox settings", buttonSize)) changeSkybox = !changeSkybox;
		// change post process settings
		if (ImGui::Button("Post process settings", buttonSize)) postProcessSettings = !postProcessSettings;
		if (ImGui::Button("Save the scene", buttonSize)) isSaveFile = !isSaveFile;
		if (ImGui::Button("Load a scene", buttonSize)) isReadFile = !isReadFile;
		
	} ImGui::End();
}

void MyGui::screenshotInfo() {
	ImGui::SetNextWindowPos(ImVec2(776, 493), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(363, 94), ImGuiCond_FirstUseEver);
	// show message about successful screenshot
	if (showMessage) {
		ImGui::Begin("Info");
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();

		ImGui::Text("The screenshot has been saved successfully.");
		if (ImGui::Button("Ok", ImVec2(windowSize.x-8, 20))) showMessage = false;

		ImGui::End();
	}
}

void MyGui::skyboxError() {
	if (loadImageError)
	{
		ImGui::Begin("Error");

		ImGui::Text("An error has occurred while loading in the image.\nEnsure that you have specified the file name correctly and its format.");
		if (ImGui::Button("Okay")) loadImageError = false;

		ImGui::End();
	}
}

void MyGui::postProcessWindow() {
	ImGui::SetNextWindowPos(ImVec2(1420, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 370), ImGuiCond_FirstUseEver);
	if (postProcessSettings) {
		if (ImGui::Begin("Post process settings")) {

			ImGui::Text("Tonal settings");
			ImGui::SliderFloat("Brightness", &brightness, -1.0f, 1.0f);
			ImGui::SliderFloat("Expousure", &expousure, -1.0f, 1.0f);
			ImGui::SliderFloat("Contrast", &contrast, 0.0f, 1.0f);

			ImGui::Text("Bloom settings");
			ImGui::Checkbox("Bloom", &doBloom);
			ImGui::SliderInt("Max Lod", &bloomMaxLod, 0, 7);
			ImGui::SliderInt("Ping pong bounces", &numberOfBounces, 0, 10);
			ImGui::SliderFloat("Bloom intensity", &bloomIntensity, 0.0f, 10.0f);
			ImGui::SliderFloat("Intensity of treshold", &tresholdIntensity, 0.0f, 2.0f);

		} ImGui::End();
	}
}

void MyGui::readFileWindow() {
	ImGui::SetNextWindowPos(ImVec2(811, 848), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(515, 224), ImGuiCond_FirstUseEver);
	if (isReadFile) {
		ImGui::Begin("Load a scene");
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
		for (const auto& entry : std::filesystem::directory_iterator("scenes/")) {
			std::string fileName = entry.path().filename().string();

			if (ImGui::Button(fileName.c_str(), ImVec2(windowSize.x - 8, 30))) {
				scene->readFromAfile(std::format("scenes/{}", fileName.c_str()).c_str());
				scene->updateObjectsFrom1D();

				// get the index of last NOT empty object in list
				int i = 0;
				for (i = 0; i < MAX_OBJECTS; i++)
				{
					if (scene->objects[i].name.empty()) break;
				} nextObjectToAdd = i++;
			}
		}
		
		if (ImGui::Button("Close", ImVec2(windowSize.x - 8, 30)))
		{
			isReadFile = false;
		}

		ImGui::End();
	}
}


void MyGui::saveFileWindow() {
	ImGui::SetNextWindowPos(ImVec2(411, 342), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(384, 105), ImGuiCond_FirstUseEver);
	if (isSaveFile) {
		ImGui::Begin("Save a scene file");
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
		ImGui::InputText("Scene name", inputBufferSaveFile, IM_ARRAYSIZE(inputBufferSaveFile));

		if (ImGui::Button("Submit", ImVec2(windowSize.x/2-10, 20)))
		{
			scene->saveInFile(std::format("scenes/{}", inputBufferSaveFile).c_str());
			isSaveFile = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Close", ImVec2(windowSize.x/2-10, 20)))
		{
			isSaveFile = false;
		}

		ImGui::End();
	}
}

void MyGui::renderMyGui() {
	if (!hideGUI) {
		sceneCollectionWindow();
		inputWindow();
		skyboxWindow();
		objectPropertiesWindow();
		cameraSettingsWindow();
		postProcessWindow();
		readFileWindow();
		saveFileWindow();
	}
	screenshotInfo();
	skyboxError();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MyGui::Update() {
	calcFps();
	renderMyGui();
}
