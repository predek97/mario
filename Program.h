#pragma once
#include "config.h"
#include "Player.h"
#include "Level.h"

class Program
{
public:
	Program();
	~Program();
	int handle_events();
	int display();
	bool is_ending = false;
private:
	SDL_Texture* load_texture(char *path_to_bmp);
	int reload_level();
	int start_game();
	int save_temp_level();
	int load_temp_level();
	Level* load_level(char level_to_load[]);
	bool set_fullscreen();
	void draw_string(SDL_Renderer * renderer, int x, int y, const char * text, SDL_Texture * charset);
	SDL_Texture* charset = NULL;
	SDL_Texture* heart_texture = NULL;
	SDL_Renderer* renderer = NULL;
	Level* current_level = NULL;
	Level* temp_level = NULL;
	SDL_Window* window = NULL;
	SDL_Event e;
	Uint32 now;
	Uint32 last;
	Uint32 last_fullscreen = 0;
	int screen_width = 0;
	int screen_height = 0;
	double delta_time;
	double fps_timer;
	long frames;
	bool fullscreen = false;
	bool in_menu = false;
	bool developer_mode = false;
	char message_container[BUFFER_SIZE];
};

