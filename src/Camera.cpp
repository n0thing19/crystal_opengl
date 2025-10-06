#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Inisialisasi matriks view dan projection
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Mengatur kamera untuk melihat ke arah yang benar dari posisi yang benar
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Menambahkan perspektif ke scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Mengatur matriks kamera yang baru
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Mengirim matriks kamera ke shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
	float currentSpeed = speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		currentSpeed *= 2.0f; // Kecepatan lari 2x lipat
	}

	// Menangani input keyboard
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += currentSpeed * deltaTime * -Up;
	}


	// Menangani input mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Sembunyikan kursor mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Mencegah kamera meloncat saat klik pertama
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Menyimpan koordinat kursor
		double mouseX;
		double mouseY;
		// Mengambil koordinat kursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalisasi dan ubah koordinat menjadi derajat rotasi
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Menghitung perubahan orientasi vertikal
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Batasi rotasi vertikal agar tidak terbalik
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotasi orientasi ke kiri dan kanan
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Atur kembali kursor ke tengah layar
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Tampilkan kembali kursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Pastikan klik berikutnya tidak membuat kamera meloncat
		firstClick = true;
	}
}

