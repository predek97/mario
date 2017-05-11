#pragma once
#include "Character.h"


Character::Character()
{

}


Character::~Character()
{
	SDL_DestroyTexture(sprite);
}

int Character::update(double delta_time)
{
	if (!collision_down)
		speed[1] += FREE_FALL_ACCELERATION * delta_time;
	//else if (collision_up)
		//speed[1] = speed[1] > 0 ? 0 : speed[1];
	else
		speed[1] = speed[1] < 0 ? speed[1] : 0;
	if(collision_up)
		speed[1] = speed[1] > 0 ? 0 : speed[1];
	if ((collision_right && speed[0] > 0) || (collision_left && speed[0] < 0))
		speed[0] = 0;
	//if (collision_left)
		//
	//("collision right!");
	//printf("%d \n", position.x);s
	/*if(speed[0] != 0)
		speed[0] -= speed[0] > 0 ? MARIO_DEACCELERATION * delta_time : -MARIO_DEACCELERATION * delta_time;
	if (speed[1] != 0)
		speed[1] -= speed[1] > 0 ? MARIO_DEACCELERATION * delta_time : -MARIO_DEACCELERATION * delta_time;*/
	position.x += speed[0] * delta_time;
	position.y += speed[1] * delta_time;
	if ((speed[0] > 0 && !flip) || (speed[0] < 0 && flip))
		flip = !flip;
	return 0;
}

int Character::set_speed(double x, double y)
{
	speed[0] = x;
	speed[1] = y;
	return 0;
}
int Character::set_speed_x(double x)
{
	speed[0] = x;
	return 0;
}

int Character::set_speed_y(double y)
{
	speed[1] = y;
	return 0;
}

