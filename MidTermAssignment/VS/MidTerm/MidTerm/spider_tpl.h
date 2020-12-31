#pragma once

#ifndef SPIDER_TPL
#define SPIDER_TPL

#include <model.h>
#include <leg_tpl.h>

class Spider_tpl {
public:
	Spider_tpl() {
		this->init();
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
private:
	void init() {
		for (int i = 0; i < 8; i++) {
			legs[i] = Leg_tpl(body_mat);
		}
		body = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_body.obj");
		tail = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_tail.obj");
	}
	Model body, tail;
	Leg_tpl legs[8];
	glm::mat4 body_mat = glm::mat4(1.0f);
};
#endif