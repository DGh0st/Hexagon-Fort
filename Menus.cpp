#include "Menus.h"
#include "GameManager.h"

Menus::Menus(FlatHexagonalGrid * grid, sf::RenderWindow * window, bool posDirection):hexGrid(grid), posDirection(posDirection) {
	if (window != NULL) {
		screenWidth = window->getSize().x;
		screenHeight = window->getSize().y;
	}
	else {
		screenWidth = 780;
		screenHeight = 565;
	}
	buildingTowerIndex = -1;
	initTopMenu();
	initBotMenu();
}

void Menus::initTopMenu() {
	// top menu background
	sf::RectangleShape background(sf::Vector2f((float)screenWidth, screenHeight / 14.12500f));
	background.setPosition(sf::Vector2f(0.0f, 0.0f));
	background.setTexture(&GameManager::topMenuTexture);
	topMenu.push_back(background);
	// gold and game time text
	gold.setFont(GameManager::ringm);
	time.setFont(GameManager::blkchcry);
	gold.setColor(sf::Color(178, 178, 0));
	time.setColor(sf::Color(178, 178, 0));
	gold.setCharacterSize((int) (std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 40.1305379f));
	time.setCharacterSize((int) (std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 40.1305379f));
	// title text
	nameText = sf::Text(sf::String("Hexagon Fort"), GameManager::beyondWonderland, 32);
	nameText.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 40.1305379f)); // required if window resized
	nameText.setColor(sf::Color(178, 178, 0));
	nameText.setOrigin(nameText.getGlobalBounds().width, 0);
	// health bars for both teams
	redHealthBar = blueHealthBar = sf::RectangleShape(sf::Vector2f(screenWidth / 3.25f, screenHeight / 30.0f));
	redHealthBar.setFillColor(sf::Color(255, 0, 0, 100));
	blueHealthBar.setFillColor(sf::Color(0, 255, 255, 100));
	redHealthBar.setOrigin(redHealthBar.getGlobalBounds().width, 0);
	redHealthBar.setPosition(background.getGlobalBounds().left + background.getGlobalBounds().width / 2.0f, background.getGlobalBounds().top + background.getGlobalBounds().height / 3.0f);
	blueHealthBar.setPosition(background.getGlobalBounds().left + background.getGlobalBounds().width / 2.0f, background.getGlobalBounds().top + background.getGlobalBounds().height / 3.0f);
	// health texts for both teams
	redHealthText.setFont(GameManager::ringm);
	blueHealthText.setFont(GameManager::ringm);
	redHealthText.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 60.1305379f));
	blueHealthText.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 60.1305379f));
	// game time timer
	gamePlaySecs = sf::Time(sf::seconds(0));
	gameTime.restart();
}

void Menus::initBotMenu() {
	// bottom menu background
	botBackground = sf::RectangleShape(sf::Vector2f((float)screenWidth, screenHeight / 9.41666667f));
	botBackground.setPosition(sf::Vector2f(0.0f, (float)screenHeight - screenHeight / 9.41666667f));
	botBackground.setTexture(&GameManager::botMenuTexture);
	buildTowersMenuForDisplay(3);
}

Menus::~Menus() {
	// nothing to delete as of yet ...
}

void Menus::operator=(const Menus & other) {
	posDirection = other.posDirection;
	topMenu = other.topMenu;
	buildTowersMenu = other.buildTowersMenu;
	buildsText = other.buildsText;
	towers = other.towers;
	towerRadiuses = other.towerRadiuses;
	towerRangeDisplay = other.towerRangeDisplay;
	upgradeTowerMenu = other.upgradeTowerMenu;
	upgradesText = other.upgradesText;
	info = other.info;
	selectedTower = other.selectedTower;
	botBackground = other.botBackground;
	buildingTowerIndex = other.buildingTowerIndex;
	botMenuState = other.botMenuState;
	botButtonDelay = other.botButtonDelay;
	hexGrid = other.hexGrid;
	screenWidth = other.screenWidth;
	screenHeight = other.screenHeight;
	gold = other.gold;
	time = other.time;
	gameTime = other.gameTime;
	gamePlaySecs = other.gamePlaySecs;
	redHealthBar = other.redHealthBar;
	blueHealthBar = other.blueHealthBar;
	redHealthText = other.redHealthText;
	blueHealthText = other.blueHealthText;
	nameText = other.nameText;
	initTopMenu();
	initBotMenu();
}

