#pragma once
#include "config.h"
enum Tile_Type { BLOCK = 0, AIR, FINISH};
class Tile
{
public:
	SDL_Rect position;
	Tile(Tile_Type type = BLOCK);
	Tile_Type get_tile();
	int set_tile(Tile_Type type);
	Tile_Type type;
	~Tile();
private:
};

