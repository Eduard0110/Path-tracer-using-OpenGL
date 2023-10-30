#include "object.h"
#include "scene.h"


void Scene::convertTo1D() {
	// x, y, z, r, g, b, type, radius, cubeSize, isLight, reflectivity, refract, percentSpecular, roughness powerOfLight rotationTransform
	// 1  2  3  4  5  6    7     8     9 10 11     12         13          14          15             16       17            18 19 20
	float array2D[MAX_OBJECTS][ELMENTS_IN_1OBJ];
	for (int i = 0; i < MAX_OBJECTS; i++) {
		array2D[i][0] = objects[i].pos[0];
		array2D[i][1] = objects[i].pos[1];
		array2D[i][2] = objects[i].pos[2];

		array2D[i][3] = objects[i].colour[0];
		array2D[i][4] = objects[i].colour[1];
		array2D[i][5] = objects[i].colour[2];

		array2D[i][6] = objects[i].type;
		array2D[i][7] = objects[i].radius;

		array2D[i][8] = objects[i].cubeSize[0];
		array2D[i][9] = objects[i].cubeSize[1];
		array2D[i][10] = objects[i].cubeSize[2];

		array2D[i][11] = objects[i].isLight;
		array2D[i][12] = objects[i].reflectivity;
		array2D[i][13] = objects[i].refractionIndex;
		array2D[i][14] = objects[i].percentSpecular;
		array2D[i][15] = objects[i].roughness;
		array2D[i][16] = objects[i].powerOfLight;

		array2D[i][17] = objects[i].rot[0];
		array2D[i][18] = objects[i].rot[1];
		array2D[i][19] = objects[i].rot[2];

		array2D[i][20] = objects[i].specularColour[0];
		array2D[i][21] = objects[i].specularColour[1];
		array2D[i][22] = objects[i].specularColour[2];
	}

	for (int i = 0; i < MAX_OBJECTS; ++i) {
		for (int j = 0; j < ELMENTS_IN_1OBJ; ++j) {
			sceneData1D[i * ELMENTS_IN_1OBJ + j] = array2D[i][j];
		}
	}
}

void Scene::updateObjectsFrom1D() {
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		int index = i * ELMENTS_IN_1OBJ; // Calculate the starting index for each object in the 1D array

		objects[i].pos[0] = sceneData1D[index + 0];
		objects[i].pos[1] = sceneData1D[index + 1];
		objects[i].pos[2] = sceneData1D[index + 2];

		objects[i].colour[0] = sceneData1D[index + 3];
		objects[i].colour[1] = sceneData1D[index + 4];
		objects[i].colour[2] = sceneData1D[index + 5];

		objects[i].type = sceneData1D[index + 6];
		objects[i].radius = sceneData1D[index + 7];

		objects[i].cubeSize[0] = sceneData1D[index + 8];
		objects[i].cubeSize[1] = sceneData1D[index + 9];
		objects[i].cubeSize[2] = sceneData1D[index + 10];

		objects[i].isLight = sceneData1D[index + 11];
		objects[i].reflectivity = sceneData1D[index + 12];
		objects[i].refractionIndex = sceneData1D[index + 13];
		objects[i].percentSpecular = sceneData1D[index + 14];
		objects[i].roughness = sceneData1D[index + 15];
		objects[i].powerOfLight = sceneData1D[index + 16];

		objects[i].rot[0] = sceneData1D[index + 17];
		objects[i].rot[1] = sceneData1D[index + 18];
		objects[i].rot[2] = sceneData1D[index + 19];

		objects[i].specularColour[0] = sceneData1D[index + 20];
		objects[i].specularColour[1] = sceneData1D[index + 21];
		objects[i].specularColour[2] = sceneData1D[index + 22];
	}
}

void Scene::rearrangeObjects() {
	int writeIndex = 0;

	// Move objects with type != 0 towards the front
	for (int readIndex = 0; readIndex < MAX_OBJECTS; ++readIndex) {
		if (objects[readIndex].type != 0) {
			objects[writeIndex] = objects[readIndex];
			++writeIndex;
		}
	}

	// Fill the rest of the array with objects of type 0
	while (writeIndex < MAX_OBJECTS) {
		Object obj;
		objects[writeIndex] = obj; // Set type to 0 for remaining elements
		++writeIndex;
	}
}

void Scene::saveInFile(const char* dirName) {
	// create new directory
	if (std::filesystem::create_directory(dirName)) std::cout << "Folder created successfully." << std::endl;
	else std::cerr << "Failed to create folder." << std::endl;
	
	// Write the 1d array to a file
	int arraySize = sizeof(sceneData1D) / sizeof(sceneData1D[0]);
	std::ofstream ArrayFile1d(std::format("{}/data.txt", dirName).c_str());
	if (!ArrayFile1d) std::cerr << "Error opening file for writing." << std::endl;
	for (int i = 0; i < arraySize; ++i) {
		ArrayFile1d << sceneData1D[i] << " ";
	}
	ArrayFile1d.close();

	// write the object names to a file
	std::ofstream objectsFile(std::format("{}/objects.txt", dirName).c_str());
	arraySize = sizeof(objects) / sizeof(objects[0]);
	for (int i = 0; i < arraySize; i++) {
		objectsFile << objects[i].name << std::endl;
	}
	objectsFile.close();
}

void Scene::readFromAfile(const char* dirName) {
	// Read the array from the file and store it in sceneData1D
	const int maxSize = MAX_OBJECTS * ELMENTS_IN_1OBJ;
	int loadedSize = 0;
	std::ifstream ArrayFile1d(std::format("{}/data.txt", dirName).c_str());
	if (!ArrayFile1d) std::cerr << "Error opening file for reading." << std::endl;
	float value;
	while (ArrayFile1d >> value && loadedSize < maxSize) {
		sceneData1D[loadedSize] = value;
		loadedSize++;
	}
	ArrayFile1d.close();

	// read objects names
	std::ifstream objectsFile(std::format("{}/objects.txt", dirName).c_str());
	std::string line;
	int count = 0;
	while (std::getline(objectsFile, line)) {
		objects[count].name = line;
		count++;
	}
}