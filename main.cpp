#include <SFML/Graphics.hpp>
#include "GameManager.h"

std::vector<Build> GameManager::buildingQueue; // building queue
std::vector<Upgrade> GameManager::upgradesQueue; // upgrade queue
sf::Texture GameManager::redMinionTexture, GameManager::blueMinionTexture, GameManager::fireballTexture, GameManager::iceballTexture, GameManager::healingballTexture, GameManager::redTowerTexture, GameManager::blueTowerTexture, GameManager::redTowerTexture2, GameManager::blueTowerTexture2, GameManager::startButtonSelectedTexture, GameManager::startButtonUnSelectedTexture, GameManager::topMenuTexture, GameManager::botMenuTexture, GameManager::botMenuTrimTexture, GameManager::botMenuTrimTexture2, GameManager::gameMapTexture, GameManager::redTowerRotatedTexture, GameManager::blueTowerRotatedTexture, GameManager::redTowerRotatedTexture2, GameManager::blueTowerRotatedTexture2, GameManager::redTowerTexture3, GameManager::blueTowerTexture3, GameManager::redTowerRotatedTexture3, GameManager::blueTowerRotatedTexture3; // texture
sf::SoundBuffer GameManager::coinDropBuffer, GameManager::hammerBuffer, GameManager::destroyBuffer;
sf::Sound GameManager::coinDropSound, GameManager::hammerSound, GameManager::destroySound;
sf::Font GameManager::ringm, GameManager::blkchcry, GameManager::beyondWonderland; // fonts
bool GameManager::areTimersViable;

static const sf::Vector2i originalSize(780, 565);

int main()
{
	// window
	sf::RenderWindow window(sf::VideoMode((int) (780 * 1.5), (int) (565 * 1.5)), "Hexagon Fort", (sf::Style::Close | sf::Style::Titlebar));
	window.setFramerateLimit(30);
	window.setMouseCursorVisible(false);

	// cursor
	sf::Texture cursorTexture;
	if (!cursorTexture.loadFromFile("images/mouse_pointer.png")) {
		return -1; // failed to load mouse pointer image
	}
	sf::Sprite cursor(cursorTexture);
	cursor.setScale(0.175f, 0.175f);

	// Setup game manager and map
	if (!GameManager::loadNecessaryFiles()) {
		return -1; // failed to load files so terminate application
	}
	GameManager manager(window);
	int gameState = 0; // 0 = start screen, 1 = in-game, 2 = pause, 3 = gameover
	int winner = -1; // -1 = no winner, 0 = red team won, 1 = blue team won
	sf::Sprite gameMap(GameManager::gameMapTexture);
	gameMap.setScale(1.5f, 1.5f);

	// background music
	sf::Music backgroundMusic;
	if (!backgroundMusic.openFromFile("sounds/Failing_Defense.ogg")) {
		return -1; // could not find the Failing_Defense music
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(25);
	backgroundMusic.play();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (window.isOpen()) {
			if (gameState == 0) {
				if (manager.isOnStartButton((sf::Vector2f) sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					//manager = GameManager(window); do not need to do this as we already initialized GameManager object
					gameState = 1; // go to in game
					winner = -1; // no winner
					GameManager::areTimersViable = true;
				}
			}
			else if (gameState == 1) {
				manager.runGameManager(window, 20, 0.75f);
				if (manager.getBlueTeamHealth() <= 0) {
					gameState = 3; // go to game over!
					winner = 0; // red won
					GameManager::areTimersViable = false;
				}
				else if (manager.getRedTeamHealth() <= 0) {
					gameState = 3; // go to game over!
					winner = 1; // blue won
					GameManager::areTimersViable = false;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					gameState = 2; // go to pause game
					backgroundMusic.pause();
					GameManager::areTimersViable = false;
				}
			}
			else if (gameState == 2) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect(window.getPosition(), (sf::Vector2i) window.getSize()).contains(sf::Mouse::getPosition(window)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					gameState = 1; // go to in game (continue game)
					backgroundMusic.play();
					GameManager::areTimersViable = true;
				}
			}
			else if (gameState == 3) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect(window.getPosition(), (sf::Vector2i) window.getSize()).contains(sf::Mouse::getPosition(window)) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					manager = GameManager(window);
					gameState = 1; // go to in game (restart game)
					winner = -1; // no winner
					GameManager::areTimersViable = true;
				}
			}
		}

		window.clear();

		window.draw(gameMap); // background map texture
		manager.draw(window, gameState, winner);

		if (window.isOpen()) {
			cursor.setPosition((sf::Vector2f)sf::Mouse::getPosition(window));
			window.draw(cursor);
		}

		window.display();
	}

	return 0;
}
