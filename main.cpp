#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

static uint compile_shader(unsigned int type, const std::string& source) {
	uint id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char message[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static uint create_shader(const std::string &vertex_shader, const std::string &fragment_shader) {
	uint program = glCreateProgram();
	uint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	uint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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

	// DATA

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	// BUFFERS

	uint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), (void*)positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

	std::string vs = R"(
		#version 330 core
		layout(location = 0) in vec4 position;

		void main() {
			gl_Position = position;
		}
	)";
	std::string fs = R"(
		#version 330 core
		layout(location = 0) out vec4 color;

		void main() {
			color = vec4(1.0, 0.0, 0.0, 1.0);
		}
	)";
	uint shader = create_shader(vs, fs);
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// START

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// END

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// save_screenshot(window, width, height);
	glDeleteProgram(shader);
	glfwTerminate();
}
