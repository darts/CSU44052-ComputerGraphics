#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <model.h>
#include <leg.h>

#pragma warning (disable: 4244) //getting the compiler to shut up about converting doubles to floats

class Spider {
public:
	enum class Spider_Directions {
		FORWARDS,
		BACKWARDS,
		LEFT,
		RIGHT
	};

	const int legCount = 8;
	const float turnSpeed = 0.4f;
	const int turnLegSpeedMultiplier = 8;
	const int walkLegSpeedMultiplier = 5;
	float loc = 0;
	glm::vec3 legTranslation = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 bodyModelMat = glm::mat4(1.0f);
	Model bodyModel;
	Leg legs[8];

	Spider() {}

	void Init() {
		initBody();
		initLegs();
	}

	void Draw(Shader& shader) {
		drawBody(shader);
		drawLegs(shader);
	}

	//handles key presses
	//including the time thing makes movement smoother
	//otherwise movement speed is seriously affected by the frame time
	void HandleKeyPress(Spider_Directions direction, float deltaTime) {
		float velocity = 1 * deltaTime;
		if (direction == Spider_Directions::FORWARDS) {
			this->bodyModelMat = glm::translate(this->bodyModelMat, glm::vec3(1.0 * velocity, 0.0f, 0.0f));
			this->loc += walkLegSpeedMultiplier * velocity;
		}
		if (direction == Spider_Directions::BACKWARDS) {
			this->bodyModelMat = glm::translate(this->bodyModelMat, glm::vec3(-1.0 * velocity, 0.0f, 0.0f));
			this->loc -= walkLegSpeedMultiplier * velocity;
		}
		if (direction == Spider_Directions::LEFT) {
			this->bodyModelMat = glm::rotate(this->bodyModelMat, -turnSpeed * velocity, glm::vec3(0.0f, 0.0f, 1.0f));
			this->loc -= turnLegSpeedMultiplier * velocity;
		}
		if (direction == Spider_Directions::RIGHT) {
			this->bodyModelMat = glm::rotate(this->bodyModelMat, turnSpeed * velocity, glm::vec3(0.0f, 0.0f, 1.0f));
			this->loc += turnLegSpeedMultiplier * velocity;
		}

		updateLegPos();
	}	

private:
	void updateLegPos() {
		for (int i = 0; i < legCount; i++) {
			legs[i].UpdateLoc(this->loc);
			legs[i].UpdateMat(this->bodyModelMat);
		}
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