void Menus::draw(sf::RenderWindow & window) {
	window.draw(redHealthBar);
	window.draw(blueHealthBar);
	window.draw(redHealthText);
	window.draw(blueHealthText);
	for (int i = 0; i < topMenu.size();i++) {
		window.draw(topMenu[i]);
	}
	window.draw(this->gold);
	window.draw(time);
	window.draw(nameText);

	window.draw(botBackground);
	if (botMenuState == 0) { // build tower menu
		for (int i = 0; i < buildTowersMenu.size(); i++) {
			window.draw(buildTowersMenu[i]);
			window.draw(towers[i]);
			window.draw(buildsText[i]);
			if (buildingTowerIndex != -1) {
				window.draw(towerRangeDisplay);
			}
		}
	}
	else if (botMenuState == 1) { // upgrade tower menu
		for (int i = 0; i < upgradeTowerMenu.size();i++) {
			window.draw(upgradeTowerMenu[i]);
			window.draw(upgradesText[i]);
			window.draw(info);
		}
	}
}

void Menus::manageMenus(sf::RenderWindow & window, std::vector<Tower *> teamTowers, int redHealth, int blueHealth, int gold, int maxHealth, sf::Color teamColor) {
	updateMenusIfNeeded(window, teamTowers, redHealth, blueHealth, gold, teamColor);
	if (buildingTowerIndex != -1 && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		towers[buildingTowerIndex].setPosition((sf::Vector2f) sf::Mouse::getPosition(window));
		towerRangeDisplay.setPosition((sf::Vector2f) sf::Mouse::getPosition(window));
	}
	redHealthBar.setScale(redHealth / (float) maxHealth, 1.0f);
	blueHealthBar.setScale(blueHealth / (float) maxHealth, 1.0f);
	redHealthText.setColor(sf::Color::Red);
	blueHealthText.setColor(sf::Color(sf::Color::Blue.r, sf::Color::Blue.g, sf::Color::Blue.b, sf::Color::Blue.a * 2 / 3));
	if (!redHealthBar.getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window)) && !blueHealthBar.getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window)) && !redHealthText.getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window)) && !blueHealthText.getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window))) {
		redHealthText.setColor(sf::Color(redHealthText.getColor().r, redHealthText.getColor().g, redHealthText.getColor().b, redHealthText.getColor().a / 4));
		blueHealthText.setColor(sf::Color(blueHealthText.getColor().r, blueHealthText.getColor().g, blueHealthText.getColor().b, blueHealthText.getColor().a / 4));
	}
}

