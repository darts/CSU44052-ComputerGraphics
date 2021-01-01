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
#include <boids.h>
#include <spider_tpl.h>

#pragma warning (disable: 4244) //get the compiler to shut up about converting doubles to floats

#define NUM_SPIDERS 10

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
//Spider spider_arr[NUM_SPIDERS];
float spider_mov_delay_arr[NUM_SPIDERS];
//Boids spider_boid();

bool l_dir = true, l_spot = true, l_point = true;
bool boid_walk = true;

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

void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		return;
	if (key == GLFW_KEY_M)
		l_dir = !l_dir;
	if (key == GLFW_KEY_N)
		l_spot = !l_spot;
	if (key == GLFW_KEY_B)
		l_point = !l_point;
	if (key == GLFW_KEY_V)
		boid_walk = !boid_walk;
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

//void MovSpiderArr() {
//	for (int i = 0; i < NUM_SPIDERS; i++) {
//		spider_arr[i].HandleKeyPress(Spider::Spider_Directions::FORWARDS, time_diff / 2);
//	}
//}




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
	glfwSetKeyCallback(window, KeyPressCallback);

	//enable mouse movements
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader the_shader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/triShader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/triShader.fs");
	Shader ground_shader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightsource_shader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightsource_shader.fs");

	stbi_set_flip_vertically_on_load(true);
	Shader bag_shader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/bag_shader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/bag_shader.fs");
	Model ourModel("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/bag/backpack.obj");

	Ground the_ground = Ground(glm::mat4(1.0f));
	
	the_spider.Init();

	Boids spider_boid;

	//lighting
	Shader lightingShader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightsource_shader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightsource_shader.fs");
	Shader lightCubeShader("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightcube_shader.vs", "D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/shaders/lightcube_shader.fs");

	Model dir_light = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/light_cube/light_not_cube.obj");

	Model lights[4] = {
		Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/light_cube/light_cube.obj"),
		Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/light_cube/light_cube.obj"),
		Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/light_cube/light_cube.obj"),
		Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/light_cube/light_cube.obj")
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, 3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	lightingShader.use();
	lightingShader.SetInt("material.diffuse", 0);
	lightingShader.SetInt("material.specular", 1);


	// directional light
	lightingShader.SetVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
	lightingShader.SetVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	lightingShader.SetVec3("dirLight.diffuse", glm::vec3(1.4f, 1.4f, 1.4f));
	lightingShader.SetVec3("dirLight.specular", glm::vec3(1.5f, 1.5f, 1.5f));
	// point light 1
	lightingShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
	lightingShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	lightingShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	lightingShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetFloat("pointLights[0].constant", 1.0f);
	lightingShader.SetFloat("pointLights[0].linear", 0.09);
	lightingShader.SetFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	lightingShader.SetVec3("pointLights[1].position", pointLightPositions[1]);
	lightingShader.SetVec3("pointLights[1].ambient", glm::vec3(-0.05f, -0.05f, -0.05f));
	lightingShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	lightingShader.SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetFloat("pointLights[1].constant", 1.0f);
	lightingShader.SetFloat("pointLights[1].linear", 0.09);
	lightingShader.SetFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	lightingShader.SetVec3("pointLights[2].position", pointLightPositions[2]);
	lightingShader.SetVec3("pointLights[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	lightingShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	lightingShader.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetFloat("pointLights[2].constant", 1.0f);
	lightingShader.SetFloat("pointLights[2].linear", 0.09);
	lightingShader.SetFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	lightingShader.SetVec3("pointLights[3].position", pointLightPositions[3]);
	lightingShader.SetVec3("pointLights[3].ambient", glm::vec3(0.01f, 0.01f, 0.01f));
	lightingShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	lightingShader.SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetFloat("pointLights[3].constant", 1.0f);
	lightingShader.SetFloat("pointLights[3].linear", 0.09);
	lightingShader.SetFloat("pointLights[3].quadratic", 0.032);
	// spotLight
	glm::vec3 spotlight_pos = glm::vec3(0.0f, 6.0f, 1.0f);
	glm::vec3 spotlight_dir = glm::vec3((float)atan(1) * -1.0f, (float)atan(1) * -0.8f, (float)atan(1) * 0.0f);


	//lightingShader.SetVec3("spotLight.position", camera.camera_position);
	//lightingShader.SetVec3("spotLight.direction", camera.z_axis_vector);
	lightingShader.SetVec3("spotLight.position", spotlight_pos);
	lightingShader.SetVec3("spotLight.direction", spotlight_dir);
	lightingShader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	lightingShader.SetVec3("spotLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	lightingShader.SetVec3("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	lightingShader.SetFloat("spotLight.constant", 1.0f);
	lightingShader.SetFloat("spotLight.linear", 0.09);
	lightingShader.SetFloat("spotLight.quadratic", 0.032);
	lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));




	stbi_set_flip_vertically_on_load(false);
	Model spoider("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider.obj");



	//consider depth when rendering
	//Prevents triangles being drawn over the things they are spacially behind
	glEnable(GL_DEPTH_TEST); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Spider_tpl lug = Spider_tpl();

	while (!glfwWindowShouldClose(window))
	{
		//manage frame time variance
		float current_frame = (float)glfwGetTime();
		time_diff = current_frame - previous_frame;
		previous_frame = current_frame;

		KeyPressHandler(window); //check for user input

		//MovSpiderArr();

  
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.SetVec3("viewPos", camera.camera_position);
		lightingShader.SetFloat("material.shininess", 32.0f);

		

		//generate perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.camera_zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		//generate view matrix
		glm::mat4 view = camera.GenerateCameraViewMatrix();
		//lightingShader.SetFloat("scale_factor", 1.0f); //adjust object scaling
		//lightingShader.SetMat4("projection", projection);
		//lightingShader.SetMat4("view", view);

		//lightingShader.SetFloat("scale_factor", 10.0f); //adjust object scaling


		lightingShader.use();
		//lightingShader.SetVec3("spotLight.position", camera.camera_position);
		//lightingShader.SetVec3("spotLight.direction", camera.z_axis_vector);
		lightingShader.SetMat4("projection", projection);
		lightingShader.SetMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		
		//ourModel.Draw(lightingShader);


		
		
		//lightingShader.SetMat4("projection", projection);
		//lightingShader.SetMat4("view", view);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		lightingShader.SetFloat("scale_factor", 1.0f);
		
		//the_spider.Draw(lightingShader);
		ourModel.Draw(lightingShader); //---------------------useful

		//the_ground.Draw(lightingShader); //---------------------useful

		//the_spider.Draw(lightingShader);
		

		// also draw the lamp object(s)
		lightCubeShader.use();
		lightCubeShader.SetMat4("projection", projection);
		lightCubeShader.SetMat4("view", view);
		//glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);

		// we now draw as many light bulbs as we have point lights.
		
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShader.SetMat4("model", model);
			lights[i].Draw(lightCubeShader);
		}
		model = glm::mat4(1.0f);
		model = glm::translate(model, spotlight_pos);
		model = glm::rotate(model, (float)atan(1), glm::vec3(0.0f, 0.0f, spotlight_dir.y));
		model = glm::scale(model, glm::vec3(0.2f)); // Make it smaller
		lightCubeShader.SetMat4("model", model);
		dir_light.Draw(lightCubeShader);

		the_shader.use();

		//generate perspective projection matrix
		//glm::mat4 projection = glm::perspective(glm::radians(camera.camera_zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		//generate view matrix
		//glm::mat4 view = camera.GenerateCameraViewMatrix();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		the_shader.SetMat4("model", model);
		the_shader.SetFloat("scale_factor", 10.0f);
		the_shader.SetMat4("projection", projection);
		the_shader.SetMat4("view", view);

		//the_spider.Draw(the_shader); //---------------------useful

		/*for (int i = 0; i < NUM_SPIDERS; i++) {
			spider_arr[i].Draw(the_shader);
		}*/

		if(boid_walk)
			spider_boid.UpdateBoidsPos(time_diff);
		//spider_boid.DrawBoids(the_shader); //---------------------useful





		//old, working shader code------------------
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.05f));
		the_shader.SetMat4("model", model);
		//lug.Draw(the_shader);
		///------------------------

		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.01f));
		//the_shader.SetMat4("model", model);
		//lightingShader.SetFloat("scale_factor", 10.0f);
		//lightingShader.SetMat4("projection", projection);
		//lightingShader.SetMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.05f));
		//lightingShader.SetMat4("model", model);

		lightingShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.05f));
		lightingShader.SetMat4("model", model);
		lightingShader.SetMat4("projection", projection);
		lightingShader.SetMat4("view", view);
		lightingShader.SetFloat("scale_factor", 10.0f);
		lug.Draw(lightingShader); //---------------------useful

		/*bag_shader.use();

		model = glm::mat4(1.0f);

		bag_shader.setmat4("projection", projection);
		bag_shader.setmat4("view", view);
		bag_shader.setmat4("model", model);*/
		//bag_shader.SetFloat("scale_factor", 1.0f);

		//the_ground.Draw(bag_shader);
		

		lightingShader.use();
		lightingShader.SetMat4("projection", projection);
		lightingShader.SetMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		//model = glm::rotate(model, (float)atan(1) * 4, glm::vec3(1.0f, 0.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		lightingShader.SetFloat("scale_factor", 1.0f);
		spoider.Draw(lightingShader); //---------------------useful


		//update lighting
		if (l_dir) {
			lightingShader.SetVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			lightingShader.SetVec3("dirLight.diffuse", glm::vec3(1.4f, 1.4f, 1.4f));
			lightingShader.SetVec3("dirLight.specular", glm::vec3(1.5f, 1.5f, 1.5f));
		}
		else {
			lightingShader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		}

		if (l_point) {
			// point light 1
			lightingShader.SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			lightingShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			// point light 2
			lightingShader.SetVec3("pointLights[1].ambient", glm::vec3(-0.05f, -0.05f, -0.05f));
			lightingShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			// point light 3
			lightingShader.SetVec3("pointLights[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			// point light 4
			lightingShader.SetVec3("pointLights[3].ambient", glm::vec3(0.01f, 0.01f, 0.01f));
			lightingShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			lightingShader.SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else {
			// point light 1
			lightingShader.SetVec3("pointLights[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			// point light 2
			lightingShader.SetVec3("pointLights[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			// point light 3
			lightingShader.SetVec3("pointLights[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			// point light 4
			lightingShader.SetVec3("pointLights[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[3].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("pointLights[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		}

		if (l_spot) {
			lightingShader.SetVec3("spotLight.diffuse", glm::vec3(10.0f, 10.0f, 10.0f));
			lightingShader.SetVec3("spotLight.specular", glm::vec3(25.0f, 25.0f, 25.0f));
		}
		else {
			lightingShader.SetVec3("spotLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			lightingShader.SetVec3("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); //clean up
	return 0;
}