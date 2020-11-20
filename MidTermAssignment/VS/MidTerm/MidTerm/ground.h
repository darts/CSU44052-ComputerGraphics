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
		plane_model = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/ground_plane.obj");
	}

	void Draw(Shader& shader) {
		shader.use();
		shader.SetMat4("model", starting_position);
		plane_model.Draw(shader);
	}
};