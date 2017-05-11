#include "Tile.h"




Tile::Tile(Tile_Type type)
{
	this->type = type;
	position.h = TILE_HEIGHT;
	position.w = TILE_WIDTH;
}

Tile_Type Tile::get_tile()
{
	return type;
}

int Tile::set_tile(Tile_Type type)
{
	this->type = type;
	return 0;
}


Tile::~Tile()
{
}
