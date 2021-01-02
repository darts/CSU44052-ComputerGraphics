#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float SCALE_FACTOR = 4.0f;
constexpr float COHERENCE_FACTOR = 0.1f / SCALE_FACTOR;
constexpr float SEPARATION_FACTOR = 0.2f / SCALE_FACTOR;
constexpr float ALIGNMENT_FACTOR = 0.06f / SCALE_FACTOR;
constexpr float RTB_FACTOR = 2.0f / SCALE_FACTOR;
constexpr float SPEED_SCALE = 0.04f;

class Boid_ng {
public:
	glm::vec2 location;
	glm::vec2 vector; //https://youtu.be/fVq4_HhBK8Y

	Boid_ng() {
		location = glm::vec2(0.0f);
		vector = glm::vec2(0.0f);
	}

	Boid_ng(glm::vec2 location, glm::vec2 vector) {
		this->location = location;
		this->vector = vector;
	}

	void Walk() {
		this->vector = glm::normalize(this->vector);
		glm::vec2 tmp_vector = glm::vec2(this->vector.x * SPEED_SCALE, this->vector.y * SPEED_SCALE);
		this->location = this->location + tmp_vector;
	}

	//updates vector to aim for a target location
	void SteerTowardsLocation(glm::vec2 target_loc) {
		glm::vec2 ideal_vector = glm::vec2(target_loc.x - location.x, target_loc.y - location.y);
		ideal_vector = glm::normalize(ideal_vector);

		steerTowardsVector(ideal_vector, COHERENCE_FACTOR);
	}

	void SteerAwayFromLocation(glm::vec2 avoid_loc) {
		glm::vec2 ideal_vector = glm::vec2(avoid_loc.x - location.x, avoid_loc.y - location.y);
		ideal_vector = glm::normalize(ideal_vector);
		ideal_vector = glm::vec2(-ideal_vector.x, -ideal_vector.y);

		steerTowardsVector(ideal_vector, SEPARATION_FACTOR);
	}

	void SteerTowardsVector(glm::vec2 target_vector) {
		steerTowardsVector(target_vector, ALIGNMENT_FACTOR);
	}

	void SteerTowardsOrigin() {
		glm::vec2 ideal_vector = glm::vec2(-location.x, -location.y);
		ideal_vector = glm::normalize(ideal_vector);

		steerTowardsVector(ideal_vector, RTB_FACTOR);
	}

private:
	void steerTowardsVector(glm::vec2 target_vector, float steer_factor) {
		if (target_vector.x > vector.x)
			vector.x += steer_factor;
		else
			vector.x -= steer_factor;

		if (target_vector.y > vector.y)
			vector.y += steer_factor;
		else
			vector.y -= steer_factor;

		vector = glm::normalize(vector);
	}
};