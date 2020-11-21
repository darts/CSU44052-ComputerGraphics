#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>
#include <Spider.h>
#include <ground.h>

#pragma warning (disable: 4244) //get the compiler to shut up about converting doubles to floats

//default window dimensions
const unsigned int WIN_WIDTH = 1600 / 1.2;
const unsigned int WIN_HEIGHT = 1200 / 1.2;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); //start the camera back a bit so it doesn't end up inside the model
float previous_mouse_x_pos = WIN_WIDTH / 2.0f; 
float previous_mouse_y_pos = WIN_HEIGHT / 2.0f;
bool first_mouse_callback = true;

// timing
float time_diff = 0.0f;	// time between current frame and last frame
float previous_frame = 0.0f;

Spider the_spider;

//callback for resizing window
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void KeyPressHandler(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.HandleKeyPress(Camera::Camera_Directions::FORWARDS, time_diff);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.HandleKeyPress(Camera::Camera_Directions::BACKWARDS, time_diff);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.HandleKeyPress(Camera::Camera_Directions::LEFT, time_diff);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.HandleKeyPress(Camera::Camera_Directions::RIGHT, time_diff);

	//spider
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		the_spider.HandleKeyPress(Spider::Spider_Directions::FORWARDS, time_diff);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		the_spider.HandleKeyPress(Spider::Spider_Directions::BACKWARDS, time_diff);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		the_spider.HandleKeyPress(Spider::Spider_Directions::LEFT, time_diff);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		the_spider.HandleKeyPress(Spider::Spider_Directions::RIGHT, time_diff);

}

void MouseMoveCallback(GLFWwindow* window, double mouse_x_pos, double mouse_y_pos)
{
	if (first_mouse_callback)
	{
		previous_mouse_x_pos = (float)mouse_x_pos;
		previous_mouse_y_pos = (float)mouse_y_pos;
		first_mouse_callback = false;
	}

	float mouse_move_x = (float)mouse_x_pos - previous_mouse_x_pos;
	float mouse_move_y = previous_mouse_y_pos - (float)mouse_y_pos;

	camera.HandleMouseMove(mouse_move_x, mouse_move_y);

	previous_mouse_x_pos = (float)mouse_x_pos;
	previous_mouse_y_pos = (float)mouse_y_pos;
}

//we can ignore scrolling in x direction bc a normal scrollwheel, being vertical, only moves in the y direction
void MouseScrollCallback(GLFWwindow* window, double scroll_x, double scroll_y)
{
	camera.HandleMouseScroll((float)scroll_y);
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Mid Term Assignment", NULL, NULL);
	if (window == NULL) //make sure window was created
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //make sure glad was loaded
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); //create viewport

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); //window resizing
	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);

	//enable mouse movements
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader the_shader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/triShader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/triShader.fs");
	Shader ground_shader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/ground_shader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/ground_shader.fs");

	Ground the_ground = Ground(glm::mat4(1.0f));
	
	the_spider.Init();

	//consider depth when rendering
	//Prevents triangles being drawn over the things they are spacially behind
	glEnable(GL_DEPTH_TEST); 

	while (!glfwWindowShouldClose(window))
	{
		//manage frame time variance
		float current_frame = (float)glfwGetTime();
		time_diff = current_frame - previous_frame;
		previous_frame = current_frame;

		KeyPressHandler(window); //check for user input
  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		the_shader.use();

		//generate perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.camera_zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		//generate view matrix
		glm::mat4 view = camera.GenerateCameraViewMatrix();
		the_shader.SetFloat("scale_factor", 10.0f); //adjust object scaling
		the_shader.SetMat4("projection", projection);
		the_shader.SetMat4("view", view);

		the_spider.Draw(the_shader);

		ground_shader.use();

		ground_shader.SetFloat("scale_factor", 5.0f);
		ground_shader.SetMat4("projection", projection);
		ground_shader.SetMat4("view", view);

		the_ground.Draw(ground_shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //clean up
	return 0;
}