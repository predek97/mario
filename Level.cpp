#include "Level.h"



Level::Level(char level_to_load[])
{
	/*for (int i = 0; i < map_width; i++)
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			grid[i][j] = (j >= 12) ? BLOCK : AIR;
			grid[i][j].position.x = i * TILE_WIDTH;
			grid[i][j].position.y = j * TILE_HEIGHT;
		}
	grid[5][11].set_tile(BLOCK);
	grid[5][10].set_tile(BLOCK);
	for (int i = 8; i < 15; i++)
		grid[i][8].set_tile(BLOCK);
			
*/
	if(level_to_load != "NONE")
		load_from_file(level_to_load);
	camera_position.h = SCREEN_HEIGHT;
	camera_position.w = SCREEN_WIDTH;
	camera_position.y = map_height / 2;
}
Level::~Level()
{
	if (grid != NULL)
	{
		for (int i = 0; i < map_width; i++)
			delete[] grid[i];
		delete[] grid;
	}
	SDL_DestroyTexture(tile_texture);
}

int Level::update(Uint32 delta_time)
{
	if (finish)
	{
		if (level_count < NUMBER_OF_LEVELS)
		{
			char buffer[BUFFER_SIZE];
			level_count += 1;
			sprintf(buffer, "levels/level%d.lvl", level_count);
			load_from_file(buffer);
			finish = false;
		}
		else
			return 3;
	}
	time_left -= delta_time;
	//camera_position.x = (mario.position.x % SCREEN_WIDTH)-1;
	//printf("%d %d\n", camera_position.x, mario.position.x);
	detect_collisions();
	mario.update(delta_time);
	camera_position.x = (mario.position.x + mario.position.w) - (SCREEN_WIDTH)/2;
	if (camera_position.x < 0)
		camera_position.x = 0;

	if (camera_position.x > (map_width*TILE_WIDTH - camera_position.w))
		camera_position.x = map_width*TILE_WIDTH - camera_position.w;
	if (time_left <= 1000 || mario.position.y > map_height*TILE_HEIGHT)
	{
		--mario.number_of_lifes;
		if (mario.number_of_lifes > 0)
			return 1;
		else
			return 2;
	}		
	return 0;
}

int Level::save_to_file(char * filename)
{
	FILE *opened_file = fopen(filename, "w");
	printf("Saving to file! %d", opened_file);
	if (opened_file != NULL)
	{
		char *buffer = new char[BUFFER_SIZE];
		fputs(_itoa(map_width, buffer, 10), opened_file);
		fputs("\n", opened_file);
		fputs(_itoa(map_height, buffer, 10), opened_file);
		fputs("\n", opened_file);
		fputs(_itoa(time_limit, buffer, 10), opened_file);
		fputs("\n", opened_file);
		fputs(_itoa(start_position_x, buffer, 10), opened_file);
		fputs("\n", opened_file);
		fputs(_itoa(start_position_y, buffer, 10), opened_file);
		fputs("\n", opened_file);
		for (int i = 0; i < map_height; i++)
		{
		for (int j = 0; j < map_width; j++)
		{
		fputs(_itoa(grid[j][i].get_tile(), buffer, 10), opened_file);
		fputs(" ", opened_file);
		}
		fputs("\n", opened_file);
		}
		fclose(opened_file);
	}
	return 0;
}

Level & Level::operator=(Level level_to_copy)
{
	// TODO: insert return statement here
	start_position_x = level_to_copy.start_position_x;
	start_position_y = level_to_copy.start_position_y;
	map_width = level_to_copy.map_width;
	map_height = level_to_copy.map_height;
	grid = level_to_copy.grid;
	background = level_to_copy.background;
	tile_texture = level_to_copy.tile_texture;
	mario = level_to_copy.mario;
	camera_position = level_to_copy.camera_position;
	time_left = level_to_copy.time_left;
	time_limit = level_to_copy.time_limit;
	return *this;
}

int Level::load_from_file(char * filename)
{
	FILE *opened_file = fopen(filename, "r");
	printf("Loading from file! %s\n", filename);
	if (opened_file != NULL)
	{
		if (grid != NULL)
		{
			for (int i = 0; i < map_width; i++)
				delete[] grid[i];
			delete[] grid;
		}
		int temp = 0;
		fscanf(opened_file, "%d", &temp);
		map_width = temp;
		//printf("%d ", map_width);
		fscanf(opened_file, "%d", &temp);
		map_height = temp;
		//printf("%d ", map_height);
		fscanf(opened_file, "%d", &temp);
		time_limit = temp;
		time_left = time_limit;
		//printf("%d ", time_limit);
		fscanf(opened_file, "%d", &temp);
		start_position_x = temp;
		//printf("%d ", start_position_x);
		fscanf(opened_file, "%d", &temp);
		start_position_y = temp;
		//printf("%d\n", start_position_y);
		mario.position.y = start_position_y;
		mario.position.x = start_position_x;
		grid = new Tile *[map_width];
		for (int i = 0; i < map_width; i++)
			grid[i] = new Tile[map_height];

		for (int i = 0; i < map_height; i++)
		{
			
			for (int j = 0; j < map_width; j++)
			{
				
				fscanf(opened_file, "%d", &temp);
				grid[j][i].set_tile((Tile_Type)temp);
				//printf("%d ", temp);
				grid[j][i].position.x = j * TILE_WIDTH;
				grid[j][i].position.y = i * TILE_HEIGHT;
			}
			//printf("\n");

		}
		/*for (int i = 0; i < map_width; i++)
			for (int j = 0; j < map_height; j++)
			{
				grid[i][j].position.x = i * TILE_WIDTH;
				grid[i][j].position.y = j * TILE_HEIGHT;
			}*/
	}
	else
		printf("Something went wrong trying to load file %s\n", filename);
	fclose(opened_file);
	return 0;
}

int Level::detect_collisions()
{
	mario.collision_down = false;
	mario.collision_up = false;
	mario.collision_right = false;
	mario.collision_left = false;
	mario.collision_right = (mario.position.x + mario.position.w >= map_width*TILE_WIDTH);
	mario.collision_left = (mario.position.x <= 0);
	for(int i = 0; i < map_width; i++)
		for (int j = 0; j < map_height; j++)
		{
			if (grid[i][j].get_tile() != AIR)
			{
				SDL_Rect intersection;
				SDL_IntersectRect(&grid[i][j].position, &mario.position, &intersection);
				if (intersection.w > 0 && intersection.h > 0)
				{
					if (grid[i][j].position.y > mario.position.y && intersection.w > intersection.h)
					{
						mario.collision_down = true;
						mario.position.y -= intersection.h - 1;
					}
						
					if (grid[i][j].position.y < mario.position.y && intersection.w > intersection.h)
					{
						mario.collision_up = true;
						mario.position.y += intersection.h;
					}
						
					if (grid[i][j].position.x > mario.position.x && intersection.h > intersection.w)
					{
						mario.collision_right = true;
						mario.position.x -= intersection.w;
					}
						
					if (grid[i][j].position.x < mario.position.x && intersection.h > intersection.w)
					{
						mario.collision_left = true;
						mario.position.x += intersection.w;
					}
					if ((mario.collision_down || mario.collision_up) && intersection.h > 1)
					{
						mario.is_jumping = false;
					}
					if (grid[i][j].get_tile() == FINISH)
						finish = true;
				}
				

			}
		}
	return 0;
}
