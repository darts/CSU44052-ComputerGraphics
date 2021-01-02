#pragma once

#ifndef SPIDER_TPL
#define SPIDER_TPL

#include <model.h>
#include <leg_tpl.h>

class Spider_tpl {
public:
	Spider_tpl() {
		//body_mat = glm::rotate(body_mat, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		this->init();
	}

	void Draw(Shader& shader, glm::vec2 translation, glm::vec2 rotation) {
		body_mat = glm::mat4(1.0f);
		float angle = atan2f(rotation.x, rotation.y);

		body_mat = translate(body_mat, glm::vec3(translation.x, 0.0f, translation.y));
		body_mat = glm::rotate(body_mat, (float)(angle - PI), glm::vec3(0.0f, 1.0f, 0.0f));
		for (int i = 0; i < 8; i++) {
			legs[i].ParentUpdate(body_mat);
			legs[i].Draw(shader);
		}

		glm::mat4 model = body_mat;
		model = glm::scale(model, glm::vec3(0.05f));
		shader.SetMat4("model", model);
		body.Draw(shader);
		tail.Draw(shader);

	}

	void Draw(Shader& shader) {
		for (int i = 0; i < 8; i++) {
			legs[i].Draw(shader);
		}
		glm::mat4 model = body_mat;
		model = glm::scale(model, glm::vec3(0.05f));
		shader.SetMat4("model", model);
		body.Draw(shader);
		tail.Draw(shader);
	}
	void UpdateMovement(float update_amount) {
		for (int i = 0; i < 8; i++) {
			legs[i].UpdateLoc(update_amount);
		}
	}
private:
	void init() {
		float one_offset = (float)atan(1) * 0.8f;

		//right legs
		for (int i = 0; i < 4; i++) {
			legs[i] = Leg_tpl(body_mat, -(float)i * one_offset, glm::vec3(0.0f), false, i % 2 == 0 ? 180 : 0);
		}

		//left legs
		for (int i = 0; i < 4; i++) {
			legs[i+4] = Leg_tpl(body_mat, ((float)i * one_offset), glm::vec3(-0.4f, 0.0f, -0.6f), true, i % 2 == 0 ? 90 : 270);
		}

		body = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_body.obj");
		tail = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_tail.obj");
	}
	Model body, tail;
	Leg_tpl legs[8];
	glm::mat4 body_mat = glm::mat4(1.0f);
};
#endif