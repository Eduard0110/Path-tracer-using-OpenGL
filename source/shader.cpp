#include "shader.h"

const char* Shader::fileRead(const char* fileName) {
	std::ifstream file(fileName, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		std::cerr << "Error opening the file: " << fileName << std::endl;
		return nullptr;
	}

	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[fileSize + 1]; // +1 for null terminator
	file.read(buffer, fileSize);
	buffer[fileSize] = '\0'; // Null-terminate the C-style string

	file.close();

	return buffer;
}

void Shader::compileShader(GLuint shader, const char* shaderType) {
	glCompileShader(shader);
	GLint shaderCompileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompileStatus);
	if (shaderCompileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		std::cout << shaderType << " Shader Compilation Error : " << infoLog << std::endl;
		delete[] infoLog;
	}
}

void Shader::U1f(const char* uName, float uValue) {
	glUniform1f(glGetUniformLocation(ID, uName), static_cast<GLfloat>(uValue));
}

void Shader::U2f(const char* uName, float uValue, float uValue2) {
	glUniform2f(glGetUniformLocation(ID, uName), static_cast<GLfloat>(uValue), static_cast<GLfloat>(uValue2));
}

void Shader::U3f(const char* uName, float uValue, float uValue2, float uValue3) {
	glUniform3f(glGetUniformLocation(ID, uName), static_cast<GLfloat>(uValue), static_cast<GLfloat>(uValue2), static_cast<GLfloat>(uValue3));
}

void Shader::U1i(const char* uName, int uValue) {
	glUniform1i(glGetUniformLocation(ID, uName), uValue);
}

void Shader::U1fv(const char* uName, int uValue, float uValue2[]) {
	glUniform1fv(glGetUniformLocation(ID, uName), uValue, uValue2);
}

void Shader::Active() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}
