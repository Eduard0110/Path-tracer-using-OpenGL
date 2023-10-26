#include "camera.h"


Camera::Camera(GLFWwindow* window, float position[3], float rotation[3], float speed, float sensitivity) {
	Pos[0] = position[0];
	Pos[1] = position[1];
	Pos[2] = position[2];
	Rot[0] = rotation[0];
	Rot[1] = rotation[1];
	Rot[2] = rotation[2];
	Speed = speed;
	Window = window;
	Sensitivity = sensitivity;
	glfwGetWindowSize(Window, &WIDTH, &HEIGHT);
}

void Camera::rotateCamera() {
	// mouse position
	double xPos, yPos;
	glfwGetCursorPos(Window, &xPos, &yPos);

	// normalized mouse position
	float normalizedX = (xPos - WIDTH / 2.0f) / WIDTH;
	float normalizedY = (yPos - HEIGHT / 2.0f) / HEIGHT;

	float x = normalizedX * 360.0f * Sensitivity;
	float y = normalizedY * 180.0f * Sensitivity;

	if (x != Rot[0] || y != Rot[1]) isUpdate = true;
	Rot[0] = x;
	Rot[1] = y;
}

void Camera::moveCamera() {
	float cos_a = cos(Rot[0] * (3.141592f / 180.0f));
	float sin_a = sin(Rot[0] * (3.141592f / 180.0f));

	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) { // forward
		Pos[2] += Speed * cos_a;
		Pos[0] += Speed * sin_a;
		isUpdate = true;
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) { // backward
		Pos[2] -= Speed * cos_a;
		Pos[0] -= Speed * sin_a;
		isUpdate = true;
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) { // right
		Pos[2] -= Speed * sin_a;
		Pos[0] += Speed * cos_a;
		isUpdate = true;
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) { // left
		Pos[2] += Speed * sin_a;
		Pos[0] -= Speed * cos_a;
		isUpdate = true;
	}
	if (glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { // down
		Pos[1] -= Speed;
		isUpdate = true;
	}
	if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS) { // up
		Pos[1] += Speed;
		isUpdate = true;
	}
}

void Camera::CheckForUpdate(int &updateVariable) {
	if (isUpdate) {
		updateVariable = 1;
		isUpdate = false;
	}
}

void Camera::Update() {
	rotateCamera();
	moveCamera();
}