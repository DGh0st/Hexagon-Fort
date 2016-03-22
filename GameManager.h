#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FlatHexagonalGrid.h"
#include "Menus.h"
#include "Minion.h"
#include "Tower.h"
#include "RoundRobinTower.h"
#include "HealFountainTower.h"
#include "AI.h"

struct Build {
	int x = 0; // x position of the building/tower
	int y = 0; // y position of the building/tower
	bool team = true; // team the building belongs to
	std::string classType = "Tower";
};

struct Upgrade {
	Upgrades towerUpgrades; // upgrades for the tower
	Tower tower = Tower(); // tower to upgrade
	bool shouldRemove = false; // should destruct the tower or not
};

class GameManager {
public:
	// constructor that creates a GameManager Object
	// @param window: the window to create menus and draw into
	GameManager(sf::RenderWindow & window);
	// destructor
	~GameManager();
	// equal operator
	// @param other: the other GameManager object to copy from
	void operator=(const GameManager & other);
	// draw the GameObjects that are part of GameManager using sfml
	// @param window: the window to draw the menus into
	void draw(sf::RenderWindow & window, int gameState, int winner);
	// get the red team's base health
	int getRedTeamHealth();
	// get the blue team's base health
	int getBlueTeamHealth();
	// calls the necessary methods to run the game
	void runGameManager(sf::RenderWindow & window, int maxMinionsPerTeam, float minionSpawnSpeed);
	// get the start button bounds
	bool isOnStartButton(sf::Vector2f point);

	static std::vector<Build> buildingQueue; // queue to build towers
	static std::vector<Upgrade> upgradesQueue; // queue to upgrade towers
	static sf::Texture redMinionTexture, blueMinionTexture, fireballTexture, iceballTexture, healingballTexture, redTowerTexture, blueTowerTexture, redTowerTexture2, blueTowerTexture2, startButtonSelectedTexture, startButtonUnSelectedTexture, topMenuTexture, botMenuTexture, botMenuTrimTexture, botMenuTrimTexture2, gameMapTexture, redTowerRotatedTexture, blueTowerRotatedTexture, redTowerRotatedTexture2, blueTowerRotatedTexture2, redTowerTexture3, blueTowerTexture3, redTowerRotatedTexture3, blueTowerRotatedTexture3; // texture
	static sf::SoundBuffer coinDropBuffer, hammerBuffer, destroyBuffer;
	static sf::Sound coinDropSound, hammerSound, destroySound;
	static sf::Font ringm, blkchcry, beyondWonderland; // fonts
	static bool areTimersViable; // true = viable, false = not viable

	// load files that are necessary like fonts and textures
	static bool loadNecessaryFiles();
private:
	struct Team {
		int totalHealth = 1000;
		int minionDamage = 10;
		int minionHealth = 100;
		float minionSpeed = 0.2f;
		int towerDamage = 20;
		int towerHealth = 100;
		float towerAttackSpeed = 400.0f;
		float towerRange = 100.0f;
		int bulletsToSpray = 1;
		bool side = true;
		int gold = 500;
		sf::Color color;
	};
	bool user; // player is on red team if true and blue otherwise
	struct Team red; // red team
	struct Team blue; // blue team
	FlatHexagonalGrid map; // hexagon grid for the game
	Menus bars; // menus for the game
	std::vector<Tower *> redTowers; // towers for the red team
	std::vector<Tower *> blueTowers; // towers for the blue team
	sf::Clock minionTimer; // timer to spawn minions
	std::vector<Minion> redMinions; // minions for the red team
	std::vector<Minion> blueMinions; // minions for the blue team
	AI ai; // ai for the computer
	sf::Text gameOverText, underGameOverText, clickAnyText , nameText, musicCredits;
	sf::RectangleShape backdrop, startGameButton;
	std::vector<sf::Text> effects; // text effects

	// build a tower at hex and returns true if successfully does so
	// @param hex: hexagon to build tower on top of
	// @param teamRed: true if build tower for team red, false if build tower for team blue
	bool buildTower(Hexagon hex, bool teamRed, std::string towerType);
	// generates minions if needed
	// @param seconds: generates a minions every seconds
	// @param maxTeamSize: max amount of minions a team can have at a specific time
	void generateMinionsIfNeeded(float seconds, int maxTeamSize, sf::RenderWindow & window);
	// movement management for the minion
	void minionMovement();
	// tower attack management
	void towerAttack(sf::RenderWindow & window);
	// go through the buildingQueue
	void manageBuildingQueue();
	// go through the upgradesQueue
	void manageUpgradesQueue();
	// setup the screen displayed for GameOver Screen
	// @param window: window to display in
	void setupStartScreen(sf::RenderWindow & window);
	// setup the screen displayed for GameOver Screen
	// @param window: the size of the backdrop is gotten from window
	void setupPauseOrGameOverScreen(sf::RenderWindow & window, std::string header, std::string underHeader, std::string description);
	// setup availableUpgrades
	void setupAvailableUpgrades();
	// find the insert point for tower so that graphics look nice
	int findInsertLocation(std::vector<Tower *> teamTowers, Hexagon hex);
};

#endif