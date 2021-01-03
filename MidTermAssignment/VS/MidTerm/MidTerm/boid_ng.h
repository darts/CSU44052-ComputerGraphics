#pragma once
#define DEBUG_BOID_NG false

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float SCALE_FACTOR = 7.0f;
constexpr float COHERENCE_FACTOR = 0.1f / SCALE_FACTOR;
constexpr float SEPARATION_FACTOR = 0.2f / SCALE_FACTOR;
constexpr float ALIGNMENT_FACTOR = 0.06f / SCALE_FACTOR;
constexpr float RTB_FACTOR = 0.2f / SCALE_FACTOR;
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
		glm::vec2 tmp_vec = ideal_vector;
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
		return steerTowardsVector2(target_vector, steer_factor);

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

	void steerTowardsVector2(glm::vec2 target_vector, float steer_factor) {
		float target_vector_angle = atan2f(target_vector.x, target_vector.y);
		if (isnan(target_vector_angle))
			std::cout << endl;
		float current_vector_angle = atan2f(vector.x, vector.y);

#if DEBUG_BOID_NG
		std::cout << target_vector_angle << " " << current_vector_angle << " ";
#endif

		if (target_vector_angle < 0)
			target_vector_angle = target_vector_angle + (2 * PI);
		if (current_vector_angle < 0)
			current_vector_angle = current_vector_angle + (2 * PI);

		if (abs(target_vector_angle - current_vector_angle) > PI) {
			if (target_vector_angle > current_vector_angle) {
				current_vector_angle -= min((float)(((2 * PI) - target_vector_angle) + current_vector_angle), steer_factor);
			}
			else {
				current_vector_angle += min((float)(((2 * PI) - current_vector_angle) + target_vector_angle), steer_factor);
			}
		}
		else {
			if (target_vector_angle > current_vector_angle) {
				current_vector_angle += min(target_vector_angle - current_vector_angle, steer_factor);
			}
			else {
				current_vector_angle -= min(current_vector_angle - target_vector_angle, steer_factor);
			}
		}
#if DEBUG_BOID_NG
		std::cout << current_vector_angle << std::endl;
#endif

		vector = glm::vec2(sin(current_vector_angle), cos(current_vector_angle));

		if (isnan(vector.x) || isnan(vector.y))
			std::cout << endl;
		vector = glm::normalize(vector);
	}
};