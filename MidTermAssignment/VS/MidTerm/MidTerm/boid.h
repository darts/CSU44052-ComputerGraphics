#pragma once

class Boid {
public: 
	float x_pos, y_pos, orientation;

	Boid(float x_pos, float y_pos, float orientation) {
		this->x_pos = x_pos;
		this->y_pos = y_pos;
		this->orientation = orientation;
	}
};