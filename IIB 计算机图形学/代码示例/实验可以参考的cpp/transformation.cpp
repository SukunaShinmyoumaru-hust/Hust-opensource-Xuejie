//////////////////////////////////////////////////////////////////////////////
//
//  Sphere.cpp
//  1. 球体的绘制（求出球面上所有的点）
//  2. 三角面片的构造
//  3. 利用统一变量进行数据传递
//////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vmath.h>
#include <vector>

//窗口大小参数
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float aspact = (float)4.0 / (float)3.0;

//旋转参数
static GLfloat xRot = 20.0f;
static GLfloat yRot = 20.0f;

//句柄参数
GLuint vertex_array_object; // == VAO句柄
GLuint vertex_buffer_object; // == VBO句柄
GLuint element_buffer_object;//==EBO句柄
int shader_program;//着色器程序句柄

//球的数据参数
std::vector<float> sphereVertices;
std::vector<int> sphereIndices;
const int Y_SEGMENTS = 20;
const int X_SEGMENTS = 20;
const float Radio = 3.0;
const GLfloat  PI = 3.14159265358979323846f;


void initial(void)
{
	//进行球体顶点和三角面片的计算
	// 生成球的顶点
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * Radio * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * Radio * PI) * std::sin(ySegment * PI);


			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
		}
	}

	// 生成球的顶点
	for (int i = 0; i < Y_SEGMENTS; i++)
	{
		for (int j = 0; j < X_SEGMENTS; j++)
		{

			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}

	// 球
	glGenVertexArrays(1, &vertex_array_object);
	glGenBuffers(1, &vertex_buffer_object);
	//生成并绑定球体的VAO和VBO
	glBindVertexArray(vertex_array_object);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 解绑VAO和VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 顶点着色器和片段着色器源码
	const char *vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 vPos;\n"           // 位置变量的属性位置值为0
		"out vec4 vColor;\n"           // 位置变量的属性位置值为0
		"uniform mat4 transform;\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transform*vec4(vPos, 1.0);\n"
		"    vColor = color;\n"
		"}\n\0";
	const char *fragment_shader_source =
		"#version 330 core\n"
		"in vec4 vColor;\n"                   // 输出的颜色向量
		"out vec4 FragColor;\n"                   // 输出的颜色向量
		"void main()\n"
		"{\n"
		"    FragColor = vColor;\n"
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
	glLineWidth(5);//设置线宽

	//启动剔除操作
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//开启深度测试
	glEnable(GL_DEPTH_TEST);
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case GLFW_KEY_2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case GLFW_KEY_3:
		glEnable(GL_CULL_FACE);    //打开背面剔除
		glCullFace(GL_BACK);          //剔除多边形的背面
		break;
	case GLFW_KEY_4:
		glDisable(GL_CULL_FACE);     //关闭背面剔除
		break;
	default:
		break;
	}
}

void Draw(void)
{
	// 清空颜色缓冲和深度缓冲区
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//处理图形的旋转
	vmath::mat4 trans = vmath::perspective(60, aspact, 1.0f, 500.0f) *vmath::translate(0.0f, 0.0f, -5.0f);
	unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans);

	//处理图形的颜色
	GLfloat vColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	unsigned int colorLoc = glGetUniformLocation(shader_program, "color");
	glUniform4fv(colorLoc, 1, vColor);

	// 绘制第一个红色的球
	glBindVertexArray(vertex_array_object);                                    // 绑定VAO
	glDrawElements(GL_TRIANGLES, X_SEGMENTS*Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);             
	
	//绘制第二个黑色的球
	xRot += (float)0.05f;
	vmath::mat4 trans2 = vmath::perspective(60, aspact, 1.0f, 500.0f) *vmath::translate(0.0f, 0.0f, -5.0f)* vmath::rotate(xRot, vmath::vec3(0.0, 1.0, 0.0))*vmath::translate(3.0f, 0.0f, 0.0f)*vmath::scale(0.3f);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans2);
	GLfloat vColor2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(colorLoc, 1, vColor2);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS*Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);                                                      // 解除绑定

}

void reshaper(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (height == 0)
	{
		aspact = (float)width;
	}
	else
	{
		aspact = (float)width / (float)height;
	}

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sphere", NULL, NULL);

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

	initial();//初始化

	//窗口大小改变时调用reshaper函数
	glfwSetFramebufferSizeCallback(window, reshaper);

	//窗口中有键盘操作时调用key_callback函数
	glfwSetKeyCallback(window, key_callback);

	
	while (!glfwWindowShouldClose(window))
	{
		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 解绑和删除VAO和VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
