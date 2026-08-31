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

enum class Scene {
	Menu,
	Game,
	Win
};

int selectedDiskCount = 3;
const int MIN_DISKS = 3;
const int MAX_DISKS = 8;

Renderer* renderer;
GameLogic* game;

Scene currentScene = Scene::Menu;
SDL_Rect playButton = { 280, 300, 240, 70 };
SDL_Rect minusButton = { 200, 370, 50, 50 };
SDL_Rect plusButton  = { 550, 370, 50, 50 };

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

void handleMenuEvents(const SDL_Event& event, const SDL_Rect& playButton,
                      const SDL_Rect& minusButton, const SDL_Rect& plusButton,
                      Scene& scene, int& selectedDiskCount) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;

			auto isInside = [&](const SDL_Rect& rect) {
				return (mouseX >= rect.x &&
				mouseX <= rect.x + rect.w &&
				mouseY >= rect.y &&
				mouseY <= rect.y + rect.h);
			};

			if (isInside(playButton)) {
				delete game;
				game = new GameLogic(selectedDiskCount);
				scene = Scene::Game;
			}
			else if (isInside(minusButton)) {
				selectedDiskCount = max(selectedDiskCount - 1, MIN_DISKS);
			}
			else if (isInside(plusButton)) {
				selectedDiskCount = min(selectedDiskCount + 1, MAX_DISKS);
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

void gameLoop() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
		}

		if (currentScene == Scene::Menu) {
			handleMenuEvents(event, playButton, minusButton, plusButton,
                             currentScene, selectedDiskCount);
		}

		if (currentScene == Scene::Game) {
			handleGameEvents(event, *game, currentScene);
		}

		if (currentScene == Scene::Win) {
			handleWinEvents(event, *game, currentScene);
		}

		if (currentScene == Scene::Game && game->isSolved()) {
			currentScene = Scene::Win;
		}
	}

	renderer->clear();


	if (currentScene == Scene::Menu) {
		bool hovered = isMouseOver(playButton);
		renderer->drawMenu(playButton, minusButton, plusButton,
                           hovered, selectedDiskCount, MIN_DISKS, MAX_DISKS);
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
	game = new GameLogic(selectedDiskCount);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(gameLoop, 0, true);
#else
	while (running) {
		gameLoop();
	}
#endif
	return 0;
}