void Menus::updateMenusIfNeeded(sf::RenderWindow & window, std::vector<Tower *> teamTowers, int redHealth, int blueHealth, int gold, sf::Color teamColor) {
	if (GameManager::areTimersViable && gameTime.getElapsedTime().asSeconds() >= 1.0f) {
		gameTime.restart();
		gamePlaySecs += sf::Time(sf::seconds(1)); // add one to game clock iff game is running (is in focus)
	}

	// player/team gold and gametime clock
	this->gold.setString("Gold:" + std::to_string(gold));
	if (this->gold.getString().getSize() > 11) {
		this->gold.setString(this->gold.getString().substring(0, 10) + "...");
	}
	char temp[] = "00:00:00";
	sprintf_s(temp, sizeof(temp), "%.2d:%.2d:%.2d", (int)gamePlaySecs.asSeconds() / 3600 % 24, (int)gamePlaySecs.asSeconds() / 60 % 60, (int)gamePlaySecs.asSeconds() % 60);
	time.setString(temp);
	this->gold.setPosition(topMenu[0].getGlobalBounds().left + topMenu[0].getGlobalBounds().width / 30.0f, topMenu[0].getGlobalBounds().top + topMenu[0].getGlobalBounds().height / 16);
	time.setPosition(topMenu[0].getGlobalBounds().left + topMenu[0].getGlobalBounds().width / 2 - time.getGlobalBounds().width / 2, topMenu[0].getGlobalBounds().top - topMenu[0].getGlobalBounds().height / 16);

	// Game title
	nameText.setPosition(topMenu[0].getGlobalBounds().left + topMenu[0].getGlobalBounds().width * 29.0f / 30.0f, topMenu[0].getGlobalBounds().top + topMenu[0].getGlobalBounds().height / 16);

	// health texts
	redHealthText.setString(std::to_string(redHealth));
	blueHealthText.setString(std::to_string(blueHealth));
	redHealthText.setOrigin(redHealthText.getGlobalBounds().width / 2, redHealthText.getGlobalBounds().height);
	blueHealthText.setOrigin(blueHealthText.getGlobalBounds().width / 2, blueHealthText.getGlobalBounds().height);
	redHealthText.setPosition(sf::Vector2f(redHealthBar.getGlobalBounds().left + redHealthBar.getLocalBounds().width / 2, redHealthBar.getGlobalBounds().top + redHealthBar.getLocalBounds().height / 2));
	blueHealthText.setPosition(sf::Vector2f(blueHealthBar.getGlobalBounds().left + blueHealthBar.getLocalBounds().width / 2, blueHealthBar.getGlobalBounds().top + blueHealthBar.getLocalBounds().height / 2));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && botButtonDelay.getElapsedTime().asSeconds() >= 0.2f && buildingTowerIndex == -1) {
		botButtonDelay.restart();
		Hexagon hexagon = hexGrid->getHexagon(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		if (botBackground.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			if (botMenuState == 0) { // build tower menu
				for (int i = 0; i < buildTowersMenu.size(); i++) {
					if (buildTowersMenu[i].getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window))) {
						buildingTowerIndex = i;
						// tower range display
						towerRangeDisplay = sf::CircleShape(towerRadiuses[i], 100);
						towerRangeDisplay.setFillColor(sf::Color(teamColor.r / 3, teamColor.g / 3, teamColor.b / 3, teamColor.a / 8));
						towerRangeDisplay.setOutlineColor(teamColor);
						towerRangeDisplay.setOutlineThickness(1.0f);
						towerRangeDisplay.setOrigin(towerRadiuses[i], towerRadiuses[i]);
						towerRangeDisplay.setPosition((sf::Vector2f) sf::Mouse::getPosition());
					}
				}
			}
			else if (botMenuState == 1) { // upgrade tower menu
				for (int i = 0; i < upgradeTowerMenu.size(); i++) {
					if (upgradeTowerMenu[i].getGlobalBounds().contains((sf::Vector2f) sf::Mouse::getPosition(window))) {
						sendUpgradeRequest(i, teamTowers, gold);
					}
				}
			}
		}
		else if (hexagon.hex != NULL && hexagon.isOccupied && ((posDirection && hexagon.hex->getFillColor() == sf::Color::Red) || (!posDirection && hexagon.hex->getFillColor() == sf::Color::Cyan))) {
			upgradeTowerMenu.erase(upgradeTowerMenu.begin(), upgradeTowerMenu.end());
			upgradesText.erase(upgradesText.begin(), upgradesText.end());
			buildUpgradeMenuForDisplay(4, hexagon, teamTowers); // make upgrade menu based on selected hexagon (tower)
		}
		else {
			buildTowersMenu.erase(buildTowersMenu.begin(), buildTowersMenu.end());
			towers.erase(towers.begin(), towers.end());
			buildsText.erase(buildsText.begin(), buildsText.end());
			towerRadiuses.erase(towerRadiuses.begin(), towerRadiuses.end());
			buildTowersMenuForDisplay(3); // make build tower menu
		}
	}
	else if (buildingTowerIndex != -1 && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sendBuildRequest(window); // send request if released left click while holding a tower
	}
}

