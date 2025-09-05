//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//  1. 增加颜色部分的处理
//  2.增加图元的属性
//  3. 增加键盘操作
//////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vmath.h>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint vertex_array_object; // VAO句柄
GLuint vertex_buffer_object; // VBO句柄
GLuint shader_program;

//增加旋转参数
static GLfloat xRot = 20.0f;
static GLfloat yRot = 20.0f;

void initial(void)
{
	// 定义图形对象的顶点数据
	const  float triangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f,  -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	// 定义图形对象的颜色数据，与顶点匹配
	const  float color[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	// 生成并绑定VAO和VBO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle) + sizeof(color), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangle), triangle);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(triangle), sizeof(color), color);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)sizeof(triangle));
	glEnableVertexAttribArray(1);


	// 顶点着色器和片段着色器源码
	const char *vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"           // 位置变量的属性位置值为0
		"layout (location = 1) in vec4 vColor;\n"        // 颜色变量的属性位置值为1
		"out vec4 color;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transform*vec4(aPos, 1.0);\n"
		"    color = vColor;\n"
		"}\n\0";
	const char *fragment_shader_source =
		"#version 330 core\n"
		"in vec4 color;\n"                           // 输入的颜色向量
		"out vec4 FragColor;\n"                // 输出的颜色向量
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
	default:
		break;
	}
}

void Draw(void)
{
	// 清空颜色缓冲
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//处理图形的旋转
	vmath::mat4 trans = vmath::rotate(xRot, vmath::vec3(1.0, 0.0, 0.0)) * 
						vmath::rotate(yRot, vmath::vec3(0.0, 1.0, 0.0));
	unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);

	// 绘制三角形
	glBindVertexArray(vertex_array_object);                                    // 绑定VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);                                          // 绘制三角形
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangles", NULL, NULL);

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

	//指定窗口大小改变时调用的函数reshaper
	glfwSetFramebufferSizeCallback(window, reshaper);

	//窗口键盘事件调用函数key_callback
	glfwSetKeyCallback(window, key_callback);

	//显示操作说明
	std::cout << "方向键可以控制图形的旋转。" << std::endl;
	std::cout << "数字键1，2，3设置多边形模式为点模式、线模式和填充模式。" << std::endl;
	std::cout << "数字键4关闭剔除模式。" << std::endl;
	std::cout << "数字键5打开剔除模式并且剔除多边形的背面。" << std::endl;
	std::cout << "数字键6，7设置多边形绕法为顺时针或者逆时针。" << std::endl;

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

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
