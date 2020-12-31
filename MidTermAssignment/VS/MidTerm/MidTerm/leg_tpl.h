#pragma once

#include <model.h>

class Leg_tpl {
public:
	Leg_tpl() {
		//placeholder
	}

	Leg_tpl(glm::mat4 body_matrix) {
		this->body_mat = body_matrix;
		this->init();
	}
	void Draw(Shader& shader) {
		float ScaleFactor = 0.05f;

		//-----------------------------------------
		float UpperOffset_self_x = 7.974f * (ScaleFactor * 0.5);
		float UpperOffset_self_z = 12.112f * (ScaleFactor * 0.5);
		float UpperOffset_self_y = -1.6995f * (ScaleFactor * 0.5);

		float UpperOffset_x = 7.974f * ScaleFactor;
		float UpperOffset_z = 12.112f * ScaleFactor;
		float UpperOffset_y = -1.6995f * ScaleFactor;

		//UpperOffset_y = 0.0f;
		//UpperOffset_z = 0.0f;

		glm::mat4 UpperModel = glm::mat4(1.0f);
		
		UpperModel = glm::translate(UpperModel, glm::vec3(UpperOffset_self_x, UpperOffset_self_y, -UpperOffset_self_z));
		UpperModel = glm::rotate(UpperModel, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		UpperModel = glm::translate(UpperModel, glm::vec3(-UpperOffset_self_x, -UpperOffset_self_y, UpperOffset_self_z));

		UpperModel = glm::translate(UpperModel, glm::vec3(UpperOffset_x, UpperOffset_y, -UpperOffset_z));

		UpperModel = body_mat * UpperModel;

		//UpperModel = glm::scale(UpperModel, glm::vec3(ScaleFactor));

		shader.SetMat4("model", glm::scale(UpperModel, glm::vec3(ScaleFactor)));

		this->leg_upper.Draw(shader);
		//-----------------------------------------

		//-----------------------------------------
		float MiddleOffset_self_x = 1.5071f * ScaleFactor;
		float MiddleOffset_self_z = 4.6712f * ScaleFactor;
		float MiddleOffset_self_y = -1.6758f * ScaleFactor;

		//float MiddleOffset_x = 1.7188f * ScaleFactor;
		//float MiddleOffset_z = 4.5487f * ScaleFactor;
		//float MiddleOffset_y = -1.6017f * ScaleFactor;
		
		float MiddleOffset_x = (11.613f * ScaleFactor) - UpperOffset_x + MiddleOffset_self_x;
		float MiddleOffset_z = (17.54f * ScaleFactor) - UpperOffset_z + MiddleOffset_self_z;
		float MiddleOffset_y = (-0.16364f * ScaleFactor) - UpperOffset_y + MiddleOffset_self_y;

		glm::mat4 MiddleModel = glm::mat4(1.0f); //multiply this with the parent matrix

		MiddleModel = glm::translate(MiddleModel, glm::vec3(MiddleOffset_self_x, MiddleOffset_self_y, -MiddleOffset_self_z));
		MiddleModel = glm::rotate(MiddleModel, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		MiddleModel = glm::translate(MiddleModel, glm::vec3(-MiddleOffset_self_x, -MiddleOffset_self_y, MiddleOffset_self_z));

		MiddleModel = glm::translate(MiddleModel, glm::vec3(MiddleOffset_x, MiddleOffset_y, -MiddleOffset_z));
		//MiddleModel = glm::scale(MiddleModel, glm::vec3(ScaleFactor));

		MiddleModel = UpperModel * MiddleModel;

		shader.SetMat4("model", glm::scale(MiddleModel, glm::vec3(ScaleFactor)));

		this->leg_middle.Draw(shader);
		//-----------------------------------------

		//-----------------------------------------
		float LowerOffset_self_x = MiddleOffset_self_x;
		float LowerOffset_self_z = MiddleOffset_self_z;
		float LowerOffset_self_y = MiddleOffset_self_y;

		float LowerOffset_x = MiddleOffset_self_x * 0.95;
		float LowerOffset_z = MiddleOffset_self_z * 0.95;
		float LowerOffset_y = MiddleOffset_self_y * 0.95;

		glm::mat4 LowerModel = glm::mat4(1.0f); //multiply this with the parent matrix

		LowerModel = glm::translate(LowerModel, glm::vec3(LowerOffset_self_x, LowerOffset_self_y, -LowerOffset_self_z));
		LowerModel = glm::rotate(LowerModel, (float)atan(1)*0, glm::vec3(1.0f, 0.0f, 0.0f));
		LowerModel = glm::translate(LowerModel, glm::vec3(-LowerOffset_self_x, -LowerOffset_self_y, LowerOffset_self_z));

		LowerModel = glm::translate(LowerModel, glm::vec3(LowerOffset_x, LowerOffset_y, -LowerOffset_z));
		//LowerModel = glm::scale(LowerModel, glm::vec3(ScaleFactor));

		LowerModel = MiddleModel * LowerModel;

		shader.SetMat4("model", glm::scale(LowerModel, glm::vec3(ScaleFactor)));

		this->leg_lower.Draw(shader);
		//-----------------------------------------
	}

	void ParentUpdate(glm::mat4 mat) {
		this->body_mat = mat;
	}

private:
	void init() {
		this->leg_upper = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_upper.obj");
		this->leg_middle = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_middle.obj");
		this->leg_lower = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_bottom.obj");
	}
	Model leg_upper, leg_middle, leg_lower;
	glm::mat4 body_mat;
};
