#include "Dependencies\glew\glew.h"
//#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include <iostream>
#include <GLFW\glfw3.h>
#define GLEW_STATIC
#include <cstdlib>
#include <ctime>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

int scene=1, switcher=0;
const int maxscenes = 3;
bool plywaj = false;

GLuint screenWidth = 800, screenHeight = 600;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat movementx = 0.0f;
GLfloat movementy = 0.0f;
GLfloat movementz = 0.0f;
int dir=1;
GLint ile = 1.0f;

int main()
{
	srand(time(NULL));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "PROJEKT", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;
	glewInit();


	glViewport(0, 0, screenWidth, screenHeight);


	glEnable(GL_DEPTH_TEST);

	Shader shader("C:/Users/Ser-LT/Desktop/PROJEKT/Project1/VertexShader.vs", "C:/Users/Ser-LT/Desktop/PROJEKT/Project1/FragmentShader.frag");

	Model zlotaRybka("C:/Users/Ser-LT/Desktop/PROJEKT/Project1/Models/GoldenFish_OBJ/Golden_Fish_OBJ.obj");
	Model city("C:/Users/Ser-LT/Desktop/PROJEKT/Project1/Models/Paris/Paris2010_0.obj");
	//GLint x, y, z;
	GLfloat rotater = 0.15f;
	GLfloat rybx = 1.0f, ryby = 1.0f, rybz = -5.0f;
	//bool losuj = false;

	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(4.3f, 1.6f, -2.0f),
		glm::vec3(4.3f, -1.6f, -4.0f)
	};
	while (!glfwWindowShouldClose(window))
	{
		if (scene == 1 && plywaj == true)
		{
			GLfloat lastx, lastz;
			int switcherr = 0;
			switch (dir)
			{
			case 1:
				if (ile<3000)
				{
					
					rotater = 0.15f;
					rybx = 1.0f + 0.01f*ile;
					if (ile > 1000 && ile < 2000)
						rotater = 0.05f;
					if (ile > 2000 && ile < 3000)
						rotater = 0.25f;
					
				}
				else
				{
					dir ++;
					//rybx = 1.0f + 0.01f*ile;
					//rybz = -5.0f;
					lastx = rybx;
					ile = 1;
				}
				break;
			case 2:
				if (ile<3000)
				{
					rybz = -5.0f + 0.01f*ile;
					rotater = 4.45f;
					if (ile > 1000 && ile < 2000)
						rotater = 4.35f;
					if (ile > 2000 && ile < 3000)
						rotater = 4.55f;
				}
				else
				{
					dir ++;
					//rybz = -5.0f + 0.01f*ile;
					//rybx = 6.0f;
					lastz = rybz;
					ile = 1;
				}
				break;
			case 3:
				if (ile<3000)
				{
					rybx = lastx - 0.01f*ile;
					rotater = 3.15f;
					if (ile > 1000 && ile < 2000)
						rotater = 3.05f;
					if (ile > 2000 && ile < 3000)
						rotater = 3.25f;
				}
				else
				{
					dir ++;
					//rybx = 1.0f*skala;
					//rybz = 0.0f;
					lastx = rybx;
					ile = 1;
				}
				break;
			case 4:
				if (ile<3000)
				{
					rybz = lastz - 0.01f*ile;
					rotater = 1.50f;
					if (ile > 1000 && ile < 2000)
						rotater = 1.40f;
					if (ile > 2000 && ile < 3000)
						rotater = 1.60f;
				}
				else {
					dir = 1;
					//rybz = -5.0f*skala;
					//rybx = 1.0f;
					lastz = rybz;
					ile = 1;
				}
				break;
			}
		}


		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].linear"), 0.009);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].quadratic"), 0.0032);

		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].linear"), 0.009);
		glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].quadratic"), 0.0032);

		glm::mat4 model;
		
		switch (scene)
		{
		case 0:
			
		case 1:
			if(plywaj==true)
			ile++;
			
		//	cout << "rybx " << rybx << endl;
		//	cout << "ryby " << ryby << endl;
		//	cout << "rybz " << rybz << endl;
			if(plywaj==true)
			cout << dir << endl;
			model = glm::translate(model, glm::vec3(rybx, ryby, rybz));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			model = glm::rotate(model, rotater, glm::vec3(0.0f, 0.1f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			zlotaRybka.Draw(shader);
			break;
		case 2:
			model = glm::translate(model, glm::vec3(0.0f, -80.0f, -5.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			city.Draw(shader);
			break;
		case 3:
			model = glm::translate(model, glm::vec3(0.0f, -9.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			city.Draw(shader);
			break;
		}
		glBindVertexArray(0);
		

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}


#pragma region "User input"


void Do_Movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		camera.Fly(0, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.Fly(1, deltaTime);
	if (keys[GLFW_KEY_F1] && switcher==0)
	{
		ile = 0;
		if (switcher == 0)
		{
			switcher = 1;
		}
		else switcher = 0;
		scene++;
		if (scene > maxscenes)
			scene = 1;
	}
	if (keys[GLFW_KEY_F2] && switcher == 1)
	{
		ile = 0;
		if (switcher == 0)
		{
			switcher = 1;
		}
		else switcher = 0;
		scene++;
		if (scene > maxscenes)
			scene = 0;
	}
	if (keys[GLFW_KEY_0])
	{
		if (plywaj == true)
			plywaj = false;
	}
	if (keys[GLFW_KEY_9])
	{
		if (plywaj == false)
			plywaj = true;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion