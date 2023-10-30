#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>


class Camera {
public:
	float Pos[3];
	float Rot[3];
	float Speed;
	float Sensitivity;
	GLFWwindow* Window;
	bool isUpdate = false;
	int WIDTH, HEIGHT;

	Camera(GLFWwindow* window, float position[3], float rotation[3], float speed, float sensitivity);

	void rotateCamera();

	void moveCamera();

	void CheckForUpdate(int&);

	void Update();
};
