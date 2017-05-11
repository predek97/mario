#pragma once
#include "Program.h"



Program::Program()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_GetWindowSize(window, &screen_width, &screen_height);
				SDL_RenderSetScale(renderer, screen_width / SCREEN_WIDTH, screen_height / SCREEN_HEIGHT);
				SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 0);
				current_level = load_level("levels/level1.lvl");
				charset = load_texture("textures/cs8x8.bmp");
				heart_texture = load_texture("textures/hearts.bmp");
				Uint64 NOW = SDL_GetTicks();
				Uint64 LAST = 0;
				delta_time = 0;
				fps_timer = 0;
				frames = 0;
			}
			
		}
	}

}


Program::~Program()
{
}

int Program::handle_events()
{
	last = now;
	now = SDL_GetTicks();

	delta_time = static_cast<double>(now - last);
	//printf("%f\n", delta_time);
	fps_timer += delta_time;
	frames += 1;
	if (fps_timer > 1) {
		//printf("%f\n", static_cast<double>(delta_time));
		frames = 0;
		fps_timer -= 1;
	}
	if (developer_mode)
		current_level->time_left += delta_time;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
		case SDL_QUIT:
			is_ending = true; break;
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			current_level->mario.set_speed_x(MARIO_MAX_SPEED);
		}
		else if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			current_level->mario.set_speed_x(-MARIO_MAX_SPEED);
		}
		else
			current_level->mario.set_speed_x(0);
		if (currentKeyStates[SDL_SCANCODE_UP])
			current_level->mario.jump();
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
			is_ending = true;
		if (currentKeyStates[SDL_SCANCODE_F1])
			developer_mode = !developer_mode;
		if (currentKeyStates[SDL_SCANCODE_S])
			save_temp_level();
		if (currentKeyStates[SDL_SCANCODE_L])
			load_temp_level();
		if (currentKeyStates[SDL_SCANCODE_N])
		{
			if (!in_menu)
				reload_level();
			else
				start_game();
		}
			
		if (currentKeyStates[SDL_SCANCODE_F])
			set_fullscreen();

		if (developer_mode) {
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouse_x, mouse_y;
				SDL_GetMouseState(&mouse_x, &mouse_y);
				int tile_x = (mouse_x + current_level->camera_position.x) / TILE_WIDTH, tile_y = mouse_y / TILE_HEIGHT;
				current_level->grid[tile_x][tile_y].set_tile(current_level->grid[tile_x][tile_y].get_tile() == BLOCK ? AIR : BLOCK);
			}
			if (currentKeyStates[SDL_SCANCODE_F2])
				current_level->save_to_file("levels/level3.lvl");
		}
	}
	int action_to_take = current_level->update(delta_time);
	switch(action_to_take)
	{
	case 3:
		in_menu = true;
		sprintf(message_container, "Gratulacje, wygrales! Kliknij N aby rozpoczac nowa gre!");
		break;
	case 2:
		in_menu = true;
		sprintf(message_container, "Przegrales! Kliknij N aby rozpoczac nowa gre!");
		break;
	case 1:
		reload_level();
	case 0:
		break;
	}
	return 0;
}

