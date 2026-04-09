#include <iostream>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <optional>
#include <SDL2/SDL.h>
#include "GameLogic.h"
#include "Renderer.h"

using namespace std;

bool isMouseOver(const SDL_Rect& rect) {
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	return (mouseX >= rect.x &&
		mouseX <= rect.x + rect.w &&
		mouseY >= rect.y &&
		mouseY <= rect.y + rect.h);
}

int getPillarFromMouse(float mouseX, float windowWidth) {
	float section = windowWidth / 3;
	return static_cast<int>(mouseX / section);
}

enum class Scene {
	Menu,
	Game,
	Win
};


void handleMenuEvents(const SDL_Event& event, const SDL_Rect& playButton, Scene& scene) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;

			bool inside =
				(mouseX >= playButton.x &&
				mouseX <= playButton.x + playButton.w &&
				mouseY >= playButton.y &&
				mouseY <= playButton.y + playButton.h);
			if (inside) {
				scene = Scene::Game;
			}
		}
	}
}

void handleGameEvents(const SDL_Event& event, GameLogic& game, Scene& scene) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;

			int pillar = clamp(getPillarFromMouse(mouseX, 800), 0, 2);

			if (!game.hasSelectedDisk()) {
				game.selectDisk(pillar);
			}
			else {
				game.placeDisk(pillar);
			}
		}
	}
}


void handleWinEvents(const SDL_Event& event, GameLogic& game, Scene& scene) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			game.reset();
			scene = Scene::Menu;
		}
	}
}

Renderer* renderer;
GameLogic* game;

Scene currentScene = Scene::Menu;
SDL_Rect playButton = { 280, 300, 240, 70 };

void gameLoop() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
		}

		if (currentScene == Scene::Menu) {
			handleMenuEvents(event, playButton, currentScene);
		}

		if (currentScene == Scene::Game) {
			handleGameEvents(event, *game, currentScene);
		}

		if (currentScene == Scene::Win) {
			handleWinEvents(event, *game, currentScene);
		}

		if (currentScene == Scene::Game and game->isSolved()) {
			currentScene = Scene::Win;
		}
	}

	renderer->clear();


	if (currentScene == Scene::Menu) {
		bool hovered = isMouseOver(playButton);
		renderer->drawMenu(playButton, hovered);
	}

	else if (currentScene == Scene::Game) {
		renderer->drawGame(*game);
	}

	else if (currentScene == Scene::Win) {
		renderer->drawWin(*game);
	}

	renderer->present();
}

bool running = true; 
int main() {
	renderer = new Renderer(800, 600);
	game = new GameLogic(3);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(gameLoop, 0, true);
#else
	while (running) {
		gameLoop();
	}
#endif
	return 0;
}
