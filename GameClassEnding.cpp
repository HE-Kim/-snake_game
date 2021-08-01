
#include "GameClassEnding.h"
#include "GameClassPhase.h"
#include "SDL_image.h"
#include <iostream>
#include "SDL_ttf.h"
#include <atlstr.h>
#include <string>

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern bool g_flag_running;
extern int g_current_game_phase;
extern int eat;

int win;
bool phase_cnt;

TTF_Font *end_font_;
//SDL_Rect time_rect_;
//SDL_Texture *time_;


SDL_Color white_;

SDL_Rect win_end_rect_;
SDL_Texture *win_end;

Ending::Ending()
{
	phase_cnt = false;
	// For Texture
	win = 0;

	SDL_Surface* temp_surface = IMG_Load("../Resources/end.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
	destination_rectangle_.x = 0;
	destination_rectangle_.y = 0;
	source_rectangle_.x = 0;
	source_rectangle_.y = 0;
	destination_rectangle_.w = source_rectangle_.w;
	destination_rectangle_.h = source_rectangle_.h;

	//text
	end_font_ = TTF_OpenFont("../Resources/Chlorinp.ttf", 100);

	white_ = { 255, 255,255, 0 };

	SDL_Surface *tmp2_surface = TTF_RenderText_Blended(end_font_, std::to_string((long long)win).c_str(), white_); // 요딴식으로 하면된다.
	win_end_rect_.x = 0;
	win_end_rect_.y = 0;
	win_end_rect_.w = tmp2_surface->w;
	win_end_rect_.h = tmp2_surface->h;

	win_end = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);

}

Ending::~Ending()
{
	SDL_DestroyTexture(texture_);
}

void win_end_update()
{
	SDL_Surface *tmp2_surface = TTF_RenderText_Blended(end_font_, std::to_string((long long)win).c_str(), white_); // 요딴식으로 하면된다.
	win_end_rect_.x = 0;
	win_end_rect_.y = 0;
	win_end_rect_.w = tmp2_surface->w;
	win_end_rect_.h = tmp2_surface->h;

	win_end = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);
}

void Ending::Update()
{
	if (win < eat)
		win = eat;
	win_end_update();
	phase_cnt = true;
}


void Ending::Render()
{

	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);

	{
		SDL_Rect r;
		r.x = 550;
		r.y = 370;
		r.w = 50;
		r.h = 60;
		SDL_RenderCopy(g_renderer, win_end, 0, &r);
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Ending::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				g_current_game_phase = PHASE_STAGE1;
			}
			break;

		default:
			break;
		}
	}
}
