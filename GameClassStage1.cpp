#include "GameClassStage1.h"
#include "GameClassPhase.h"
#include "SDL_image.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <atlstr.h>
#include <string>



extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern bool g_flag_running;

bool can_eaten;
int cnt;
int eat; //참치 먹은량
extern int win; // 이전것과 비교
extern bool phase_cnt;



TTF_Font *game_font_;
//SDL_Rect time_rect_;
//SDL_Texture *time_;


SDL_Color white;
SDL_Rect text_good_rect_;
SDL_Texture *text_good_;

SDL_Rect win_rect_;
SDL_Texture *win_;

Node *head = nullptr;

Stage1::Stage1()
{	
	// For Character
	character_go_left_ = false;
	character_go_right_ = false;
	character_go_up_ = false;
	character_go_down_ = false;


	character_x_ = 300;
	character_y_ = 300;
	can_();

	bg_surface = IMG_Load("../Resources/bg_2.png");

	bg_destination_rectangle_.x = bg_source_rectangle_.x = 0;
	bg_destination_rectangle_.y = bg_source_rectangle_.y = 0;
	bg_destination_rectangle_.w = bg_source_rectangle_.w = bg_surface->w;
	bg_destination_rectangle_.h = bg_source_rectangle_.h = bg_surface->h;

	bg_texture_ = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);


	SDL_Surface* can_surface = IMG_Load("../Resources/tuna_.png");
	can_texture_ = SDL_CreateTextureFromSurface(g_renderer, can_surface);
	SDL_FreeSurface(can_surface);

	SDL_QueryTexture(can_texture_, NULL, NULL, &can_source_rectangle_.w, &can_source_rectangle_.h);
	can_source_rectangle_.x = 0;
	can_source_rectangle_.y = 0;

	can_destination_rectangle_.x = (int)can_x_;
	can_destination_rectangle_.y = (int)can_y_;
	can_destination_rectangle_.w = 30;
	can_destination_rectangle_.h = 30;

	

	// For Texture

	head_temp_surface = IMG_Load("../Resources/cat_.png");
	head_texture_ = SDL_CreateTextureFromSurface(g_renderer, head_temp_surface);
	SDL_FreeSurface(head_temp_surface);

	SDL_QueryTexture(head_texture_, NULL, NULL, &head_source_rectangle_.w, &head_source_rectangle_.h);
	head_source_rectangle_.x = 0;
	head_source_rectangle_.y = 0;
	head_destination_rectangle_.x = (int)character_x_;
	head_destination_rectangle_.y = (int)character_y_;
	head_destination_rectangle_.w = 30;
	head_destination_rectangle_.h = 30;

	tail_temp_surface = IMG_Load("../Resources/cat_.png");
	tail_texture_ = SDL_CreateTextureFromSurface(g_renderer, tail_temp_surface);
	SDL_FreeSurface(tail_temp_surface);

	SDL_QueryTexture(tail_texture_, NULL, NULL, &tail_source_rectangle_.w, &tail_source_rectangle_.h);
	tail_source_rectangle_.x = 0;
	tail_source_rectangle_.y = 0;
	tail_destination_rectangle_.x = (int)character_x_;
	tail_destination_rectangle_.y = (int)character_y_;
	tail_destination_rectangle_.w = 30;
	tail_destination_rectangle_.h = 30;

	game_font_ = TTF_OpenFont("../Resources/Chlorinp.ttf", 100);

	white = { 255, 255,255, 0 };

	SDL_Surface *tmp_surface = TTF_RenderText_Blended(game_font_, std::to_string((long long)eat).c_str(), white); // 요딴식으로 하면된다.
	text_good_rect_.x = 0;
	text_good_rect_.y = 0;
	text_good_rect_.w = tmp_surface->w;
	text_good_rect_.h = tmp_surface->h;

	text_good_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);

	SDL_Surface *tmp2_surface = TTF_RenderText_Blended(game_font_, std::to_string((long long)win).c_str(), white); // 요딴식으로 하면된다.
	win_rect_.x = 0;
	win_rect_.y = 0;
	win_rect_.w = tmp2_surface->w;
	win_rect_.h = tmp2_surface->h;

	win_ = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);

	Node_init();

	can_eaten = false;

	srand((unsigned int)time(0));
	

}

Stage1::~Stage1()
{
	SDL_DestroyTexture(head_texture_);
}
void update_win()
{

	SDL_Surface *tmp2_surface = TTF_RenderText_Blended(game_font_, std::to_string((long long)win).c_str(), white); // 요딴식으로 하면된다.
	win_rect_.x = 0;
	win_rect_.y = 0;
	win_rect_.w = tmp2_surface->w;
	win_rect_.h = tmp2_surface->h;

	win_ = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
	SDL_FreeSurface(tmp2_surface);

}
void Stage1::Node_init()
{
	Node *n = new Node;
	n->next = head;
	head = n;
	n->data.x = head_destination_rectangle_.x;
	n->data.y = head_destination_rectangle_.y;

	if (phase_cnt == true)
	{
		while (n->next != nullptr)
			del_tail();
	}
	cnt = 0;
	eat = 0;
	character_x_ = 300;
	character_y_ = 300;
	can_();
	add_front();
	update_win();
}

void update_eat_cnt()
{
	SDL_Surface *tmp_surface = TTF_RenderText_Blended(game_font_, std::to_string((long long)eat).c_str(), white); // 요딴식으로 하면된다.
	text_good_rect_.x = 0;
	text_good_rect_.y = 0;
	text_good_rect_.w = tmp_surface->w;
	text_good_rect_.h = tmp_surface->h;

	text_good_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
}
void Stage1::can_()
{
	can_x_ = rand() % (680);
	can_y_ = rand() % (550);
	can_destination_rectangle_.x = (int)can_x_;
	can_destination_rectangle_.y = (int)can_y_;
}
void Stage1::check_can()
{
	can_();
	can_eaten = true;;
	eat++;
}

