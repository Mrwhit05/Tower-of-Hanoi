#include "Renderer.h"
#include <SDL_ttf.h>
#include <string>

void drawText(SDL_Renderer* renderer,
	TTF_Font* font,
	const std::string& text,
	int x, int y,
	SDL_Color color) {

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = surface->w;
	dst.h = surface->h;

	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, nullptr, &dst);
	SDL_DestroyTexture(texture);
}

Renderer::Renderer(int w, int h) : width(w), height(h) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
	}

	if (TTF_Init() == -1) {
		SDL_Log("TTF_Init Error: %s", TTF_GetError());
	}

	font = TTF_OpenFont("assets/ARIAL.TTF", 24);
	if (!font) {
		SDL_Log("Failed to load font: %s", TTF_GetError());
	}

	window = SDL_CreateWindow(
		"Tower of Hanoi",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

SDL_Renderer* Renderer::getSDLRenderer() {
	return renderer;
}

void Renderer::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	SDL_RenderPresent(renderer);
}

void Renderer::drawGame(const GameLogic& game) {
	//Move count text
	SDL_Color white = { 255, 255, 255, 255 };
	drawText(renderer, font, "Moves: " + std::to_string(game.getMoveCount()), 10, 10, white);

	const auto& pillars = game.getPillars();
	float diskHeight = 20;

	//Pillar builder
	for (int p = 0; p < 3; p++) {
		for (int i = 0; i < pillars[p].size(); i++) {
			int disk = pillars[p][i];

			float width = disk * 30;
			float x = (800.f / 3.f) * p + (800.f / 6.f) - width / 2.f;
			float y = 450.f - (i + 1) * diskHeight;

			SDL_Rect rect;
			rect.x = (int)x;
			rect.y = (int)y;
			rect.w = (int)width;
			rect.h = (int)(diskHeight - 2);

			SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	//Floating block
	if (game.hasSelectedDisk()) {
		int disk = game.getSelectedDisk();

		float width = disk * 30.f;

		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		SDL_Rect rect;
		rect.x = mouseX - width / 2;
		rect.y = 100;
		rect.w = (int)width;
		rect.h = (int)diskHeight;

		SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Renderer::drawMenu(SDL_Rect& playButton, bool hovered) {
	if (hovered) {
		SDL_SetRenderDrawColor(renderer, 160, 60, 60, 255);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 120, 40, 40, 255);
	}
	SDL_RenderFillRect(renderer, &playButton);

	SDL_Color white = { 255, 255, 255, 255 };

	drawText(renderer, font, "Tower of Hanoi", playButton.x + playButton.w / 2 - 20,
		playButton.y + playButton.h / 2 - 150,
		white);

	drawText(renderer, font, "PLAY", playButton.x + playButton.w / 2 - 40,
		playButton.y + playButton.h / 2 - 10,
		white);
}

void Renderer::drawWin(const GameLogic& game) {
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color green = { 0, 255, 0, 255 };

	//Title
	drawText(renderer, font, "YOU WIN!", 320, 180, green);

	//Moves text
	string moves = "Solved in " + to_string(game.getMoveCount()) + " moves";

	if (!font) {
		return;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(font, moves.c_str(), white);

	if (!surface) {
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	int textW = surface->w;
	int textH = surface->h;

	SDL_FreeSurface(surface);

	SDL_Rect dst;
	dst.x = 400 - textW / 2;
	dst.y = 260 - textH / 2;
	dst.w = textW;
	dst.h = textH;

	SDL_RenderCopy(renderer, texture, nullptr, &dst);
	SDL_DestroyTexture(texture);

	// Hint
	drawText(renderer, font,
		"Click anywhere to return",
		280, 320, white);
}