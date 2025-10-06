#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// Vektor utama kamera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 1.0f, 0.0f);  
	glm::vec3 Up = glm::vec3(0.0f, 0.0f, -1.0f);          
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Mencegah kamera meloncat saat klik pertama
	bool firstClick = true;

	// Menyimpan lebar dan tinggi window
	int width;
	int height;

	// Menyesuaikan kecepatan dan sensitivitas kamera
	float speed = 5.0f;
	float sensitivity = 100.0f;

	// Konstruktor kamera
	Camera(int width, int height, glm::vec3 position);

	// Memperbarui matriks kamera
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Mengirim matriks kamera ke shader
	void Matrix(Shader& shader, const char* uniform);
	// Menangani input kamera
	void Inputs(GLFWwindow* window, float deltaTime);
};
#endif

