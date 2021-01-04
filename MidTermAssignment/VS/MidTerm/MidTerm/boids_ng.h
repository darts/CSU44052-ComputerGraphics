#pragma once
#define DEBUG_BOIDS_NG false

#include <boid_ng.h>
#include <spider_tpl.h>

constexpr int NUM_BOIDS_NG = 20;
constexpr float LOCAL_RANGE = 13.0f;
constexpr float TOO_CLOSE = 6.0f;
constexpr int START_MAX = 15;
constexpr int START_RANGE = START_MAX * 2;
constexpr float MAX_X_NG = 20.0f;
constexpr float MIN_X_NG = -MAX_X_NG;
constexpr float MAX_Y_NG = 20.0f;
constexpr float MIN_Y_NG = -MAX_Y_NG;
constexpr float PLAYER_COLLISION_RANGE = 5.0f;

class Boids_ng {
public:
	Boid_ng boids[NUM_BOIDS_NG];
	Boid_ng boids_local[NUM_BOIDS_NG];
	int num_local_boids;

	Spider_tpl spiders[NUM_BOIDS_NG];

	Boids_ng() {
		init();
	}

	void DrawBoids(Shader the_shader) {
		for (int i = 0; i < NUM_BOIDS_NG; i++) {
			spiders[i].Draw(the_shader, boids[i].location, boids[i].vector);
		}
	}

	void MoveBoids(glm::vec2 player_pos, bool check_player_collision) {
		for (int i = 0; i < NUM_BOIDS_NG; i++) {
			if (!updateStayInBox(i)) {
				updateCoherence(i);
				updateSeparation(i);
				updateAlignment(i);
			}
			
			updatePlayerCollision(player_pos, i, check_player_collision);

			spiders[i].UpdateMovement(8);
			boids[i].Walk();
		}
	}

	int GetLocalBoids(int boid_index) {
		glm::vec2 location = boids[boid_index].location;
		int boid_count = 0;
		for (int i = 0; i < NUM_BOIDS_NG; i++) {
			float dist = getDistanceBetweenBoids(location, boids[i].location);
			if (dist < LOCAL_RANGE) {
				boids_local[boid_count++] = boids[i];
			}
		}
		return boid_count;
	}


private:
	void init() {
		for (int i = 0; i < NUM_BOIDS_NG; i++) {
			glm::vec2 location = glm::vec2((rand() % START_RANGE) - START_MAX, (rand() % START_RANGE) - START_MAX);
			glm::vec2 vector = glm::normalize(glm::vec2((rand() % START_RANGE) - START_MAX, (rand() % START_RANGE) - START_MAX));
			boids[i] = Boid_ng(location, vector);
		}
	}

	void updateCoherence(int boid_index) {
		int num_local = GetLocalBoids(boid_index);
		if (num_local > 1) {
			num_local_boids = num_local;
			glm::vec2 av_loc = getAverageLocation(num_local);
			boids[boid_index].SteerTowardsLocation(av_loc);
		}
	}

	void updateSeparation(int boid_index) {
		float closest_dist = std::numeric_limits<float>::max();
		int closest_index;
		for (int i = 0; i < NUM_BOIDS_NG; i++) {
			if (i != boid_index) {
				float dist = getDistanceBetweenBoids(boids[boid_index].location, boids[i].location);
				if (dist < closest_dist) {
					closest_dist = dist;
					closest_index = i;
				}
			}
		}

		if (closest_dist < TOO_CLOSE) {
			boids[boid_index].SteerAwayFromLocation(boids[closest_index].location);
		}
	}

	void updateAlignment(int boid_index) {
		glm::vec2 vector = glm::vec2(0.0f);
		for (int i = 0; i < num_local_boids; i++) {
			vector += boids_local[i].vector;
		}
		vector = glm::normalize(vector);

		boids[boid_index].SteerTowardsVector(vector);
	}

	bool updateStayInBox(int boid_index) {
		if (outOfBounds(boids[boid_index].location)) {
			boids[boid_index].SteerTowardsOrigin();
			return true;
		}
		return false;
	}

	bool outOfBounds(glm::vec2 location) {
		if (location.x > MAX_X_NG || location.x < MIN_X_NG || location.y > MAX_Y_NG || location.y < MIN_Y_NG) {
#if DEBUG_BOIDS_NG
			std::cout << location.x << " " << location.y << std::endl;
#endif
			return true;
		}
		return false;
	}

	float getDistanceBetweenBoids(glm::vec2 boid_a, glm::vec2 boid_b) {
		return glm::distance(boid_a, boid_b);
	}

	glm::vec2 getAverageLocation(int num_boids_in_locale) {
		glm::vec2 average = boids_local[0].location;

		for (int i = 1; i < num_boids_in_locale; i++) {
			average += boids_local[i].location;
		}

		average.x /= num_boids_in_locale;
		average.y /= num_boids_in_locale;

		return average;
	}

	void updatePlayerCollision(glm::vec2 player_pos, int boid_index, bool check_collision) {
		float dist_to_player = getDistanceBetweenBoids(player_pos, boids[boid_index].location);

		if (check_collision && dist_to_player < PLAYER_COLLISION_RANGE) {
			spiders[boid_index].SetNextDrawCycleColour(true, glm::vec3(0.8f, 0.1f, 0.1f));
		}
		else {
			spiders[boid_index].SetNextDrawCycleColour(false, glm::vec3(0.8f, 0.1f, 0.1f));
		}
	}
};