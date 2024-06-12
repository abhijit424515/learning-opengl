#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headers/stb_image_write.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

void save_screenshot(GLFWwindow *window, int &width, int &height) {
  	glfwGetFramebufferSize(window, &width, &height);
  	unsigned char *pixels = new unsigned char[3 * width * height];
  	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width * 3; ++x) {
            std::swap(pixels[y * width * 3 + x], pixels[(height - 1 - y) * width * 3 + x]);
        }
    }

  	std::string filename = "output/screenshot.png";
  	stbi_write_png(filename.c_str(), width, height, 3, pixels, 0);
  	delete[] pixels;
}

int main(void) {
	if (!glfwInit())
		return -1;

  	int width = 640, height = 480;

  	GLFWwindow *window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cerr << "[Error] glewInit()" << std::endl;
		exit(1);
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// START

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		// END

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// save_screenshot(window, width, height);
	glfwTerminate();
}