int Program::display()
{
	//SDL_RenderSetViewport(renderer, &current_level->camera_position);
	//SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b));
	SDL_RenderClear(renderer);
	if (!in_menu)
	{
		SDL_RenderCopy(renderer, current_level->background, NULL, NULL);
		SDL_Rect life_dialog, src;
		life_dialog.y = LIFE_DIALOG_POSITION_Y;
		src.w = HEART_WIDTH;
		life_dialog.w = HEART_WIDTH;
		life_dialog.h = HEART_HEIGHT;
		for (int i = 0; i < MAX_LIFES_NUMBER; i++)
		{
			life_dialog.x = LIFE_DIALOG_POSITION_X - i * HEART_WIDTH;
			src.y = 0;
			if (i + current_level->mario.number_of_lifes >= MAX_LIFES_NUMBER)
				src.x = 0;
			else
				src.x = HEART_WIDTH;
			SDL_RenderCopy(renderer, heart_texture, &src, &life_dialog);

		}
		for (int i = 0; i < current_level->map_width; i++) {
			for (int j = 0; j < current_level->map_height; j++) {
				if (current_level->grid[i][j].get_tile() != AIR)
				{
					SDL_Rect temp = current_level->grid[i][j].position;
					temp.x -= current_level->camera_position.x;
					src = current_level->grid[i][j].position;
					//je¿eli blok znajduje siê pod innym to zostaje wyrenderowany bez œniegu z wierzchu
					if (current_level->grid[i][j].get_tile() == FINISH)
					{
						src.x = TILE_WIDTH * 2;
						src.y = 0;
					}
					else
					{
						if (j > 0 && current_level->grid[i][j - 1].get_tile() == BLOCK)
						{
							src.x = TILE_WIDTH;
							src.y = 0;
						}
						else
						{
							src.x = 0;
							src.y = 0;
						}
					}
					
					//temp.y += current_level->camera_position.y;
					SDL_RenderCopy(renderer, current_level->tile_texture, &src, &temp);
				}
			}
		}
		SDL_Rect temp = current_level->mario.position;
		temp.x -= current_level->camera_position.x;
		src = current_level->mario.position;
		src.y = 0;
		printf("%d", current_level->mario.is_jumping);
		if (current_level->mario.is_jumping)
			src.x = MARIO_WIDTH;
		else if (current_level->mario.is_walking)
		{
			if (current_level->mario.last_sprite_change > WALKING_DURATION)
			{
				current_level->mario.walking_sprite_number = ++current_level->mario.walking_sprite_number % NUMBER_OF_WALKING_FRAMES;
				current_level->mario.last_sprite_change = 0;
			}
			else
				current_level->mario.last_sprite_change += delta_time;
			src.x = MARIO_WIDTH * (2 + current_level->mario.walking_sprite_number);		
		}
		else
			src.x = 0;
		SDL_RenderCopyEx(renderer, current_level->mario.sprite, &src, &temp, NULL, NULL, current_level->mario.speed[0] < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
		sprintf(message_container, "Pozostalo: %d s", current_level->time_left / 1000);
		draw_string(renderer, TIME_TEXT_POSITION_X, TIME_TEXT_POSITION_Y, message_container, charset);
		sprintf(message_container, "temp.x = %d", temp.x);
		draw_string(renderer, TIME_TEXT_POSITION_X, TIME_TEXT_POSITION_Y + 100, message_container, charset);
	}
	else
	{
		draw_string(renderer, MENU_MESSAGE_POSITION_X, MENU_MESSAGE_POSITION_Y, message_container, charset);

	}
	SDL_RenderPresent(renderer);
	return 0;
}

SDL_Texture* Program::load_texture(char *path_to_texture)
{
	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded_bmp = SDL_LoadBMP(path_to_texture);
	if (loaded_bmp == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", *path_to_texture, SDL_GetError());
	}
	new_texture = SDL_CreateTextureFromSurface(renderer, loaded_bmp);
	SDL_FreeSurface(loaded_bmp);
	return new_texture;
}

int Program::reload_level()
{
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "levels/level%d.lvl", current_level->level_count);
	temp_level = load_level(buffer);
	temp_level->mario.number_of_lifes = current_level->mario.number_of_lifes;
	temp_level->level_count = current_level->level_count;
	delete current_level;
	current_level = temp_level;
	return 0;
}

int Program::start_game()
{
	delete current_level;
	current_level = load_level("levels/level1.lvl");
	in_menu = false;
	return 0;
}

int Program::save_temp_level()
{
	if (temp_level != NULL)
		delete temp_level;
	temp_level = new Level("NONE");
	*temp_level = *current_level;
	return 0;
}

int Program::load_temp_level()
{
	if (temp_level != NULL)
	{
		delete current_level;
		*current_level = *temp_level;
	}
	return 0;
}

Level* Program::load_level(char level_to_load[])
{
	Level* loaded_level = new Level(level_to_load);
	loaded_level->tile_texture = load_texture("textures/tile.bmp");
	loaded_level->mario.sprite = load_texture("textures/mario.bmp");
	loaded_level->background = load_texture("textures/background.bmp");
	return loaded_level;
}

bool Program::set_fullscreen()
{
	printf("now - last_fullscreen = %d \n", (now - last_fullscreen));
	if (!fullscreen && (now - last_fullscreen) > 100)
	{
		SDL_DisplayMode display_mode;
		SDL_GetCurrentDisplayMode(0, &display_mode);
		SDL_RenderSetScale(renderer, display_mode.w / SCREEN_WIDTH, display_mode.h / SCREEN_HEIGHT);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullscreen = true;
		last_fullscreen = now;
		screen_width = display_mode.w;
		screen_height = display_mode.h;
		return true;
	}
	else if ((now - last_fullscreen) > 100)
	{
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_RenderSetScale(renderer, 1, 1);
		SDL_SetWindowFullscreen(window, 0);
		fullscreen = false;
		last_fullscreen = now;
		screen_width = SCREEN_WIDTH;
		screen_height = SCREEN_HEIGHT;
		return true;
	}
	//current_level->update_camera(screen_width, screen_height);
	return false;
}

void Program::draw_string(SDL_Renderer *renderer, int x, int y, const char *text,
	SDL_Texture *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_RenderCopy(renderer, charset, &s, &d);
		x += 8;
		text++;
	};
};

