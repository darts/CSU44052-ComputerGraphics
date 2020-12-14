#ifndef SPIDER_BOI
#define SPIDER_BOI

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <model.h>
#include <leg.h>

#pragma warning (disable: 4244) //getting the compiler to shut up about converting doubles to floats

constexpr float OFFSET_TWEAK = 4.0f;
constexpr int legCount = 8;
constexpr float turnSpeed = 0.4f;
constexpr int turnLegSpeedMultiplier = 8;
constexpr int walkLegSpeedMultiplier = 5;
constexpr float movement_speed_modifier = 1.0f;

class Spider {
public:
	float x_pos, y_pos, orientation;

	enum class Spider_Directions {
		FORWARDS,
		BACKWARDS,
		LEFT,
		RIGHT
	};

	Spider() {}

	Spider(float x_offset, float y_offset, float orientation) {
		x_pos = x_offset;
		y_pos = y_offset;
		orientation = orientation;
		Init();
	}

	void Init(float x_offset, float y_offset, float orientation) {
		this->x_pos = x_offset;
		this->y_pos = y_offset;
		this->orientation = orientation;
		Init();
	}

	void Init() {
		initBody();
		moveForward(x_pos * OFFSET_TWEAK);
		turnRight(90 / turnSpeed);
		moveForward(y_pos * OFFSET_TWEAK);
		turnLeft(90 / turnSpeed);
		initLegs();
	}

	void Draw(Shader& shader) {
		drawBody(shader);
		drawLegs(shader);
	}

	//handles key presses
	//including the time thing makes movement smoother
	//otherwise movement speed is seriously affected by the frame time
	void HandleKeyPress(Spider_Directions direction, float frame_time) {
		float velocity = movement_speed_modifier * frame_time;
		if (direction == Spider_Directions::FORWARDS) {
			moveForward(velocity);
		}
		if (direction == Spider_Directions::BACKWARDS) {
			moveBackward(velocity);
		}
		if (direction == Spider_Directions::LEFT) {
			turnLeft(velocity);
		}
		if (direction == Spider_Directions::RIGHT) {
			turnRight(velocity);
		}

		updateLegPos();
	}	

private:
	float loc = 0;
	glm::vec3 legTranslation = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 bodyModelMat = glm::mat4(1.0f);
	Model bodyModel;
	Leg legs[8];

	void moveForward(float velocity) {
		this->bodyModelMat = glm::translate(this->bodyModelMat, glm::vec3(1.0 * velocity, 0.0f, 0.0f));
		//this->x_pos += 
		this->loc += walkLegSpeedMultiplier * velocity;
	}

	void moveBackward(float velocity) {
		this->bodyModelMat = glm::translate(this->bodyModelMat, glm::vec3(-1.0 * velocity, 0.0f, 0.0f));
		this->loc -= walkLegSpeedMultiplier * velocity;
	}

	void turnLeft(float velocity) {
		this->bodyModelMat = glm::rotate(this->bodyModelMat, -turnSpeed * velocity, glm::vec3(0.0f, 0.0f, 1.0f));
		this->loc -= turnLegSpeedMultiplier * velocity;
		this->orientation -= turnSpeed * velocity;
		normaliseOrientation();
	}

	void turnRight(float velocity) {
		this->bodyModelMat = glm::rotate(this->bodyModelMat, turnSpeed * velocity, glm::vec3(0.0f, 0.0f, 1.0f));
		this->loc += turnLegSpeedMultiplier * velocity;
		this->orientation += turnSpeed * velocity;
		normaliseOrientation();
	}

	void updateLegPos() {
		for (int i = 0; i < legCount; i++) {
			legs[i].UpdateLoc(this->loc);
			legs[i].UpdateMat(this->bodyModelMat);
		}
	}

	void normaliseOrientation() {
		this->orientation = fmod(this->orientation, 360.0f);
	}

	void initBody() {
		this->bodyModelMat = glm::rotate(this->bodyModelMat, (float)(atan(1) * 4 / 2), glm::vec3(1.0f, 0.0f, 0.0f));
		this->bodyModel = Model("D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/main_body.obj");
	}

	void initLegs() {
		for (int i = 0; i < (legCount / 2); i++) {
			glm::mat4 leg = glm::mat4(1.0f);
			
			leg = glm::translate(leg, glm::vec3(-0.7f, 0.3f, 0.3f));

			leg = glm::translate(leg, legTranslation * ((float)i / 2));

			//add this back but gotta curve the other side the other way
			//leg = glm::rotate(leg, -(float)i / 6, glm::vec3(0.0f, 0.0f, 1.0f)); 
			
			const float PI = atan(1) * 4;

			legs[i] = Leg(
				"D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/lower_leg.obj",
				"D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/lower_leg.obj",
				leg,
				false,
				getOffset(i) * (PI/2));

			leg = glm::rotate(leg, PI, glm::vec3(0.0f, 0.0f, 1.0f));
			leg = glm::translate(leg, glm::vec3(0.0f, 0.8f, 0.0f));

			legs[i+4] = Leg(
				"D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/lower_leg.obj",
				"D:/Git-Some/CSU44052-ComputerGraphics/MidTermAssignment/models/lower_leg.obj",
				leg,
				true,
				getOffset(i+4) * (PI/2));
		}
		updateLegPos();
	}

	int getOffset(int index) {
		switch (index)
		{
		case 0:
		case 5:
			return 1;
		case 2:
		case 7:
			return 2;
		case 1:
		case 4:
			return 3;
		default:
			return 4;
		}
	}

	void drawBody(Shader& shader) {
		shader.SetMat4("model", this->bodyModelMat);
		bodyModel.Draw(shader);
	}

	void drawLegs(Shader& shader) {
		for (int i = 0; i < legCount; i++) {
			legs[i].Draw(shader);
		}
	}
};
#endif // !SPIDER_BOI