void Menus::buildTowersMenuForDisplay(int maxTowers) {
	for (int i = 0; i < maxTowers; i++) {
		// button background
		sf::RectangleShape buildingButton(sf::Vector2f((float) (botBackground.getGlobalBounds().width - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f * (maxTowers + 1)) / maxTowers, (float) (botBackground.getGlobalBounds().height - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 96.3132909f)));
		buildingButton.setTexture(rand() % 2 ? &GameManager::botMenuTrimTexture : &GameManager::botMenuTrimTexture2);
		buildingButton.setPosition(sf::Vector2f((float) (std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f + i * (buildingButton.getGlobalBounds().width + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)), (float) (botBackground.getGlobalBounds().top + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)));
		buildTowersMenu.push_back(buildingButton);
		// button tower
		sf::ConvexShape polygon(4);
		polygon.setPoint(0, sf::Vector2f(0, 0));
		polygon.setPoint(1, sf::Vector2f((float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) * 2 / 66.4229592f, 0));
		polygon.setPoint(2, sf::Vector2f((float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) * 2 / 66.4229592f, (float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) * 2 / 66.4229592f));
		polygon.setPoint(3, sf::Vector2f(0, (float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) * 2 / 66.4229592f));
		polygon.setOrigin((float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 66.4229592f, (float) std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2))  / 66.4229592f);
		polygon.setPosition(buildingButton.getGlobalBounds().left + buildingButton.getGlobalBounds().width / 2, buildingButton.getGlobalBounds().top + buildingButton.getGlobalBounds().height / 2);
		polygon.setRotation(90);
		// button text
		sf::Text temp;
		temp.setFont(GameManager::ringm);
		temp.setColor(sf::Color(255, 255, 0));
		temp.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 73.5073838f));
		float towerRange = (float)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 9.63132909f);
		if (i == 0) { // towerType = "Tower"
			if (posDirection) {
				polygon.setTexture(&GameManager::redTowerRotatedTexture);
			}
			else {
				polygon.setTexture(&GameManager::blueTowerRotatedTexture);
			}
			temp.setString("Tower (-450)");
			towerRadiuses.push_back(towerRange);
		}
		else if(i == 1) { // towerType = "RoundRobinTower"
			if (posDirection) {
				polygon.setTexture(&GameManager::redTowerRotatedTexture2);
			}
			else {
				polygon.setTexture(&GameManager::blueTowerRotatedTexture2);
			}
			temp.setString("Spray Tower (-450)");
			towerRadiuses.push_back(towerRange / 2);
		}
		else if (i == 2) { // towerType = "HealFountainTower"
			if (posDirection) {
				polygon.setTexture(&GameManager::redTowerRotatedTexture3);
			}
			else {
				polygon.setTexture(&GameManager::blueTowerRotatedTexture3);
			}
			temp.setString("Healing Tower (-450)");
			towerRadiuses.push_back(towerRange / 2);
		}
		temp.setOrigin(temp.getGlobalBounds().width / 2, temp.getGlobalBounds().height / 2);
		temp.setPosition(buildingButton.getGlobalBounds().left + buildingButton.getGlobalBounds().width / 2, buildingButton.getGlobalBounds().top + buildingButton.getGlobalBounds().height / 4);
		towers.push_back(polygon);
		buildsText.push_back(temp);
	}
	botMenuState = 0;
}

