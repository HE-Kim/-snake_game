#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <atlstr.h>
#include <string>
#include <iostream>

class Stage1
{
public:
	Stage1();
	~Stage1();

	void HandleEvents();
	void Update();
	void Render();

	void check_can();
	void add_front();
	void del_tail();
	void Node_init();
	void check_hit();
	void can_(); //canÀÇ À§Ä¡
private:
	SDL_Texture * bg_texture_; //  SDL_Texture variable
	SDL_Rect bg_source_rectangle_;
	SDL_Rect bg_destination_rectangle_;
	SDL_Surface* bg_surface;

	SDL_Texture * can_texture_; //  SDL_Texture variable
	SDL_Rect can_source_rectangle_;
	SDL_Rect can_destination_rectangle_;

	SDL_Texture * head_texture_; // the SDL_Texture 
	SDL_Rect head_source_rectangle_; // the rectangle for source image
	SDL_Rect head_destination_rectangle_; // for destination
	SDL_Surface* head_temp_surface;


	SDL_Texture * tail_texture_; // the SDL_Texture 
	SDL_Rect tail_source_rectangle_; // the rectangle for source image
	SDL_Rect tail_destination_rectangle_; // for destination
	SDL_Surface* tail_temp_surface;





	bool character_go_left_;
	bool character_go_right_;
	bool character_go_up_;
	bool character_go_down_;

	double character_x_;
	double character_y_;

	double can_x_;
	double can_y_;
};

class Node
{
public:
	//int data;
	SDL_Rect data;
	Node *next;
	Node *p;
};

