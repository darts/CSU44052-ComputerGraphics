#pragma once

#include <model.h>
#define PI atan(1) * 4

class Leg_tpl {
public:
	Leg_tpl() {
		//placeholder
	}

	Leg_tpl(glm::mat4 body_matrix, float initial_rotation, glm::vec3 initial_movement, bool is_inverted, float state_rot_offset) {
		this->body_mat = body_matrix;
		this->initial_rot_offset = initial_rotation;
		this->initial_mov_offset = initial_movement;
		this->is_inverted = is_inverted;
		this->state_offset = state_rot_offset;
		this->init();
	}
	void Draw(Shader& shader) {
		float mov_offset = getLocOffset();
		int inversion_multiplier = is_inverted ? -1 : 1;
		int inversion_rot_multiplier = inversion_multiplier;
		float inversion_offset = is_inverted ? -0.5 : 0;
		
		//inversion_rot_multiplier = 1;
		//inversion_offset = 0;

		float angular_offset = getAngularOffset();
		float vertical_offset = getVerticalOffset();

		//-----------------------------------------
		float UpperOffset_self_x = is_inverted ? 4.0f * scale_factor : 7.974f * (scale_factor * 0.5);
		float UpperOffset_self_z = is_inverted ? -6.0f * scale_factor : 12.112f * (scale_factor * 0.5);
		float UpperOffset_self_y = is_inverted ? 2.0f * scale_factor : -1.6995f * (scale_factor * 0.5);

		float UpperOffset_x = inversion_multiplier * 7.974f * scale_factor;
		float UpperOffset_z = inversion_multiplier * 12.112f * scale_factor;
		float UpperOffset_y = inversion_multiplier * -1.6995f * scale_factor;

		//UpperOffset_y = 0.0f;
		//UpperOffset_z = 0.0f;

		glm::mat4 UpperModel = glm::mat4(1.0f);
		UpperModel = glm::translate(UpperModel, this->initial_mov_offset);
		
		UpperModel = glm::translate(UpperModel, glm::vec3(UpperOffset_self_x, UpperOffset_self_y, -UpperOffset_self_z));
		UpperModel = glm::rotate(UpperModel, inversion_rot_multiplier * angular_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		UpperModel = glm::rotate(UpperModel, ((mov_offset + initial_rot_offset)), glm::vec3(0.0f, 1.0f, 0.0f));
		UpperModel = glm::rotate(UpperModel, vertical_offset, glm::vec3(1.0f, 0.0f, 0.0f));
		UpperModel = glm::translate(UpperModel, glm::vec3(-UpperOffset_self_x, -UpperOffset_self_y, UpperOffset_self_z));



		UpperModel = is_inverted ? UpperModel : glm::translate(UpperModel, glm::vec3(UpperOffset_x, UpperOffset_y, -UpperOffset_z));

		UpperModel = body_mat * UpperModel;

		//UpperModel = glm::scale(UpperModel, glm::vec3(ScaleFactor));

		shader.SetMat4("model", glm::scale(UpperModel, glm::vec3(scale_factor)));

		this->leg_upper.Draw(shader);
		//-----------------------------------------

		//-----------------------------------------
		float MiddleOffset_self_x = 1.5071f * scale_factor;
		float MiddleOffset_self_z = 4.6712f * scale_factor;
		float MiddleOffset_self_y = -1.6758f * scale_factor;

		//float MiddleOffset_x = 1.7188f * ScaleFactor;
		//float MiddleOffset_z = 4.5487f * ScaleFactor;
		//float MiddleOffset_y = -1.6017f * ScaleFactor;
		
		float MiddleOffset_x = is_inverted ? -UpperOffset_self_x - MiddleOffset_self_x + 0.025: (11.613f * scale_factor) - UpperOffset_x + MiddleOffset_self_x;
		float MiddleOffset_z = is_inverted ? -UpperOffset_self_z + MiddleOffset_self_z - 0.025: (17.54f * scale_factor) - UpperOffset_z + MiddleOffset_self_z;
		float MiddleOffset_y = is_inverted ? UpperOffset_self_y + MiddleOffset_self_y - 0.02: (-0.16364f * scale_factor) - UpperOffset_y + MiddleOffset_self_y;

		glm::mat4 MiddleModel = glm::mat4(1.0f); //multiply this with the parent matrix

		MiddleModel = glm::translate(MiddleModel, glm::vec3(MiddleOffset_self_x, MiddleOffset_self_y, -MiddleOffset_self_z));
		MiddleModel = glm::rotate(MiddleModel, vertical_offset * this->secondary_mov_ratio, glm::vec3(1.0f, 0.0f, 0.0f));
		MiddleModel = glm::translate(MiddleModel, glm::vec3(-MiddleOffset_self_x, -MiddleOffset_self_y, MiddleOffset_self_z));

		MiddleModel = glm::translate(MiddleModel, glm::vec3(MiddleOffset_x, MiddleOffset_y, -MiddleOffset_z));
		//MiddleModel = glm::scale(MiddleModel, glm::vec3(ScaleFactor));

		MiddleModel = UpperModel * MiddleModel;

		shader.SetMat4("model", glm::scale(MiddleModel, glm::vec3(scale_factor)));

		this->leg_middle.Draw(shader);
		//-----------------------------------------

		//-----------------------------------------
		float LowerOffset_self_x = MiddleOffset_self_x;
		float LowerOffset_self_z = MiddleOffset_self_z;
		float LowerOffset_self_y = MiddleOffset_self_y;

		float LowerOffset_x = is_inverted ? -MiddleOffset_self_x * 0.95 : MiddleOffset_self_x * 0.95;
		float LowerOffset_z = is_inverted ? MiddleOffset_self_z * 0.95 : MiddleOffset_self_z * 0.95;
		float LowerOffset_y = is_inverted ? MiddleOffset_self_y * 0.95 : MiddleOffset_self_y * 0.95;

		glm::mat4 LowerModel = glm::mat4(1.0f); //multiply this with the parent matrix

		LowerModel = glm::translate(LowerModel, glm::vec3(LowerOffset_self_x, LowerOffset_self_y, -LowerOffset_self_z));
		LowerModel = glm::rotate(LowerModel, vertical_offset * this->secondary_mov_ratio, glm::vec3(1.0f, 0.0f, 0.0f));
		LowerModel = glm::translate(LowerModel, glm::vec3(-LowerOffset_self_x, -LowerOffset_self_y, LowerOffset_self_z));

		LowerModel = glm::translate(LowerModel, glm::vec3(LowerOffset_x, LowerOffset_y, -LowerOffset_z));
		//LowerModel = glm::scale(LowerModel, glm::vec3(ScaleFactor));

		LowerModel = MiddleModel * LowerModel;

		shader.SetMat4("model", glm::scale(LowerModel, glm::vec3(scale_factor)));

		this->leg_lower.Draw(shader);
		//-----------------------------------------
	}

	void ParentUpdate(glm::mat4 mat) {
		this->body_mat = mat;
	}

	void UpdateLoc(float increment_amount) {
		this->movement_state += increment_amount;
	}

private:
	void init() {
		this->leg_upper = is_inverted ? Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_upper_flip.obj") : Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_upper.obj");
		this->leg_middle = is_inverted ? Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_middle_flip.obj") : Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_middle.obj");
		this->leg_lower = is_inverted ? Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_bottom_flip.obj") : Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/spider/spider_/spider_leg_bottom.obj");
	}

	float getLocOffset() {

		return 0.0f;
	}

	float getAngularOffset() {
		float current_state = convertDegToRad(convertStateToCircular());
		float current_angular_state = cos(current_state);
		return current_angular_state * mov_range_deg;
	}

	float getVerticalOffset() {
		float current_state = convertDegToRad(convertStateToCircular());
		float current_angular_state = sin(current_state + PI);
		if (current_angular_state < 0)
			current_angular_state = 0;
		return current_angular_state * mov_range_deg;
	}

	float convertDegToRad(float deg) {
		return (deg / 180) * PI;
	}

	float convertStateToCircular() {
		return fmod(movement_state + state_offset, 360.0f);
	}

	Model leg_upper, leg_middle, leg_lower;
	glm::mat4 body_mat;
	float movement_state = 0;
	float state_offset;
	float scale_factor = 0.05f;
	float secondary_mov_ratio = 1.4f;
	float initial_rot_offset;
	glm::vec3 initial_mov_offset;
	bool is_inverted;

	float mov_range = 60;
	float mov_range_deg = mov_range / 360;
};
