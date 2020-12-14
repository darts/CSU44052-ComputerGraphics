#pragma once

#include <stdlib.h>
#include <boid.h>
#include <spider.h>
#include <cmath>
#include <maths/maths_funcs.h>

constexpr int NUM_BOIDS = 10;
constexpr float LOCAL_BOID_DIST = 10.0f; //radius to find boids in;
constexpr float THRESHOLD_DIST_TO_BORDER = 3.0f;
constexpr float THRESHOLD_DIST_TO_CLOSEST = 5.0f;

constexpr float MAX_X = 30.0f;
constexpr float MAX_Y = 30.0f;
//constexpr float MIN_X = -MAX_X;
//constexpr float MIN_Y = -MAX_Y;
constexpr float MIN_X = 0;
constexpr float MIN_Y = 0;
constexpr float MAX_MAX = max(MAX_X, MAX_Y);

class Boids {
public:
	struct coords {
		float x;
		float y;
	};

	Boids() {
		init_boids();
	}

	void DrawBoids(Shader& the_shader) {
		for (int i = 0; i < NUM_BOIDS; i++) {
			spider_arr[i].Draw(the_shader);
		}
	}

	void UpdateBoidsPos(float time_diff) {
		for (int i = 0; i < NUM_BOIDS; i++) {
			int local_boid_count = get_local_boids(i);
			float dist_to_border = get_distance_to_border(i);
			Spider c_sp = spider_arr[i];
			if (dist_to_border < THRESHOLD_DIST_TO_BORDER) {
				spider_arr[i].HandleKeyPress(Spider::Spider_Directions::LEFT, time_diff); //TODO determine the best direction to turn
			}
			else if (dist_to_closest < THRESHOLD_DIST_TO_CLOSEST) {
				spider_arr[i].HandleKeyPress(Spider::Spider_Directions::RIGHT, time_diff); //TODO determine which way to turn
			}
			else {
				coords c_o_m = get_com_of_local(local_boid_count);
				//steer_towards(COM)

				float locale_orientation = get_orientation_of_local(local_boid_count);
			}
			spider_arr[i].HandleKeyPress(Spider::Spider_Directions::FORWARDS, time_diff);;
		}
	}

private:
	Spider spider_arr[NUM_BOIDS];
	Spider local_arr[NUM_BOIDS];
	float dist_to_closest;
	//Shader &the_shader;

	void init_boids() {
		//spider_arr = new Spider[NUM_BOIDS];
		srand(time(NULL));

		for (int i = 0; i < NUM_BOIDS; i++) {
			float x_offset = (float)(rand() % 5);
			float y_offset = (float)(rand() % 5);
			float orientation = (float)(rand() % 360);

			spider_arr[i].Init(x_offset, y_offset, orientation);
		}
	}

	//returns the number of boids in the locale
	int get_local_boids(int current_index) {
		Spider current_spider = spider_arr[current_index];
		dist_to_closest = MAX_MAX + 1;
		int num_in_locale = 0;
		for (int i = 0; i < NUM_BOIDS; i++) {
			if (current_index == i)
				continue;
			float dist = sqrt(pow(abs(current_spider.x_pos) - abs(spider_arr[i].x_pos), 2) + pow(abs(current_spider.y_pos) - abs(spider_arr[i].y_pos), 2));
			if (dist < LOCAL_BOID_DIST)
				local_arr[num_in_locale++] = spider_arr[i];
			if (dist < dist_to_closest)
				dist_to_closest = dist;
		}
		return num_in_locale;
	}

	float get_distance_to_border(int current_index) {
		Spider c_sp = spider_arr[current_index];
		/*coords bt_left = coords{ MIN_X, MIN_Y };
		coords bt_right = coords{ MAX_X, MIN_Y };
		coords t_left = coords{ MIN_X, MAX_Y };
		coords t_right = coords{ MAX_X, MAX_Y };*/

		if(c_sp.orientation == 0)
			return MAX_Y - c_sp.y_pos;
		if (c_sp.orientation == 90)
			return MAX_X - c_sp.x_pos;
		if (c_sp.orientation == 180)
			return c_sp.y_pos;
		if (c_sp.orientation == 270)
			return c_sp.x_pos;


		float dist_a, dist_b;
		if (c_sp.orientation > 270 || c_sp.orientation < 90) {
			float str_ln = MAX_Y - c_sp.y_pos;
			dist_a = str_ln / cos(fmod(c_sp.orientation, 90));
		}
		if (c_sp.orientation > 90 && c_sp.orientation < 270) {
			float str_ln = c_sp.y_pos - MIN_Y; 
			dist_a = str_ln / cos(fmod(c_sp.orientation, 90));
		}
		if (c_sp.orientation < 180) {
			float str_ln = MAX_X - c_sp.x_pos;
			dist_b = str_ln / cos(fmod(c_sp.orientation, 90));
		}
		if (c_sp.orientation > 180) {
			float str_ln = c_sp.x_pos - MIN_X;
			dist_b = str_ln / cos(fmod(c_sp.orientation, 90));
		}

		return min(dist_a, dist_b);

		//return min({ c_sp.x_pos - MIN_X, c_sp.y_pos - MIN_Y, MAX_X - c_sp.x_pos, MAX_Y - c_sp.y_pos });
	}

	float angle_between_coords(coords current, coords target) {

	}

	coords get_com_of_local(int num_local) {
		float x = 0.0f;
		float y = 0.0f;
		for (int i = 0; i < num_local; i++) {
			x += local_arr[i].x_pos;
			y += local_arr[i].y_pos;
		}
		return coords{ x / (float)num_local, y / (float)num_local };
	}

	float get_orientation_of_local(int num_local) {
		float orientation = 0;
		for (int i = 0; i < num_local; i++) {
			orientation += local_arr[i].orientation;
		}
		return orientation / (float)num_local;
	}
};