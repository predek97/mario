#pragma once
#include "config.h"
class Character
{
public:
	Character();
	~Character();
	int update(double delta_time);
	int set_speed_x(double x);
	int set_speed_y(double y);
	int set_speed(double x, double y);
	double speed[2];
	bool flip = false;
	bool collision_down = false;
	bool collision_up = false;
	bool collision_left = false;
	bool collision_right = false;
	SDL_Rect position;
	SDL_Texture * sprite;
private:
};

