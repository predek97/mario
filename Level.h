#pragma once
#include "config.h"
#include "Tile.h"
#include "Player.h"
class Level
{
public:
	Level(char level_to_load[]);
	~Level();
	bool finish = false;
	int update(Uint32 delta_time);
	Uint32 time_limit;
	Uint32 time_left = time_limit;
	SDL_Rect camera_position;
	Player mario;
	SDL_Texture* tile_texture;
	SDL_Texture* background;
	Tile** grid = NULL;
	int map_width;
	int map_height;
	int save_to_file(char* filename);
	Level & Level::operator=(Level level_to_copy);
	unsigned level_count = 1;
private:
	int start_position_x;
	int start_position_y;
	int load_from_file(char* filename);
	int detect_collisions();
};

