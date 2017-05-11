#include "Player.h"



int Player::jump()
{
	if (!is_jumping && collision_down) 
	{
		set_speed_y(-MARIO_JUMP_SPEED);
		is_jumping = true;
	}
	return 0;
}

int Player::update(double delta_time)
{
	if (is_jumping && !collision_up)
		position.y += MARIO_JUMP_SPEED*delta_time;
	Character::update(delta_time);		
	if (speed[0] != 0)
		is_walking = true;
	else
		is_walking = false;
	return 0;
}

Player::Player()
{
	position.x = 0;
	position.y = 0;//7*TILE_HEIGHT;
	position.w = MARIO_WIDTH;
	position.h = MARIO_HEIGHT;
}


Player::~Player()
{
	//Character::~Character();
}
