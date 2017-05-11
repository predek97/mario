#pragma once
#include "Character.h"

class Player: public Character
{
public:
	int jump();
	int update(double delta_time);
	char number_of_lifes = 3;
	Player();
	~Player();
	bool is_jumping = false;
	bool is_walking = false;
	char walking_sprite_number = 0;
	Uint32 last_sprite_change = 0;
};

