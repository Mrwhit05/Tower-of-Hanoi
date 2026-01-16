#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>
#include "GameLogic.h"

using namespace std;

int getPillarFromMouse(float mouseX, float windowWidth) {
	float section = windowWidth / 3;
	return static_cast<int>(mouseX / section);
}

bool isMouseOver(const sf::RectangleShape& rect, sf::RenderWindow& window) {
	auto pixel = sf::Mouse::getPosition(window);
	sf::Vector2f world = window.mapPixelToCoords(pixel);
	return rect.getGlobalBounds().contains(world);
}

enum class Scene {
	Menu,
	Game,
	Win
};

void handleMenuEvents(const sf::Event& event, const sf::RectangleShape& playButton, sf::RenderWindow& window, Scene& scene) {
	if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			auto world = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			if (playButton.getGlobalBounds().contains(world)) {
				scene = Scene::Game;
			}
		}
	}
}

void handleGameEvents(const sf::Event& event, GameLogic& game, Scene& scene, sf::RenderWindow& window) {
	if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			float mouseX = static_cast<float>(mouse->position.x);
			int pillar = clamp(getPillarFromMouse(mouseX, window.getSize().x), 0, 2);

			if (!game.hasSelectedDisk()) {
				game.selectDisk(pillar);
			}
			else {
				game.placeDisk(pillar);
			}
		}
	}
}

void handleWinEvents(const sf::Event& event, GameLogic& game, Scene& scene) {
	if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			game.reset();
			scene = Scene::Menu;
		}
	}
}

void drawMenu(sf::RenderWindow& window, sf::RectangleShape& playButton, sf::Text& playText) {
	window.draw(playButton);
	window.draw(playText);
}

void drawGame(sf::RenderWindow& window, const GameLogic& game, sf::Text& moveText) {
	//Move count text
	moveText.setString("Moves: " + std::to_string(game.getMoveCount()));
	window.draw(moveText);

	const auto& pillars = game.getPillars();
	float diskHeight = 20;

	//Pillar builder
	for (int p = 0; p < 3; p++) {
		for (int i = 0; i < pillars[p].size(); i++) {
			int disk = pillars[p][i];

			float width = disk * 30;
			float x = (800.f / 3.f) * p + (800.f / 6.f) - width / 2.f;
			float y = 450.f - (i + 1) * diskHeight;

			sf::RectangleShape diskShape(sf::Vector2f(width, diskHeight - 2));
			diskShape.setFillColor(sf::Color(100, 200, 255));
			diskShape.setPosition({ x, y });
			window.draw(diskShape);
		}
	}

	//Floating block
	if (game.hasSelectedDisk()) {
		int disk = game.getSelectedDisk();

		float width = disk * 30.f;
		float x = sf::Mouse::getPosition(window).x - width / 2.f;
		float y = 100.f;

		sf::RectangleShape rect(sf::Vector2f(width, diskHeight - 2));
		rect.setFillColor(sf::Color(255, 200, 100));
		rect.setPosition({ x, y });

		window.draw(rect);
	}
}

void drawWin(sf::RenderWindow& window, GameLogic& game, sf::Text& winTitle, sf::Text& winMoves, sf::Text& winHint) {
	winMoves.setString("Solved in " + std::to_string(game.getMoveCount()) + " moves");

	sf::FloatRect mb = winMoves.getLocalBounds();
	winMoves.setOrigin({ mb.size.x / 2.f, mb.size.y / 2.f });
	winMoves.setPosition({ 400.f, 260.f });

	window.draw(winTitle);
	window.draw(winMoves);
	window.draw(winHint);
}

void updateMenu(sf::RectangleShape& playButton, sf::RenderWindow& window) {
	if (isMouseOver(playButton, window)) {
		playButton.setFillColor(sf::Color(160, 60, 60));
	}
	else {
		playButton.setFillColor(sf::Color(120, 40, 40));
	}
}

int main() {
	Scene currentScene = Scene::Menu;
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Tower of Hanoi");
	GameLogic game(3);

	//Load font
	sf::Font font;
	if (!font.openFromFile("assets/arial.ttf")) {
		cerr << "Failed to load font\n";
		return -1;
	}

	sf::RectangleShape playButton({ 240.f, 70.f });
	playButton.setPosition({ 300.f, 300.f });
	playButton.setFillColor(sf::Color(120, 40, 40));
	playButton.setOutlineThickness(3.f);
	playButton.setOutlineColor(sf::Color::White);
	playButton.setPosition({ 280.f, 300.f });

	sf::Text playText(font);
	playText.setString("PLAY");
	playText.setCharacterSize(36);
	playText.setFillColor(sf::Color::White);

	sf::FloatRect textBounds = playText.getLocalBounds();
	sf::Vector2f buttonPos = playButton.getPosition();
	sf::Vector2f buttonSize = playButton.getSize();

	playText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f});

	playText.setPosition({ buttonPos.x + buttonSize.x / 2.f, buttonPos.y + buttonSize.y / 2.f });

	sf::Text winTitle(font);
	winTitle.setString("YOU WIN!");
	winTitle.setCharacterSize(48);
	winTitle.setFillColor(sf::Color::Green);
	winTitle.setPosition({ 400.f, 180.f });
	auto titleBounds = winTitle.getLocalBounds();
	winTitle.setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y / 2.f });

	sf::Text winMoves(font);
	winMoves.setCharacterSize(24);
	winMoves.setFillColor(sf::Color::White);
	winMoves.setPosition({ 400.f, 260.f });

	sf::Text winHint(font);
	winHint.setString("Click anywhere to return to menu");
	winHint.setCharacterSize(18);
	winHint.setFillColor(sf::Color(200, 200, 200));
	winHint.setPosition({ 400.f, 320.f });
	auto hintBounds = winHint.getLocalBounds();
	winHint.setOrigin({ hintBounds.size.x / 2.f, hintBounds.size.y / 2.f });

	//Create move text
	sf::Text moveText(font);
	moveText.setString("Moves: 0");
	moveText.setCharacterSize(20);
	moveText.setFillColor(sf::Color::White);
	moveText.setPosition({ 10.f, 10.f });

	while (window.isOpen()) {
		while (optional<sf::Event> event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (currentScene == Scene::Menu) {
				handleMenuEvents(*event, playButton, window, currentScene);
			}

			if (currentScene == Scene::Game) {
				handleGameEvents(*event, game, currentScene, window);
			}

			if (currentScene == Scene::Win) {
				handleWinEvents(*event, game, currentScene);
			}

			if (currentScene == Scene::Game and game.isSolved()) {
				currentScene = Scene::Win;
			}
		}

		window.clear(sf::Color::Black);

		if (currentScene == Scene::Menu) {
			updateMenu(playButton, window);
			drawMenu(window, playButton, playText);
		}

		else if (currentScene == Scene::Game) {
			drawGame(window, game, moveText);
		}

		else if (currentScene == Scene::Win) {
			drawWin(window, game, winTitle, winMoves, winHint);
		}
		
		window.display();
	}
}