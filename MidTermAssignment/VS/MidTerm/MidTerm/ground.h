#pragma once

#include <model.h>
#include <shader.h>

class Ground {
public:
	Model plane_model;
	glm::mat4 starting_position;

	Ground(glm::mat4 position) {
		this->starting_position = position;
		this->starting_position = glm::translate(starting_position, glm::vec3(0.0f, -1.0f, 0.0f));
		plane_model = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/ground/ground_2.obj");
	}

	void Draw(Shader& shader) {
		//shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		shader.SetMat4("model", model);
		plane_model.Draw(shader, false, glm::vec3(1.0f));
	}
};