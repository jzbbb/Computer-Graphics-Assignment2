/*
Student Information
Student ID:
Student Name:
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/stb_image.h"


#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include "Dependencies/glm/gtc/type_ptr.hpp"

using namespace std;

// screen setting
const float SCR_WIDTH = 2400;
const float SCR_HEIGHT = 1800;

unsigned int VBO[4], VAO[4], EBO[3];

unsigned int cubemap_texture;
unsigned int sky_vao, sky_vbo;

Textures texture[4],sky_texture;
Textures texture_snow[2];

std::vector<std::string> snow{
	("resources/model/snow/snow_01.jpg"),
	("resources/model/snow/snow_02.jpg")
};
unsigned int snow_index = 0;

unsigned int penguin_index = 0;

float delta_time = 0.0f;  // 当前帧与上一帧的时间差
float last_frame = 0.0f;  // 上一帧的时间

const GLfloat  PI = 3.14159265358979323846f;

//将球横纵划分成50X50的网格
const int Y_SEGMENTS = 100;
const int X_SEGMENTS = 100;

float last_x = SCR_WIDTH / 2.0f, last_y = SCR_HEIGHT / 2.0f;   // 设置鼠标初始位置为屏幕中心

Camera camera = Camera(glm::vec3(0.0f, 2.5f, 6.0f));                 // 摄像机位置
bool camera_move = false;

float penguin_x = 0.0f;
float penguin_y = 0.0f;
float penguin_z = 0.0f;
float delta = 0.0f;
float angle = 0.0f;

bool day = true;//true为白天，false为黑天

bool sp = true;//是否是第三人称跟随，点击左键取消第三人称并设为鼠标控制视角移动

// lighting
glm::vec3 lightPos(-0.0f, 0.0f, 0.0f);//点光源位置
glm::vec3 DirLight_shininess(2.0);//定向光亮度
float DirLight_shininess2=1.0;//定向光亮度
int spotlight = -1;//控制聚光的开关
bool light_pole_switch = false;

//light pole
glm::vec3 light_pole_pos[] = {
	glm::vec3(10.0f,-0.5f,8.0f),
	glm::vec3(10.0f,-0.5f,-8.0f),
	glm::vec3(-10.0f,-0.5f,8.0f),
	glm::vec3(-10.0f,-0.5f,-8.0f),
};
float light_pole_angle[] = {
	glm::radians(180.0f),
	glm::radians(180.0f),
	glm::radians(0.0f),
	glm::radians(0.0f),	
};
glm::vec3 lighting_pole_pos[] = {
	glm::vec3(7.0f,4.8f,8.0f),
	glm::vec3(7.0f,4.8f,-8.0f),
	glm::vec3(-7.0f,4.8f,8.0f),
	glm::vec3(-7.0f,4.8f,-8.0f),
};
glm::vec3 grandstand_pos[] = {
	glm::vec3(15.0f,0.0f,8.0f),
	glm::vec3(15.0f,0.0f,-8.0f),
	glm::vec3(-15.0f,0.0f,8.0f),
	glm::vec3(-15.0f,0.0f,-8.0f),
};
float grandstand_angle[] = {
	glm::radians(-90.0f),
	glm::radians(-90.0f),
	glm::radians(90.0f),
	glm::radians(90.0f),
};


int NBA_choice = 0;

Textures NBA[28];

std::vector<std::string> NBA_court{
	("resources/texture/NBA_court/laker.png"),
	("resources/texture/NBA_court/76.png"),
	("resources/texture/NBA_court/buxingzhe.png"),
	("resources/texture/NBA_court/duxingxia.png"),
	("resources/texture/NBA_court/huangfeng.png"),
	("resources/texture/NBA_court/huixiong.png"),
	("resources/texture/NBA_court/huosai.png"),
	("resources/texture/NBA_court/juejin.png"),
	("resources/texture/NBA_court/jueshi.png"),
	("resources/texture/NBA_court/kaierteren.png"),
	("resources/texture/NBA_court/kaituozhe.png"),
	("resources/texture/NBA_court/king.png"),
	("resources/texture/NBA_court/kuaichuan.png"),
	("resources/texture/NBA_court/lanwang.png"),
	("resources/texture/NBA_court/leitin.png"),
	("resources/texture/NBA_court/menglong.png"),
	("resources/texture/NBA_court/moshu.png"),
	("resources/texture/NBA_court/nikesi.png"),
	("resources/texture/NBA_court/qishi.png"),
	("resources/texture/NBA_court/rehuo.png"),
	("resources/texture/NBA_court/ssl.png"),
	("resources/texture/NBA_court/taiyang.png"),
	("resources/texture/NBA_court/tihu.png"),
	("resources/texture/NBA_court/xionglu.png"),
	("resources/texture/NBA_court/yongshi.png"),
	("resources/texture/NBA_court/公牛.png"),
	("resources/texture/NBA_court/火箭.png"),
	("resources/texture/NBA_court/老鹰.png"),
};

float skybox_vertices[] = {   //天空盒顶点数组
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces//加载天空盒纹理
{
  ("resources/texture/jaj_snow/jajsnow1_rt.tga"),
  ("resources/texture/jaj_snow/jajsnow1_lf.tga"),
  ("resources/texture/jaj_snow/jajsnow1_up.tga"),
  ("resources/texture/jaj_snow/jajsnow1_dn.tga"),
  ("resources/texture/jaj_snow/jajsnow1_bk.tga"),
  ("resources/texture/jaj_snow/jajsnow1_ft.tga")
};

Shader shader;
Shader sky_shader;
Shader light_shader;



unsigned int load_cubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrchannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrchannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
void get_OpenGL_info()
{
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

void sendDataToOpenGL()
{
	//TODO
	//Load objects and bind to VAO and VBO
	//Load textures
	//Section1 加载模型数据
	std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;
	// 生成球的顶点
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);


			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
		}
	}
	// 生成球的Indices
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

	float Quad[] = {
		0.54f,  0.29f, 0.0f,   0.0f,0.0f,1.0f, 1.0f,1.0f,
		0.54f, -0.29f, 0.0f,   0.0f,0.0f,1.0f, 1.0f,0.0f,
	   -0.54f, -0.29f, 0.0f,   0.0f,0.0f,1.0f, 0.0f,0.0f,

	    0.54f,  0.29f, 0.0f,   0.0f,0.0f,1.0f, 1.0f,1.0f,
	   -0.54f, -0.29f, 0.0f,   0.0f,0.0f,1.0f, 0.0f,0.0f,
	   -0.54f,  0.29f, 0.0f,   0.0f,0.0f,1.0f, 0.0f,1.0f
	};

	glGenVertexArrays(4, VAO);
	glGenBuffers(4, VBO);
	glGenBuffers(3, EBO);


	for (int i = 0; i < snow.size(); i++) {
		texture_snow[i].setupTexture(snow[i].c_str());
	}
	for (int i = 0; i < NBA_court.size(); i++) {
		NBA[i].setupTexture(NBA_court[i].c_str());
	}

	//quad
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	//sphere---light
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	//---light pole
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	
	//shader.setInt("material.diffuse", 0);
	//shader.setInt("material.specular", 1);

	//sky_box
	{
	glGenVertexArrays(1, &sky_vao);
	glGenBuffers(1, &sky_vbo);
	glBindVertexArray(sky_vao);
	glBindBuffer(GL_ARRAY_BUFFER, sky_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	cubemap_texture = load_cubemap(faces);
	}

}

void initializedGL(void) //run only once
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	//TODO: set up the camera parameters	
	//TODO: set up the vertex shader and fragment shader
	shader.setupShader("./shader/VertexShaderCode.glsl", "./shader/FragmentShaderCode.glsl");
	sky_shader.setupShader("./shader/skybox.vs", "./shader/skybox.fs");
	light_shader.setupShader("./shader/lamp.vertex", "./shader/lamp.frag");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}

void paintGL(void)  //always run
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); //specify the background color, this is just an example
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	


	shader.use();
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	light_shader.use();
	light_shader.setMat4("projection", projection);
	light_shader.setMat4("view", view);

	{//light
		shader.use();
		shader.setVec3("viewPos", camera.position);
		shader.setFloat("material.shininess", 128.0f);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// directional light
		shader.setVec3("dirLight.direction", -0.0f, -1.0f, -0.0f);
		shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		shader.setVec3("DirLight_shininess", DirLight_shininess);
		shader.setFloat("DirLight_shininess2", DirLight_shininess2);
		// point light 
		shader.setVec3("pointLights[0].position", lightPos);
		shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.05f);
		shader.setFloat("pointLights[0].quadratic", 0.001f);
		for (int j = 0; j < 4; j++) {
			char uniformName[128];
			std::snprintf(uniformName, 128, "pointLights[%d].position", j + 1);
			shader.setVec3(uniformName, lighting_pole_pos[j]);
			if (light_pole_switch) {
				std::snprintf(uniformName, 128, "pointLights[%d].ambient", j + 1);
				shader.setVec3(uniformName, 0.05f, 0.05f, 0.05f);
				std::snprintf(uniformName, 128, "pointLights[%d].diffuse", j + 1);
				shader.setVec3(uniformName, 0.8f, 0.8f, 0.8f);
			}
			else {
				std::snprintf(uniformName, 128, "pointLights[%d].ambient", j + 1);
				shader.setVec3(uniformName, 0.0f, 0.0f, 0.0f);
				std::snprintf(uniformName, 128, "pointLights[%d].diffuse", j + 1);
				shader.setVec3(uniformName, 0.0f, 0.0f, 0.0f);
			}
			std::snprintf(uniformName, 128, "pointLights[%d].specular", j + 1);
			shader.setVec3(uniformName, 1.0f, 1.0f, 1.0f);
			std::snprintf(uniformName, 128, "pointLights[%d].constant", j + 1);
			shader.setFloat(uniformName, 1.0f);
			std::snprintf(uniformName, 128, "pointLights[%d].linear", j + 1);
			shader.setFloat(uniformName, 0.09f);
			std::snprintf(uniformName, 128, "pointLights[%d].quadratic", j + 1);
			shader.setFloat(uniformName, 0.032f);
		}
		// spotLight
		shader.setVec3("spotLight.position", camera.position);
		shader.setVec3("spotLight.direction", camera.forward);
		shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.005f);
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.5f)));
		shader.setInt("spotlight", spotlight);
	}




	for (int i = 0; i < 5; i++) {
		if (i == 0) {//sky_box
			glDepthFunc(GL_LEQUAL); // 深度测试条件 小于等于
			sky_shader.use();
			sky_shader.setMat4("projection", projection);
			glm::mat4 sky_view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			sky_shader.setMat4("view", sky_view);
			sky_texture.bind(0);
			sky_shader.setInt("sky", 0);
			glBindVertexArray(sky_vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);//绘制天空盒
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);//深度测试  输入的深度值小于参考值，则通过
			sky_texture.unbind();
		}
		else if(i==1){//light
			//让光源绕原点旋转
			float a = 50.0f; // x轴半径
			float b = 20.0f; // y轴半径
			float speed = 0.2f; // 运动速度
			float t = glfwGetTime() * speed; // 参数t
			float lightY;
			
			if (day) {
				DirLight_shininess = glm::vec3(2.0f);
			}
			else {
				DirLight_shininess = glm::vec3(0.5f);
			}
			if (sin(t) < 0) {
				lightY = -b * sin(t);
			}
			else {
				lightY = b * sin(t);		
			}
			float lightX = a * cos(t);
			float lightZ = 0.0f;
			//lightPos.x = lightX; lightPos.y = 1; lightPos.z = 1.2;						//直线移动
			lightPos.x = lightX;
			lightPos.y = lightY;
			lightPos.z = lightZ;			//环绕移动

			light_shader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(1.0f));
			light_shader.setMat4("model", model);
		}
		else if(i==2){
			light_shader.use();
			for (int j = 0; j < 4; j++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, lighting_pole_pos[j]);
				model = glm::scale(model, glm::vec3(0.2f));
				light_shader.setMat4("model", model);
				glBindVertexArray(VAO[0]);
				if (light_pole_switch) {
					glDrawElements(GL_TRIANGLES, X_SEGMENTS* Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
				}			
				glBindVertexArray(0);
			}
		}
		else if(i==3){
			shader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f,0.5f,0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(33.0f));
			shader.setMat4("model", model);
			NBA[NBA_choice].bind(0);
			shader.setInt("texture_diffuse0", 0);
			shader.setInt("texture_specular0", 0);
			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			NBA[NBA_choice].unbind();
		}
		else {
			

		}
	}

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Sets the mouse-button callback for the current window.
	if (!sp) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			spotlight = -spotlight;
	}
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
		//camera.UpdateCameraVectors(glm::vec3(0.0f, angle, 0.0f));

		camera_move = !camera_move;
		sp = !sp;
	}
		
}

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if(camera_move){
		float x_offset = x - last_x;       // 计算当前帧与上一帧的鼠标位置偏移量
		float y_offset = y - last_y;
		last_x = x;
		last_y = y;

		float sensitivity = 0.05;               // 灵敏度*偏移量，避免鼠标移动太大
		x_offset *= sensitivity;
		y_offset *= sensitivity;

		camera.Rotate(glm::vec3(x_offset, y_offset, 0));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.Zoom(yoffset);
}

// 键盘控制(摄像机位置TGFH -> 上下左右)
void ProcessInput(GLFWwindow* window)
{
	float speed = camera.mouse_speed * delta_time;
	// 移动
	if (!sp) {
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			camera.Move(camera.forward * speed);
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			camera.Move(-camera.forward * speed);
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			camera.Move(-camera.right * speed);
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			camera.Move(camera.right * speed);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Sets the Keyboard callback for the current window.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		DirLight_shininess2 += 0.2;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		DirLight_shininess2 -= 0.2;
		if (DirLight_shininess2 <= 0) {
			DirLight_shininess2 = 0.1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		snow_index = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		snow_index = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		penguin_index = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		penguin_index = 1;
	}	
	// 更新模型位置	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		delta = -0.5f;
		penguin_x += delta * sin(glm::radians(angle));
		penguin_y += delta * cos(glm::radians(angle));
		if (sp) {
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
		}
		
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		delta = +0.5f;
		penguin_x += delta * sin(glm::radians(angle));
		penguin_y += delta * cos(glm::radians(angle));
		if (sp) {
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
		}
		
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		angle += 90.0f;
		if (sp) {
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
			camera.UpdateCameraVectors(glm::vec3(0.0f, angle, 0.0f));
		}
		
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		angle -= 90.0f;
		if (sp) {
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
			camera.UpdateCameraVectors(glm::vec3(0.0f, angle, 0.0f));
		}	
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		light_pole_switch = !light_pole_switch;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		NBA_choice++;
		NBA_choice = (NBA_choice) % 28;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		day = !day;
	}
}


int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);                                                                  //    
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);            // 隐藏光标，鼠标停留在窗口内

	initializedGL();

	double jump_start_time = -1.0;
	double jump_end_time = -1.0;
	double jump_duration = 1.0; // 跳跃动作持续时间
	
	Model model1("./resources/model/snow/snow.obj");
	Model model2("./resources/model/penguin1/penguin.obj");
	Model model3("./resources/model/penguin2/penguin.obj");
	Model model4("./resources/model/Court with hoops/Court with hoops.obj");
	Model model5("./resources/model/Street Lamp/StreetLamp.obj");
	Model model6("./resources/model/grandstand/generic medium.obj");
	Model model7("./resources/model/moon/Moon.obj");
	Model model8("./resources/model/moon2/Moon.obj");
	Model model9("./resources/model/Mario/mario-sculpture.obj");

	while (!glfwWindowShouldClose(window)) {
		// 在每一帧中计算出新的deltaTime
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		// 输入控制
		ProcessInput(window);


		/* Render here */
		paintGL();
		if (sp) {
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jump_start_time < 0.0) {
			// 如果还没有跳跃，则记录跳跃开始时间
			jump_start_time = glfwGetTime();
			jump_end_time = jump_start_time + jump_duration;
		}

		// 计算模型位置
		double current_time = glfwGetTime();
		if (current_time >= jump_start_time && current_time <= jump_end_time) {
			// 如果在跳跃时间范围内，则计算跳跃位置
			float jump_progress = (current_time - jump_start_time) / jump_duration;
			float jump_height = 1.0f; // 跳跃高度
			penguin_z = jump_height * sin(jump_progress * PI);
			camera.position = glm::vec3(6 * sin(glm::radians(angle)), 2.5f, 6 * cos(glm::radians(angle))) + glm::vec3(penguin_x, penguin_z, penguin_y);
		}
		else {
			// 如果不在最后一次跳跃的时间范围内，则重置跳跃时间记录
			jump_start_time = -1.0;
			jump_end_time = -1.0;
		}

		glm::mat4 model = glm::mat4(1.0f);

		for (int i = 0; i < 10; i++) {
			if (i == 0) {//penguin
				shader.use();
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
				model = glm::translate(model, glm::vec3(penguin_x, penguin_z, penguin_y));
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.8f));
				shader.setMat4("model", model);
				if (penguin_index == 0) {
					model2.Draw(shader);
				}
				else {
					model3.Draw(shader);
				}
			}
			else if(i==1){//floor
				shader.use();
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
				model = glm::scale(model, glm::vec3(5.0));
				shader.setMat4("model", model);
				texture_snow[snow_index].bind(0);
				shader.setInt("texture_diffuse0", 0);
				shader.setInt("texture_specular0", 0);	
				model1.Draw(shader);
				texture_snow[snow_index].unbind();
			}
			else if(i==2){//court
				shader.use();
				model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.001f));
				shader.setMat4("model", model);
				texture[1].bind(0);
				shader.setInt("texture_diffuse0", 0);
				shader.setInt("texture_specular0", 0);
				model4.Draw(shader);
				texture[1].unbind();
			}
			else if (i == 3) {
				shader.use();
				for (int j = 0; j < 4; j++) {
					model = glm::mat4(1.0f);
					model = glm::translate(model, light_pole_pos[j]);
					model = glm::rotate(model, light_pole_angle[j], glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::scale(model, glm::vec3(0.4f));
					shader.setMat4("model", model);
					model5.Draw(shader);
				}
			}
			else if (i == 4) {
				shader.use();
				for (int j = 0; j < 4; j++) {
					model = glm::mat4(1.0f);
					model = glm::translate(model, grandstand_pos[j]);
					model = glm::rotate(model, grandstand_angle[j], glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::scale(model, glm::vec3(0.5f));
					shader.setMat4("model", model);
					model6.Draw(shader);
				}	
			}
			else if (i == 5) {
				shader.use();
				if (day) {
					model = glm::mat4(1.0f);
					model = glm::translate(model, lightPos);
					model = glm::scale(model, glm::vec3(0.005f));
					shader.setMat4("model", model);
					model8.Draw(shader);
				}
				else {
					model = glm::mat4(1.0f);
					model = glm::translate(model, lightPos);
					model = glm::scale(model, glm::vec3(0.005f));
					shader.setMat4("model", model);
					model7.Draw(shader);
				}
			}
			else if (i == 6) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(0.0f,6.0f,-30.0f));
				model = glm::scale(model, glm::vec3(0.1f));
				shader.setMat4("model", model);
				model9.Draw(shader);
			}
		}
		
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}






