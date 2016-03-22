#ifndef MENUS_H
#define MENUS_H

#include <SFML/Graphics.hpp>
#include "FlatHexagonalGrid.h"
#include "Tower.h"

class Menus {
public:
	// constructor that creates all the menus
	// @param grid: grid needs to be clicked in order to get bottom menu to display
	// @param window: RenderWindow in which menus are displayed
	// @param posDirection: the team that the menus should be displayed for
	Menus(FlatHexagonalGrid * grid = NULL, sf::RenderWindow * window = NULL, bool posDirection = true);
	// destructor
	~Menus();
	// equal operator
	// @param other: the other Menus object to copy from
	void operator=(const Menus & other);
	// draw the menus using sfml
	// @param window: the window to draw the menus into
	// @param redHealth: health of red team's base
	// @param blueHealth: health of blue team's base
	void draw(sf::RenderWindow & window);
	// manage menus based on state 
	void manageMenus(sf::RenderWindow & window, std::vector<Tower *> teamTowers, int redHealth, int blueHealth, int gold, int maxHealth, sf::Color teamColor);
	// update the tower info text
	void updateTowerInfo(std::vector<Tower *> teamTowers, int k);
private:
	bool posDirection; // team that the menus should be displayed for
	std::vector<sf::RectangleShape> topMenu; // top menu that is always visible

	std::vector<sf::RectangleShape>	buildTowersMenu; // menu for building various towers
	std::vector<sf::Text> buildsText; // text to dispaly in the building menu
	std::vector<sf::ConvexShape> towers; // towers to display in the menu
	std::vector<float> towerRadiuses; // radius for the towers (required for displaying range when tower selected for building)
	sf::CircleShape towerRangeDisplay; // tower range display when a tower is selected to build

	std::vector<sf::RectangleShape> upgradeTowerMenu; // menu for upgrading tower
	std::vector<sf::Text> upgradesText; // text to display in the upgrade menu
	sf::Text info; // text to display information about the tower

	Hexagon selectedTower = Hexagon(); //selected tower to upgrade
	sf::RectangleShape botBackground;
	int buildingTowerIndex; // -1 if not building anything, otherwise holds the index of the tower in buildTowersMenu
	int botMenuState; // 0 for buildTowersMenu, 1 for upgradeTowerMenu
	sf::Clock botButtonDelay; // the clock to add a delay to menu button click
	FlatHexagonalGrid *hexGrid; // the hexGrid required for update menus
	int screenWidth, screenHeight; // width and height of the screen

	sf::Text gold, time; // the hexagon's properties for test purposes
	sf::Clock gameTime; // timer to display for the game
	sf::Time gamePlaySecs; // seconds of in-game time
	sf::RectangleShape redHealthBar, blueHealthBar; // health bars for each team
	sf::Text redHealthText, blueHealthText; // health text for each team (displayed on hover)
	sf::Text nameText; // the title of the game displayed on the top bar

	// initializes the top menu with a background
	void initTopMenu();
	// initializes the bot menu with a background
	void initBotMenu();
	// change displayed bot menu if needed
	void updateMenusIfNeeded(sf::RenderWindow & window, std::vector<Tower *> teamTowers, int redHealth, int blueHealth, int gold, sf::Color teamColor);
	// make build towers menu in bot menu
	void buildTowersMenuForDisplay(int maxTowers);
	// make upgrade towers menu in bot menu
	void buildUpgradeMenuForDisplay(int upgradeTypes, Hexagon & hexagon, std::vector<Tower *> teamTowers);
	// send build request to GameManager
	void sendBuildRequest(sf::RenderWindow & window);
	// send upgrade request to GameManager
	void sendUpgradeRequest(int i, std::vector<Tower *> teamTowers, int gold);
};

#endif