void Menus::buildUpgradeMenuForDisplay(int upgradeTypes, Hexagon & hexagon, std::vector<Tower *> teamTowers) {
	int j = 0, k = 0, offset = 0;
	while (k < teamTowers.size() && teamTowers[k]->getCenter() != sf::Vector2f(hexagon.centerX, hexagon.centerY)){
		k++; // find the correct tower
	}
	while (Tower::availableUpgrades[j].classType != teamTowers[k]->towerType) {
		j++; // find the correct available upgrades
	}
	if (teamTowers[k]->towerType == "RoundRobinTower") {
		offset = 1;
	}
	for (int i = 1 + offset; i < upgradeTypes + offset; i++) {
		sf::RectangleShape upgradeButton(sf::Vector2f((float) (botBackground.getGlobalBounds().width - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f * (upgradeTypes + 2)) / (upgradeTypes + 1), (float) (botBackground.getGlobalBounds().height - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 96.3132909f)));
		upgradeButton.setTexture(rand() % 2 ? &GameManager::botMenuTrimTexture : &GameManager::botMenuTrimTexture2);
		upgradeButton.setPosition(sf::Vector2f((float) (std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f + (i - offset) * (upgradeButton.getGlobalBounds().width + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)), (float) (botBackground.getGlobalBounds().top + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)));
		sf::Text temp;
		temp.setFont(GameManager::ringm);
		temp.setColor(sf::Color(255, 255, 0));
		temp.setCharacterSize((int) (std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 53.5073838f));
		if (i == 1) {
			temp.setString("Range (-150)");
			if (Tower::availableUpgrades[j].attackRadius <= teamTowers[k]->radius) {
				temp.setString("Max Range");
			}
		}
		else if (i == 2) {
			temp.setString("Frequency (-100)");
			if (Tower::availableUpgrades[j].attackSpeed >= teamTowers[k]->frequency) {
				temp.setString("Max Frequency");
			}
		}
		else if (i == 3) {
			temp.setString("Damage (-150)");
			if (Tower::availableUpgrades[j].damage <= teamTowers[k]->damage) {
				temp.setString("Max Damage");
			}
		}
		else if (i == 4) {
			temp.setString("Spray (-150)");
			if (Tower::availableUpgrades[j].sprayBullets <= teamTowers[k]->bulletsPerAttack) {
				temp.setString("Max Spray");
			}
		}
		temp.setOrigin(temp.getGlobalBounds().width / 2, temp.getGlobalBounds().height / 2);
		temp.setPosition(upgradeButton.getGlobalBounds().left + upgradeButton.getGlobalBounds().width / 2, upgradeButton.getGlobalBounds().top + upgradeButton.getGlobalBounds().height / 2);
		upgradeTowerMenu.push_back(upgradeButton); // buttons
		upgradesText.push_back(temp); // text for buttons
	}
	sf::RectangleShape upgradeButton(sf::Vector2f((float)(botBackground.getGlobalBounds().width - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f * (upgradeTypes + 2)) / (upgradeTypes + 1), (float)(botBackground.getGlobalBounds().height - std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 96.3132909f)));
	upgradeButton.setTexture(rand() % 2 ? &GameManager::botMenuTrimTexture : &GameManager::botMenuTrimTexture2);
	upgradeButton.setPosition(sf::Vector2f((float)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f + upgradeTypes * (upgradeButton.getGlobalBounds().width + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)), (float)(botBackground.getGlobalBounds().top + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)));
	sf::Text temp;
	temp.setFont(GameManager::ringm);
	temp.setColor(sf::Color(255, 255, 0));
	temp.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 53.5073838f));
	temp.setString("Delete (+300)");
	temp.setOrigin(temp.getGlobalBounds().width / 2, temp.getGlobalBounds().height / 2);
	temp.setPosition(upgradeButton.getGlobalBounds().left + upgradeButton.getGlobalBounds().width / 2, upgradeButton.getGlobalBounds().top + upgradeButton.getGlobalBounds().height / 2);
	upgradeTowerMenu.push_back(upgradeButton); // delete button
	upgradesText.push_back(temp); // delete button text
	// set the hexagon as selected and display upgrade menu
	selectedTower = hexagon;
	botMenuState = 1;
	updateTowerInfo(teamTowers, k); // update the info text
}

void Menus::sendBuildRequest(sf::RenderWindow & window) {
	// send build request to GameManager class
	Build tower;
	tower.x = sf::Mouse::getPosition(window).x;
	tower.y = sf::Mouse::getPosition(window).y;
	tower.team = posDirection;
	if (buildingTowerIndex == 0) {
		tower.classType = "Tower";
	}
	else if (buildingTowerIndex == 1) {
		tower.classType = "RoundRobinTower";
	}
	else if (buildingTowerIndex == 2) {
		tower.classType = "HealFountainTower";
	}
	GameManager::buildingQueue.push_back(tower);
	// reset the tower in build menu
	towers[buildingTowerIndex].setPosition(buildTowersMenu[buildingTowerIndex].getGlobalBounds().left + buildTowersMenu[buildingTowerIndex].getGlobalBounds().width / 2, buildTowersMenu[buildingTowerIndex].getGlobalBounds().top + buildTowersMenu[buildingTowerIndex].getGlobalBounds().height / 2);
	buildingTowerIndex = -1;
}