// 참치를 먹으면 tail del xx 참치를 안먹으면 tail del 해야함
// 처음에 왜 추가가 안되는지 확인해보기

void Stage1::add_front()
{
	Node* n = new Node;

	head->p = n;
	n->p = nullptr;
	n->next = head;
	head = n;

	n->data.x = character_x_; 
	n->data.y = character_y_;
	
	head_destination_rectangle_.x = character_x_;
	head_destination_rectangle_.y = character_y_;

	if (can_eaten == true)
	{
		can_eaten = false;
		return;
	}
		
	else
	{
		if (cnt>1)
			del_tail();
	}
	
}

void Stage1::del_tail()
{
	if (head == nullptr) return;

	// 데이터가 1개일 때.
	if (head->next == nullptr)
	{
		delete head;
		head = nullptr;
	}

	// 데이터가 2개 이상일 때/
	else
	{
		Node *n = head;
		while (n->next->next != nullptr)
		{
			n = n->next;
		}

		delete n->next;
		n->next = nullptr;
		n->data.x = character_x_; 
		n->data.y = character_y_;

		tail_destination_rectangle_.x = character_x_;
		tail_destination_rectangle_.y = character_y_;
	}
	


}

void Stage1::check_hit()
{
	Node *n = head;
	head->p = n;

	while (n->next->next != nullptr)
	{
		n = n->next;
		if (character_x_ == n->data.x && character_y_ == n->data.y)
		{
			g_current_game_phase = PHASE_ENDING;
		}
	}
	
}

void Stage1::Update()
{


	if (phase_cnt == true)
	{
		
		Node_init();
		phase_cnt = false;
	}
	cnt++;
	Node* n = new Node;
	n = head;
	if (character_x_ >= can_x_ - 20 &&
		character_x_ <= can_x_ + 20 &&
		character_y_ >= can_y_ - 20 &&
		character_y_ <= can_y_ + 20)
		check_can();



	//if(cnt>1)
	if (character_go_left_)
	{
		character_x_ -= 30.;
		head_destination_rectangle_.x = (int)character_x_;
		add_front();
		
	}

	if (character_go_right_)
	{
		character_x_ += 30.;
		head_destination_rectangle_.x = (int)character_x_;
		add_front();
	}
	if (character_go_up_)
	{
		character_y_ -= 30.;
		head_destination_rectangle_.y = (int)character_y_;
		add_front();
	}
	if (character_go_down_)
	{
		character_y_ += 30.;
		head_destination_rectangle_.y = (int)character_y_;
		add_front();
	}

	
	//끝에 닿으면 끝
	if (head_destination_rectangle_.y > bg_destination_rectangle_.h - 30)
	{
		character_go_down_ = false;
		g_current_game_phase = PHASE_ENDING;
	}
	else if (head_destination_rectangle_.y < bg_destination_rectangle_.y)
	{
		character_go_up_ = false;
		g_current_game_phase = PHASE_ENDING;
		
	}
	if (head_destination_rectangle_.x > 710)
	{
		head_destination_rectangle_.x -= 30;
		character_go_right_ = false;
		g_current_game_phase = PHASE_ENDING;
		
	} 
	else if (head_destination_rectangle_.x < bg_source_rectangle_.x)
	{	
	character_go_left_ = false;
		g_current_game_phase = PHASE_ENDING; 
		
	}
	

	if(eat>4)
		check_hit();
	
}


void Stage1::Render()
{
	Node* n = new Node;
	n = head;

	head_destination_rectangle_.x = n->data.x;
	head_destination_rectangle_.y = n->data.y;


	SDL_RenderCopy(g_renderer, bg_texture_, &bg_source_rectangle_, &bg_destination_rectangle_);
	SDL_RenderCopy(g_renderer, head_texture_, &head_source_rectangle_, &head_destination_rectangle_);
	{
		while (n->next != NULL)
		{
		
			n = n->next;
			tail_destination_rectangle_.x = n->data.x;
			tail_destination_rectangle_.y = n->data.y;
			SDL_RenderCopy(g_renderer, tail_texture_, &tail_source_rectangle_, &tail_destination_rectangle_);

		}
	}
	SDL_RenderCopy(g_renderer, can_texture_, &can_source_rectangle_, &can_destination_rectangle_);


	{
		update_eat_cnt();
		SDL_Rect r;
		r.x = 750;
		r.y = 105;
		r.w = 15;
		r.h = 25;

		SDL_RenderCopy(g_renderer, text_good_, 0, &r);
	}
	{
		SDL_Rect r;
		r.x = 750;
		r.y = 290;
		r.w = 15;
		r.h = 25;
		SDL_RenderCopy(g_renderer, win_, 0, &r);
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Stage1::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;


		case SDL_KEYDOWN:
			// If the left arrow key is pressed. 
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				if (character_go_right_ == true)
					return;
				else
				{
					character_go_left_ = true;
					character_go_right_ = false;
					character_go_up_ = false;
					character_go_down_ = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				if (character_go_left_ == true)
					return;
				else
				{
					character_go_right_ = true;
					character_go_left_ = false;
					character_go_up_ = false;
					character_go_down_ = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				if (character_go_down_ == true)
					return;
				else
				{
					character_go_up_ = true;
					character_go_right_ = false;
					character_go_left_ = false;
					character_go_down_ = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				if (character_go_up_ == true)
					return;
				{
					character_go_down_ = true;
					character_go_right_ = false;
					character_go_up_ = false;
					character_go_left_ = false;
				}
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
			}
			break;


		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
			}
			break;

		default:
			break;
		}
	}
}
