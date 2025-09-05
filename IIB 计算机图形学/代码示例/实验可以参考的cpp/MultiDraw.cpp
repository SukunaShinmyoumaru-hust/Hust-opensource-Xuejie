//////////////////////////////////////////////////////////////////////////////
//
//  MultiDraw.cpp
//  1. 增加EBO的使用
//  2. 增加了图元重启操作
//  3. 增加了视区和窗口宽高比设置问题处理
//  4. 增加了多个实例绘制的处理
//////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vmath.h>

//窗口参数设置
float aspect=4.0f/3.0f;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//增加旋转参数
static GLfloat xRot = 20.0f;
static GLfloat yRot = 20.0f;

//句柄参数
GLuint vertex_array_object; // == VAO句柄
GLuint vertex_buffer_object;// == VBO句柄
GLuint element_buffer_object; // == EBO句柄
int shader_program;//着色器程序句柄

//属性参数
int USE_PRIMITIVE_RESTART = 0; //是否使用图元重启

void initial(void)
{
	//立方体数据信息
	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// 立方体的顶点颜色
	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};

	//立方体绘制时的顶点索引
	static const GLushort cube_indices[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,         //第一个循环
		0xFFFF,                         // 终止符
		2, 6, 0, 4, 1, 5, 3, 7          //第二个循环
	};

	//生成并绑定EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// 生成并绑定VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	
	//生成并绑定VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)sizeof(cube_positions));
	glEnableVertexAttribArray(1);

	// 顶点着色器和片段着色器源码
	const char *vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec4 vPos;\n"           // 位置变量的属性位置值为0
		"layout (location = 1) in vec4 vColor;\n"        // 颜色变量的属性位置值为1
		"out vec4 color;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transform * vPos;\n"
		"    color = vColor;\n"
		"}\n\0";
	const char *fragment_shader_source =
		"#version 330 core\n"
		"in vec4 color;\n"                           // 输入的颜色向量
		"out vec4 FragColor;\n"                   // 输出的颜色向量
		"void main()\n"
		"{\n"
		"    FragColor = color;\n"
		"}\n\0";

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

	//设定点线面的属性
	glPointSize(15);//设置点的大小
	glLineWidth(15);//设置线宽
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_UP:
		xRot -= 5.0f;
		break;
	case GLFW_KEY_DOWN:
		xRot += 5.0f;
		break;
	case GLFW_KEY_LEFT:
		yRot -= 5.0f;
		break;
	case GLFW_KEY_RIGHT:
		yRot += 5.0f;
		break;
	case GLFW_KEY_1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  //设置多边形为点模式
		break;
	case GLFW_KEY_2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //设置多边形为线模式
		break;
	case GLFW_KEY_3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //设置多边形为填充模式
		break;
	case GLFW_KEY_4:
		glDisable(GL_CULL_FACE);     //关闭背面剔除
		break;
	case GLFW_KEY_5:
		glEnable(GL_CULL_FACE);    //打开背面剔除
		glCullFace(GL_BACK);          //剔除多边形的背面
		break;
	case GLFW_KEY_6:
		glFrontFace(GL_CW);          //多边形绕法为顺时针
		break;
	case GLFW_KEY_7:
		glFrontFace(GL_CCW);       //多边形绕法为逆时针
		break;
	case GLFW_KEY_8:
		USE_PRIMITIVE_RESTART = 0;       //关闭图元重启
		break;
	case GLFW_KEY_9:
		USE_PRIMITIVE_RESTART = 1;       //打开图元重启
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

	//处理图形的旋转
	vmath::mat4 trans = vmath::perspective(70, aspect, 1.0f, 500.0f)*vmath::translate(0.0f,0.0f,-5.0f) *vmath::rotate(xRot, vmath::vec3(1.0, 0.0, 0.0)) * vmath::rotate(yRot, vmath::vec3(0.0, 1.0, 0.0));
	unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);

	glBindVertexArray(vertex_array_object);                                    // 绑定VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);  // 绑定EBO

	if (USE_PRIMITIVE_RESTART == 0)
	{
		//不使用图元重启绘制立方体
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));
		
		/*
		//绘制多个同样的立方体
		trans = vmath::perspective(70, aspect, 1.0f, 500.0f)*vmath::translate(-5.0f, 0.0f, -10.0f) *vmath::rotate(xRot, vmath::vec3(1.0, 0.0, 0.0)) * vmath::rotate(yRot, vmath::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));

		trans = vmath::perspective(70, aspect, 1.0f, 500.0f)*vmath::translate(5.0f, 0.0f, -10.0f) *vmath::rotate(xRot, vmath::vec3(1.0, 0.0, 0.0)) * vmath::rotate(yRot, vmath::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));
		*/
	}
	else
	{
		//使用图元重启绘制立方体
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
	}
	glBindVertexArray(0);                                                      // 解除绑定

}

void reshaper(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void Finalize(void)
{
	glUseProgram(0);
	glDeleteProgram(shader_program);
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);
}

int main()
{
	glfwInit(); // 初始化GLFW

	// OpenGL版本为3.3，主次版本号均设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	// 使用核心模式(无需向后兼容性)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口(宽、高、窗口名称)
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MultiDraw", NULL, NULL);

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

	initial();

	//窗口大小改变时调用函数reshaper
	glfwSetFramebufferSizeCallback(window, reshaper);

	//窗口键盘事件调用函数key_callback
	glfwSetKeyCallback(window, key_callback);

	std::cout << "方向键可以控制图形的旋转。" << std::endl;
	std::cout << "数字键1，2，3设置多边形模式为点模式、线模式和填充模式。" << std::endl;
	std::cout << "数字键4关闭剔除模式。" << std::endl;
	std::cout << "数字键5打开剔除模式并且剔除多边形的背面。" << std::endl;
	std::cout << "数字键6，7设置多边形绕法为顺时针或者逆时针。" << std::endl;
	std::cout << "数字键8，9设置不使用或者使用图元重启绘制立方体。" << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Finalize();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