void Menus::sendUpgradeRequest(int i, std::vector<Tower *> teamTowers, int gold) {
	// send upgrade request to GameManager class
	for (int k = 0; k < teamTowers.size(); k++) {
		if (teamTowers[k]->getCenter() == sf::Vector2f(selectedTower.centerX, selectedTower.centerY)) { // find correct tower
			for (int j = 0; j < Tower::availableUpgrades.size(); j++) {
				if (Tower::availableUpgrades[j].classType == teamTowers[k]->towerType) { // find correct tower type for available upgrades
					Upgrade temp;
					temp.tower = *(teamTowers[k]);
					if (upgradesText[i].getString() == "Range (-150)" && Tower::availableUpgrades[j].attackRadius > teamTowers[k]->radius) {
						temp.towerUpgrades.attackRadius = std::min(Tower::availableUpgrades[j].attackRadius, teamTowers[k]->radius + 75.0f);
						if (Tower::availableUpgrades[j].attackRadius <= temp.towerUpgrades.attackRadius && gold >= 150) {
							upgradesText[i].setString("Max Range");
						}
					}
					else if (upgradesText[i].getString() == "Frequency (-100)" && Tower::availableUpgrades[j].attackSpeed < teamTowers[k]->frequency) {
						temp.towerUpgrades.attackSpeed = std::max(Tower::availableUpgrades[j].attackSpeed, teamTowers[k]->frequency - 50.0f);
						if (Tower::availableUpgrades[j].attackSpeed >= temp.towerUpgrades.attackSpeed && gold >= 100) {
							upgradesText[i].setString("Max Frequency");
						}
					}
					else if (upgradesText[i].getString() == "Damage (-150)" && Tower::availableUpgrades[j].damage > teamTowers[k]->damage) {
						temp.towerUpgrades.damage = std::min(Tower::availableUpgrades[j].damage, teamTowers[k]->damage + 5);
						if (Tower::availableUpgrades[j].damage <= temp.towerUpgrades.damage && gold >= 150) {
							upgradesText[i].setString("Max Damage");
						}
					}
					else if (upgradesText[i].getString() == "Spray (-150)" && Tower::availableUpgrades[j].sprayBullets > teamTowers[k]->bulletsPerAttack) {
						temp.towerUpgrades.sprayBullets = std::min(Tower::availableUpgrades[j].sprayBullets, teamTowers[k]->bulletsPerAttack * 2);
						if (Tower::availableUpgrades[j].sprayBullets <= temp.towerUpgrades.sprayBullets && gold >= 150) {
							upgradesText[i].setString("Max Spray");
						}
					}
					else if (upgradesText[i].getString() == "Delete (+300)") {
						temp.shouldRemove = true;
						botMenuState = 0;
					}
					// set the upgradeText position again just in case we changed the text
					upgradesText[i].setOrigin(upgradesText[i].getGlobalBounds().width / 2, upgradesText[i].getGlobalBounds().height / 2);
					upgradesText[i].setPosition(upgradeTowerMenu[i].getGlobalBounds().left + upgradeTowerMenu[i].getGlobalBounds().width / 2, upgradeTowerMenu[i].getGlobalBounds().top + upgradeTowerMenu[i].getGlobalBounds().height / 2);
					GameManager::upgradesQueue.push_back(temp);
					return;
				}
			}
		}
	}
}

void Menus::updateTowerInfo(std::vector<Tower *> teamTowers, int k) {
	if (k == -1 || k > teamTowers.size()) { // k is -1 iff we need to find the correct tower
		k = 0;
		while (k < teamTowers.size() && teamTowers[k]->getCenter() != sf::Vector2f(selectedTower.centerX, selectedTower.centerY)) {
			k++; // find correct tower
		}
	}
	if (k < teamTowers.size()) {
		info = sf::Text();
		info.setFont(GameManager::blkchcry);
		info.setColor(sf::Color(255, 255, 0));
		info.setCharacterSize((int)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 53.5073838f / 1.75f));
		char arr[] = "Attack Radius: 300.0\nAttack Speed: 200.0\nAttack Damage: 40\nBullet Spray: 16\n";
		if (teamTowers[k]->towerType == "HealFountainTower") {
			sprintf_s(arr, sizeof(arr), "Heal Radius: %.1f\nHeal Speed: %.1f\nHeal Amount: %d\nHeal Sides: %d\n", teamTowers[k]->radius, teamTowers[k]->frequency, teamTowers[k]->damage, teamTowers[k]->bulletsPerAttack);
		}
		else {
			sprintf_s(arr, sizeof(arr), "Attack Radius: %.1f\nAttack Speed: %.1f\nDamage: %d\nBullet Spray: %d\n", teamTowers[k]->radius, teamTowers[k]->frequency, teamTowers[k]->damage, teamTowers[k]->bulletsPerAttack);
		}
		info.setString(arr);
		info.setPosition(sf::Vector2f((float)(std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) * 5.0f / 192.626582f), (float)(botBackground.getGlobalBounds().top + std::sqrt(std::pow(screenWidth, 2) + std::pow(screenHeight, 2)) / 192.626582f)));
	}
}