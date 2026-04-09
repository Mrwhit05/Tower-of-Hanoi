#pragma once
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "GameLogic.h"
class Renderer
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	int width;
	int height;
public:
	Renderer(int w, int h);
	~Renderer();

	SDL_Renderer* getSDLRenderer();

	void clear();
	void present();

	void drawMenu(SDL_Rect& playButton, bool hovered);
	void drawGame(const GameLogic& game);
	void drawWin(const GameLogic& game);
	
};

