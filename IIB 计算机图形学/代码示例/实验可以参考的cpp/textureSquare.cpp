
//////////////////////////////////////////////////////////////////////////////
//
// textureSquare.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vmath.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint vertex_array_object; // VAO句柄
GLuint vertex_buffer_object; // VBO句柄
GLuint texture_buffer_object; // 纹理对象句柄
GLuint shader_program;

void initial(void)
{
	// 定义图形对象的顶点数据和纹理坐标
	static const GLfloat quad_data[] =
	{
		-0.75f, -0.75f,
		 0.75f, -0.75f,
		 0.75f, 0.75f,
		-0.75f, 0.75f,
		//以下是对应的纹理坐标
		 0.0f, 0.0f,
		 2.0f, 0.0f,
		 2.0f, 2.0f,
		 0.0f, 2.0f
	};

	//定义纹理的内容（棋盘格）
	static const unsigned char texture_data[] =
	{
		0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
		0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
		0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
		0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
	};

	// 生成并绑定VAO和VBO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//创建和绑定纹理对象
	glGenTextures(1, &texture_buffer_object);
	glBindTexture(GL_TEXTURE_2D, texture_buffer_object);
	
	//指定纹理属性
	static const GLint swizzles[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //创建纹理对象内存并保存纹理内容
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 8,
		8, 0, GL_RED, GL_UNSIGNED_BYTE,
		texture_data);

	//生成Mipmap纹理
	glGenerateMipmap(GL_TEXTURE_2D);

	// 顶点着色器和片段着色器源码
	const char *vertex_shader_source =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec2 in_position;\n"
		"layout (location = 1) in vec2 in_tex_coord;\n"
		"\n"
		"out vec2 tex_coord;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = vec4(in_position, 0.0, 1.0);\n"
		"    tex_coord = in_tex_coord;\n"
		"}\n";
	const char *fragment_shader_source =
		"#version 330 core\n"
		"\n"
		"in vec2 tex_coord;\n"
		"layout (location = 0) out vec4 color;\n"
		"uniform sampler2D tex;\n"
		"void main(void)\n"
		"{\n"
		"    color = texture(tex, tex_coord);\n"
		"}\n";

	// 生成并编译着色器
	// 顶点着色器
	int success;
	char info_log[512];
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	// 检查着色器是否成功编译，如果编译失败，打印错误信息
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}
	// 片段着色器
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	// 检查着色器是否成功编译，如果编译失败，打印错误信息
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// 链接顶点和片段着色器至一个着色器程序
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	// 检查着色器是否成功链接，如果链接失败，打印错误信息
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	// 删除着色器
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(shader_program);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_1:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case GLFW_KEY_2:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	case GLFW_KEY_3:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case GLFW_KEY_4:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	default:
		break;
	}
}

void Draw(void)
{
	// 清空颜色缓冲
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 绘制正方形
	glBindVertexArray(vertex_array_object);                                    // 绑定VAO
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);                                                      // 解除绑定

}

void reshaper(GLFWwindow* window, int width, int height)
{
	// 指定当前视口尺寸(前两个参数为左下角位置，后两个参数是渲染窗口宽、高)
	glViewport(0, 0, width, height);

}

int main()
{
	glfwInit(); // 初始化GLFW

	// OpenGL版本为3.3，主次版本号均设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// 使用核心模式(无需向后兼容性)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口(宽、高、窗口名称)
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Square Texture", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 将窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	// 初始化GLAD，加载OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//显示操作说明
	std::cout << "方向键可以选择纹理的wrap参数。" << std::endl;
	std::cout << "数字键1设置为GL_CLAMP_TO_EDGE模式。" << std::endl;
	std::cout << "数字键2设置为GL_CLAMP_TO_BORDER模式。" << std::endl;
	std::cout << "数字键3设置为GL_REPEAT模式。" << std::endl;
	std::cout << "数字键4设置为GL_MIRRORED_REPEAT模式。" << std::endl;

	initial();

	//指定窗口大小改变时调用的函数reshaper
	glfwSetFramebufferSizeCallback(window, reshaper);

	//窗口键盘事件调用函数key_callback
	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{
		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 解绑VAO和VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 删除VAO和VBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	//解绑并删除纹理
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &texture_buffer_object);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
