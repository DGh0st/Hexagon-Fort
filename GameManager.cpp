#include "GameManager.h"

std::vector<sf::Vector2i> Minion::path;
std::vector<Upgrades> Tower::availableUpgrades;

GameManager::GameManager(sf::RenderWindow & window) {
	setupStartScreen(window); // display start up screen
	srand((int)time(NULL)); // set up seed for random generators

	Minion::path = std::vector<sf::Vector2i>({ {19, 58}, {30, 64}, {52, 76}, {74, 89}, {96, 101}, {117, 89}, {139, 101}, {161, 114}, {161, 139}, {161, 164},  {139, 177}, {117, 189}, {96, 202}, {96, 227},  {96, 252}, {117, 264}, {139, 277}, {139, 302}, {139, 327}, {117, 340}, {96, 352}, {74, 365}, {74, 390}, {96, 403}, {117, 415}, {139, 403},  {161, 415}, {183, 403}, {204, 390}, {204, 365}, {204, 340}, {226, 327}, {226, 302}, {226, 277}, {248, 264}, {270, 252}, {270, 227}, {270, 202}, {248, 189}, {248, 164}, {248, 139}, {270, 126},  {291, 139}, {313, 151}, {335, 164}, {335, 189}, {335, 214}, {335, 239}, {357, 252}, {378, 264},
								{400, 277}, {422, 290}, {444, 302}, {444, 327}, {444, 352}, {444, 377}, {465, 390}, {487, 403}, {509, 415}, {531, 403}, {531, 377}, {531, 352}, {509, 340},  {509, 315}, {509, 290}, {531, 277}, {552, 264}, {552, 239}, {552, 214}, {574, 202}, {574, 177}, {574, 151}, {596, 139}, {618, 126}, {639, 139}, {661, 126}, {683, 139}, {705, 151}, {705, 177}, {683, 189}, {661, 202}, {639, 214}, {639, 239}, {639, 264}, {661, 277}, {683, 290}, {683, 315}, {683, 340}, {661, 352}, {639, 365}, {618, 377}, {618, 403}, {618, 428}, {639, 440}, {661, 453}, {683, 440}, {705, 453}, {726, 465}, {748, 478}, {759, 486}});

	if (window.getSize() != sf::Vector2u(780, 565)) {
		for (size_t i = 0; i < Minion::path.size(); i++) { // scale minion path based on scale of window
			Minion::path[i].x = Minion::path[i].x * window.getSize().x / 780;
			Minion::path[i].y = Minion::path[i].y * window.getSize().y / 565;
		}
	}

	// red team
	red.color = sf::Color::Red;
	red.side = true;
	red.towerRange = (float) std::sqrt(std::pow(window.getSize().x, 2) + std::pow(window.getSize().y, 2)) / 9.63132909f;
	// blue team
	blue.color = sf::Color::Cyan;
	blue.side = false;
	blue.towerRange = (float) std::sqrt(std::pow(window.getSize().x, 2) + std::pow(window.getSize().y, 2)) / 9.63132909f;

	map = FlatHexagonalGrid((float) std::sqrt(std::pow(window.getSize().x, 2) + std::pow(window.getSize().y, 2)) / 66.4229592f, window.getSize().x / 24.37500f, window.getSize().y / 8.69230769f, 34, 17, sf::Color::Black);
	for (int i = 0; i < Minion::path.size(); i++) { // set path for the minions in map (grid)
		map.setColor(map.getHexagon(Minion::path[i].x, Minion::path[i].y), sf::Color::Color(142, 122, 140));
	}

	bars = Menus(&map, &window, (user = true)); // top and bottom menus

	ai = AI(!user); // computer (AI)

	setupAvailableUpgrades();
}

GameManager::~GameManager() {
	for (size_t i = 0; i < redTowers.size(); i--) { // delete red towers
		delete redTowers[i];
		redTowers[i] = NULL;
		redTowers.erase(redTowers.begin() + i);
	}
	for (size_t i = 0; i < blueTowers.size(); i--) { // delete blue towers
		delete blueTowers[i];
		blueTowers[i] = NULL;
		blueTowers.erase(blueTowers.begin() + i);
	}
}

void GameManager::operator=(const GameManager & other) {
	user = other.user;
	red = other.red;
	blue = other.blue;
	map = other.map;
	bars = other.bars;
	redTowers = other.redTowers;
	blueTowers = other.blueTowers;
	minionTimer = other.minionTimer;
	redMinions = other.redMinions;
	blueMinions = other.blueMinions;
	ai = other.ai;
	gameOverText = other.gameOverText;
	clickAnyText = other.clickAnyText;
	backdrop = other.backdrop;
	nameText = other.nameText;
	startGameButton = other.startGameButton;
	musicCredits = other.musicCredits;
}

void GameManager::draw(sf::RenderWindow & window, int gameState, int winner) {
	if (gameState == 0) { // start screen
		window.draw(startGameButton);
		window.draw(nameText);
		window.draw(musicCredits);
		minionTimer.restart(); // restart minion generate timer so minions do not spawn before game starts
	}
	else { // in game screen
		// map.draw(window);

		// towers
		for (int i = 0; i < redTowers.size(); i++) {
			redTowers[i]->draw(window);
		}
		for (int i = 0; i < blueTowers.size(); i++) {
			blueTowers[i]->draw(window);
		}
		// minions
		for (int i = 0; i < redMinions.size(); i++) {
			redMinions[i].draw(window);
		}
		for (int i = 0; i < blueMinions.size(); i++) {
			blueMinions[i].draw(window);
		}
		// effects
		for (int i = 0; i < effects.size(); i++) {
			window.draw(effects[i]);
		}
		// menus
		bars.draw(window);
		if (gameState == 3) { // gameover screen
			if (user) {
				if (winner == 1) { // you lose
					setupPauseOrGameOverScreen(window, "Game Over!", "You Lose", "Click anywhere or press SPACEBAR to restart game");
				}
				else { // you won
					setupPauseOrGameOverScreen(window, "Congratulations!", "You Win", "Click anywhere or press SPACEBAR to restart game");
				}
			}
			else {
				if (winner == 0) { // you lose
					setupPauseOrGameOverScreen(window, "Game Over!", "You Lose", "Click anywhere or press SPACEBAR to restart game");
				}
				else {
					setupPauseOrGameOverScreen(window, "Congratulations!", "You Win", "Click anywhere or press SPACEBAR to restart game");
				}
			}
			window.draw(backdrop);
			window.draw(clickAnyText);
			window.draw(gameOverText);
			window.draw(underGameOverText);
		}
		else if (gameState == 2) { // pause screen
			setupPauseOrGameOverScreen(window, "Game Paused!", "", "Click anywhere or press SPACEBAR to continue");
			window.draw(backdrop);
			window.draw(clickAnyText);
			window.draw(gameOverText);
			window.draw(underGameOverText);
		}
	}
}

void GameManager::generateMinionsIfNeeded(float seconds, int maxTeamSize, sf::RenderWindow & window) {
	if (areTimersViable && minionTimer.getElapsedTime().asSeconds() > seconds) { // generate minion on each team if it can have more
		minionTimer.restart();
		if (redMinions.size() < maxTeamSize) {
			redMinions.insert(redMinions.begin(), Minion((float)std::sqrt(std::pow(window.getSize().x, 2) + std::pow(window.getSize().y, 2)) / 192.626582f, rand() % 3, red.side, red.color, red.minionDamage, red.minionHealth, red.minionSpeed, window));
		}
		if (blueMinions.size() < maxTeamSize) {
			blueMinions.insert(blueMinions.begin(), Minion((float)std::sqrt(std::pow(window.getSize().x, 2) + std::pow(window.getSize().y, 2)) / 192.626582f, rand() % 3, blue.side, blue.color, blue.minionDamage, blue.minionHealth, blue.minionSpeed, window));
		}
	}
}

void GameManager::minionMovement() {
	for (int i = 0; i < redMinions.size(); i++) { // start attempting to fight from the end of the list (oldest minions)
		for (int j = 0; j < blueMinions.size(); j++) {
			redMinions[redMinions.size() - 1 - i].shouldFightOrWait(blueMinions[blueMinions.size() - 1 - j]);
		}
	}
	if (!redMinions.empty() && redMinions[redMinions.size() - 1].state == 3) { // attack blue base
		blue.totalHealth -= redMinions[redMinions.size() - 1].damage;
		redMinions[redMinions.size() - 1].state = 0;
	}
	else if (!blueMinions.empty() && blueMinions[blueMinions.size() - 1].state == 3) { // attack red base
		red.totalHealth -= blueMinions[blueMinions.size() - 1].damage;
		blueMinions[blueMinions.size() - 1].state = 0;
	}
	for (int i = 0; i < redMinions.size(); i++) {
		if (redMinions[i].isDead()) {// || redMinions[i].getPosition().x < 0 || redMinions[i].getPosition().x > window.getSize().x || redMinions[i].getPosition().y < 0 || redMinions[i].getPosition().y > window.getSize().y) {
			int gold = rand() % 5 + 10;
			blue.gold += gold;
			if (!user) {
				sf::Text gainGold;
				gainGold.setFont(GameManager::blkchcry);
				gainGold.setCharacterSize(21);
				gainGold.setString("+ " + std::to_string(std::abs(gold)));
				gainGold.setOrigin(gainGold.getGlobalBounds().width / 2, gainGold.getGlobalBounds().height / 2);
				gainGold.setColor(sf::Color(230, 190, 138));
				gainGold.setPosition(redMinions[i].getPosition() - sf::Vector2f(0, gainGold.getGlobalBounds().height));
				effects.push_back(gainGold);
			}
			redMinions.erase(redMinions.begin() + i);
		}
		else {
			redMinions[i].move();
			if (i < redMinions.size() - 1) { // if minion is not leading the charge than check if it should wait behind teammate
				redMinions[i].shouldFightOrWait(redMinions[i + 1]);
			}
		}
	}
	for (int i = 0; i < blueMinions.size(); i++) {
		if (blueMinions[i].isDead()) {// || blueMinions[i].getPosition().x < 0 || blueMinions[i].getPosition().x > window.getSize().x || blueMinions[i].getPosition().y < 0 || blueMinions[i].getPosition().y > window.getSize().y) {
			int gold = rand() % 5 + 10;
			red.gold += gold;
			if (user) {
				sf::Text gainGold;
				gainGold.setFont(GameManager::blkchcry);
				gainGold.setCharacterSize(21);
				gainGold.setString("+ " + std::to_string(std::abs(gold)));
				gainGold.setOrigin(gainGold.getGlobalBounds().width / 2, gainGold.getGlobalBounds().height / 2);
				gainGold.setColor(sf::Color(230, 190, 138));
				gainGold.setPosition(blueMinions[i].getPosition() - sf::Vector2f(0, gainGold.getGlobalBounds().height));
				effects.push_back(gainGold);
			}
			blueMinions.erase(blueMinions.begin() + i);
		}
		else {
			blueMinions[i].move();
			if (i < blueMinions.size() - 1) { // if minion is not leading the charge than check if it should wait behind teammate
				blueMinions[i].shouldFightOrWait(blueMinions[i + 1]);
			}
		}
	}
}

void GameManager::towerAttack(sf::RenderWindow & window) {
	// tower attack and update tower bullets
	for (int i = 0; i < redTowers.size(); i++) {
		if (redTowers[i]->towerType == "HealFountainTower") {
			((HealFountainTower *)redTowers[i])->attack(redMinions, window);
		}
		else if (redTowers[i]->towerType == "RoundRobinTower") {
			((RoundRobinTower *)redTowers[i])->attack(blueMinions, window);
		}
		else {
			redTowers[i]->attack(blueMinions, window);
		}
		redTowers[i]->moveBullets();
	}
	for (int i = 0; i < blueTowers.size(); i++) {
		if (blueTowers[i]->towerType == "HealFountainTower") {
			((HealFountainTower *)blueTowers[i])->attack(blueMinions, window);
		}
		else if (blueTowers[i]->towerType == "RoundRobinTower") {
			((RoundRobinTower *)blueTowers[i])->attack(redMinions, window);
		}
		else {
			blueTowers[i]->attack(redMinions, window);
		}
		blueTowers[i]->moveBullets();
	}
}

int GameManager::getRedTeamHealth() {
	return red.totalHealth;
}

int GameManager::getBlueTeamHealth() {
	return blue.totalHealth;
}

int GameManager::findInsertLocation(std::vector<Tower *> teamTowers, Hexagon hex) {
	for (int i = 0; i < teamTowers.size(); i++) {
		if (teamTowers[i]->getCenter().y > hex.centerY) {
			return i;
		}
	}
	return (int) teamTowers.size();
}

bool GameManager::buildTower(Hexagon hex, bool teamRed, std::string towerType) {
	if (hex.hex != NULL) {
		if (teamRed && hex.hex->getFillColor() == sf::Color(1, 0, 0) && red.gold >= 450) {
			if (towerType == "Tower") {
				redTowers.insert(redTowers.begin() + findInsertLocation(redTowers, hex), new Tower(hex, red.side, red.towerRange, red.towerAttackSpeed, red.towerDamage, red.towerHealth, red.color, red.bulletsToSpray));
			}
			else if (towerType == "RoundRobinTower") {
				redTowers.insert(redTowers.begin() + findInsertLocation(redTowers, hex), (Tower *)(new RoundRobinTower(hex, red.side, red.towerRange * 0.5f, red.towerAttackSpeed * 0.75f, red.towerDamage * 3 / 4, red.towerHealth, red.color, red.bulletsToSpray * 3)));
			}
			else if (towerType == "HealFountainTower") {
				redTowers.insert(redTowers.begin() + findInsertLocation(redTowers, hex), (Tower *)(new HealFountainTower(hex, red.side, red.towerRange * 0.5f, red.towerAttackSpeed * 0.75f, red.towerDamage * 4, red.totalHealth, red.color, red.bulletsToSpray * 12)));
			}
			if (teamRed == user) {
				hammerSound.play();
			}
			red.gold -= 450; // pay the price
			if (user == red.side) {
				sf::Text spendGold;
				spendGold.setFont(GameManager::blkchcry);
				spendGold.setCharacterSize(21);
				spendGold.setString("- 450");
				spendGold.setOrigin(spendGold.getGlobalBounds().width / 2, spendGold.getGlobalBounds().height / 2);
				spendGold.setColor(sf::Color(230, 190, 138));
				spendGold.setPosition(redTowers[0]->getCenter() - sf::Vector2f(0, spendGold.getGlobalBounds().height));
				effects.push_back(spendGold);
			}
			return true;
		}
		else if (!teamRed && hex.hex->getFillColor() == sf::Color(0, 0, 1) && blue.gold >= 450) {
			if (towerType == "Tower") {
				blueTowers.insert(blueTowers.begin() + findInsertLocation(blueTowers, hex), new Tower(hex, blue.side, blue.towerRange, blue.towerAttackSpeed, blue.towerDamage, blue.towerHealth, blue.color, red.bulletsToSpray));
			}
			else if(towerType == "RoundRobinTower") {
				blueTowers.insert(blueTowers.begin() + findInsertLocation(blueTowers, hex), (Tower *)(new RoundRobinTower(hex, blue.side, blue.towerRange * 0.5f, blue.towerAttackSpeed * 0.75f, blue.towerDamage * 3 / 4, blue.towerHealth, blue.color, red.bulletsToSpray * 3)));
			}
			else if (towerType == "HealFountainTower") {
				blueTowers.insert(blueTowers.begin() + findInsertLocation(blueTowers, hex), (Tower *)(new HealFountainTower(hex, blue.side, blue.towerRange * 0.5f, blue.towerAttackSpeed * 0.75f, blue.towerDamage * 4, blue.towerHealth, blue.color, blue.bulletsToSpray * 12)));
			}
			if (teamRed == user) {
				hammerSound.play();
			}
			blue.gold -= 450; // pay the price
			if (user == blue.side) {
				sf::Text spendGold;
				spendGold.setFont(GameManager::blkchcry);
				spendGold.setCharacterSize(21);
				spendGold.setString("- 450");
				spendGold.setOrigin(spendGold.getGlobalBounds().width / 2, spendGold.getGlobalBounds().height / 2);
				spendGold.setColor(sf::Color(230, 190, 138));
				spendGold.setPosition(blueTowers[0]->getCenter() - sf::Vector2f(0, spendGold.getGlobalBounds().height));
				effects.push_back(spendGold);
			}
			return true;
		}
	}
	return false; // unable to build
}

void GameManager::manageBuildingQueue() {
	while (!buildingQueue.empty()) { // go through building queue (vector)
		buildTower(map.getHexagon(buildingQueue[0].x, buildingQueue[0].y), buildingQueue[0].team, buildingQueue[0].classType);
		buildingQueue.erase(buildingQueue.begin() + 0);
	}
}

void GameManager::manageUpgradesQueue() {
	bool changesMade = false;
	while (!upgradesQueue.empty()) { // go through upgrade queue (vector)
		if (upgradesQueue[0].tower.colorOfHexagon == red.color) {
			for (int i = 0; i < redTowers.size(); i++) {
				if (redTowers[i]->getCenter() == upgradesQueue[0].tower.getCenter()) { // find the correct tower
					if (upgradesQueue[0].shouldRemove) { // delete tower
						delete redTowers[i];
						redTowers[i] = NULL;
						redTowers.erase(redTowers.begin() + i);
						map.setColor(map.getHexagon((int) upgradesQueue[0].tower.getCenter().x, (int) upgradesQueue[0].tower.getCenter().y), sf::Color(1, 0, 0));
						red.gold += 300;
						destroySound.play();
					}
					else if (upgradesQueue[0].towerUpgrades.attackRadius > redTowers[i]->radius && red.gold >= 150) { // upgrade radius
						redTowers[i]->radius = upgradesQueue[0].towerUpgrades.attackRadius;
						red.gold -= 150;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.attackSpeed < redTowers[i]->frequency && red.gold >= 100) { // upgrade frequency
						redTowers[i]->frequency = upgradesQueue[0].towerUpgrades.attackSpeed;
						red.gold -= 100;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.damage > redTowers[i]->damage && red.gold >= 150) { // upgrade damage
						redTowers[i]->damage = upgradesQueue[0].towerUpgrades.damage;
						red.gold -= 150;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.sprayBullets > redTowers[i]->bulletsPerAttack && red.gold >= 150) { // upgrade spray
						redTowers[i]->bulletsPerAttack = upgradesQueue[0].towerUpgrades.sprayBullets;
						red.gold -= 125;
						changesMade = true;
					}
					if (user == red.side) {
						bars.updateTowerInfo(redTowers, i); // update tower's info text in menus
						if (changesMade) {
							coinDropSound.play();
						}
					}
				}
			}
		}
		else if (upgradesQueue[0].tower.colorOfHexagon == blue.color) {
			for (int i = 0; i < blueTowers.size(); i++) {
				if (blueTowers[i]->getCenter() == upgradesQueue[0].tower.getCenter()) { // find the correct tower
					if (upgradesQueue[0].shouldRemove) { // delete tower
						delete blueTowers[i];
						blueTowers[i] = NULL;
						blueTowers.erase(blueTowers.begin() + i);
						map.setColor(map.getHexagon((int) upgradesQueue[0].tower.getCenter().x, (int) upgradesQueue[0].tower.getCenter().y), sf::Color(0, 0, 1));
						blue.gold += 300;
						destroySound.play();
					}
					else if (upgradesQueue[0].towerUpgrades.attackRadius > blueTowers[i]->radius && blue.gold >= 150) { // upgrade radius
						blueTowers[i]->radius = upgradesQueue[0].towerUpgrades.attackRadius;
						blue.gold -= 150;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.attackSpeed < blueTowers[i]->frequency && blue.gold >= 100) { // upgrade frequency
						blueTowers[i]->frequency = upgradesQueue[0].towerUpgrades.attackSpeed;
						blue.gold -= 100;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.damage > blueTowers[i]->damage && blue.gold >= 150) { // upgrade damage
						blueTowers[i]->damage = upgradesQueue[0].towerUpgrades.damage;
						blue.gold -= 150;
						changesMade = true;
					}
					else if (upgradesQueue[0].towerUpgrades.sprayBullets > blueTowers[i]->bulletsPerAttack && blue.gold >= 150) { // upgrade spray
						blueTowers[i]->bulletsPerAttack = upgradesQueue[0].towerUpgrades.sprayBullets;
						blue.gold -= 125;
						changesMade = true;
					}
					if (user == blue.side) { // update tower's info text in menus
						bars.updateTowerInfo(blueTowers, i);
						if (changesMade) {
							coinDropSound.play();
						}
					}
				}
			}
		}
		upgradesQueue.erase(upgradesQueue.begin() + 0);
	}
}

bool GameManager::loadNecessaryFiles() {
	if (!ringm.loadFromFile("fonts/RINGM.ttf")) {
		return false; // could not find ringm font
	}
	if (!blkchcry.loadFromFile("fonts/BLKCHCRY.ttf")) {
		return false; // could not find blkchry font
	}
	if (!beyondWonderland.loadFromFile("fonts/Beyond Wonderland.ttf")) {
		return false; // could not find beyond wonderland font
	}
	if (!redMinionTexture.loadFromFile("images/red_minions.png")) {
		return false; // could not find red_minions texture
	}
	if (!blueMinionTexture.loadFromFile("images/blue_minions.png")) {
		return false; // could not find blue_minions texture
	}
	if (!fireballTexture.loadFromFile("images/fireball.png")) {
		return false; // could not find fireball texture
	}
	if (!iceballTexture.loadFromFile("images/iceball.png")) {
		return false; // could not find iceball texture
	}
	if (!healingballTexture.loadFromFile("images/healingball.png")) {
		return false; // could not find healingball texture
	}
	if (!redTowerTexture.loadFromFile("images/red_towers.png")) {
		return false; // could not find red_towers texture
	}
	if (!blueTowerTexture.loadFromFile("images/blue_towers.png")) {
		return false; // could not find blue_towers texture
	}
	if (!redTowerRotatedTexture.loadFromFile("images/red_towers_rotated.png")) {
		return false; // could not find red_towers_rotated texture
	}
	if (!blueTowerRotatedTexture.loadFromFile("images/blue_towers_rotated.png")) {
		return false; // could not find red_towers_rotated texture
	}
	if (!redTowerTexture2.loadFromFile("images/red_towers2.png")) {
		return false; // could not find red_towers2 texture
	}
	if (!blueTowerTexture2.loadFromFile("images/blue_towers2.png")) {
		return false; // could not find blue_towers2 texture
	}
	if (!redTowerRotatedTexture2.loadFromFile("images/red_towers_rotated2.png")) {
		return false; // could not find red_towers_rotated2 texture
	}
	if (!blueTowerRotatedTexture2.loadFromFile("images/blue_towers_rotated2.png")) {
		return false; // could not find red_towers_rotated2 texture
	}
	if (!redTowerTexture3.loadFromFile("images/red_towers3.png")) {
		return false; // could not find red_towers3 texture
	}
	if (!blueTowerTexture3.loadFromFile("images/blue_towers3.png")) {
		return false; // could not find blue_towers3 texture
	}
	if (!redTowerRotatedTexture3.loadFromFile("images/red_towers_rotated3.png")) {
		return false; // could not find red_towers_rotated3 texture
	}
	if (!blueTowerRotatedTexture3.loadFromFile("images/blue_towers_rotated3.png")) {
		return false; // could not find red_towers_rotated3 texture
	}
	if (!startButtonSelectedTexture.loadFromFile("images/start_button_selected.png")) {
		return false; // could not find start_button_selected texture
	}
	if (!startButtonUnSelectedTexture.loadFromFile("images/start_button_unselected.png")) {
		return false; // could not find start_button_unselected texture
	}
	if (!topMenuTexture.loadFromFile("images/top_bar.png")) {
		return false; // could not find top_bar texture
	}
	if (!botMenuTexture.loadFromFile("images/bot_bar.png")) {
		return false; // could not find bot_bar texture
	}
	if (!botMenuTrimTexture.loadFromFile("images/bot_menu_trim.png")) {
		return false; // could not find bot_menu_trim texture
	}
	if (!botMenuTrimTexture2.loadFromFile("images/bot_menu_trim2.png")) {
		return false; // could not find the bot_menu_trim2 texture
	}
	if (!gameMapTexture.loadFromFile("images/game_map.png")) {
		return false; // could not find the game_map texture
	}
	if (!coinDropBuffer.loadFromFile("sounds/coin_drop.wav")) {
		return false; // could not find the coin_drop sound
	}
	else {
		coinDropSound.setBuffer(coinDropBuffer);
	}
	if (!hammerBuffer.loadFromFile("sounds/hammer.wav")) {
		return false; // could not find the hammer sound
	}
	else {
		hammerSound.setBuffer(hammerBuffer);
	}
	if (!destroyBuffer.loadFromFile("sounds/destroy.flac")) {
		return false; // could not find the destroy sound
	}
	else {
		destroySound.setBuffer(destroyBuffer);
	}
	return true;
}

void GameManager::runGameManager(sf::RenderWindow & window, int maxMinionsPerTeam, float minionSpawnSpeed) {
	// call functions required to update the game
	generateMinionsIfNeeded(minionSpawnSpeed, maxMinionsPerTeam, window);
	minionMovement();
	towerAttack(window);
	ai.updateMode(window, !user ? red.gold : blue.gold, !user ? redTowers : blueTowers);
	bars.manageMenus(window, user ? redTowers : blueTowers, red.totalHealth, blue.totalHealth, user ? red.gold : blue.gold, 1000, user ? red.color : blue.color);
	manageBuildingQueue();
	manageUpgradesQueue();
	for (int i = 0; i < effects.size(); i++) {
		effects[i].setPosition(effects[i].getPosition() - sf::Vector2f(0, effects[i].getGlobalBounds().height / 4));
		effects[i].setColor(effects[i].getColor() - sf::Color(0, 0, 0, 16));
		if (effects[i].getColor().a <= 0) {
			effects.erase(effects.begin() + i);
		}
	}
}

void GameManager::setupStartScreen(sf::RenderWindow & window) {
	nameText = sf::Text(sf::String("Hexagon Fort"), GameManager::beyondWonderland, 128); // title
	startGameButton = sf::RectangleShape(sf::Vector2f(window.getSize().x / 3.0f, window.getSize().y / 10.0f)); // start button
	musicCredits = sf::Text(sf::String("Music Credits:\nFailing Defense Kevin MacLeod (incompetech.com)\nLicensed under Creative Commons: By Attribution 3.0 License\nhttp://creativecommons.org/licenses/by/3.0/"), GameManager::blkchcry, 16); // music credits

	nameText.setOrigin(nameText.getGlobalBounds().width / 2, nameText.getGlobalBounds().height / 2);
	startGameButton.setOrigin(startGameButton.getGlobalBounds().width / 2, startGameButton.getGlobalBounds().height / 2);
	musicCredits.setOrigin(0, musicCredits.getGlobalBounds().height);

	nameText.setPosition((float) window.getSize().x / 2, (float) window.getSize().y / 2 - nameText.getGlobalBounds().height * 3 / 2);
	startGameButton.setPosition((float) window.getSize().x / 2, (float) window.getSize().y / 2);
	musicCredits.setPosition((float) window.getSize().x / 64, (float) window.getSize().y * 63 / 64);

	nameText.setColor(sf::Color(178, 178, 0));
	startGameButton.setTexture(&GameManager::startButtonUnSelectedTexture);
	musicCredits.setColor(sf::Color(78, 78, 0));
}

void GameManager::setupPauseOrGameOverScreen(sf::RenderWindow & window, std::string header, std::string underHeader, std::string description) {
	gameOverText = sf::Text(sf::String(header), GameManager::beyondWonderland, 96); // game over text
	underGameOverText = sf::Text(sf::String(underHeader), GameManager::blkchcry, 24); // description of gameover
	clickAnyText = sf::Text(sf::String(description), GameManager::blkchcry, 32); // click to restart text
	backdrop = sf::RectangleShape((sf::Vector2f) window.getSize()); // backdrop to make it look nice

	gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2, gameOverText.getGlobalBounds().height / 2);
	underGameOverText.setOrigin(underGameOverText.getGlobalBounds().width / 2, underGameOverText.getGlobalBounds().height / 2);
	clickAnyText.setOrigin(clickAnyText.getGlobalBounds().width / 2, clickAnyText.getGlobalBounds().height / 2);

	gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - gameOverText.getGlobalBounds().height / 2.0f);
	underGameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + underGameOverText.getGlobalBounds().height / 2.0f);
	clickAnyText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + underGameOverText.getGlobalBounds().height * 3.0f / 2.0f + clickAnyText.getGlobalBounds().height / 2.0f);

	backdrop.setFillColor(sf::Color(0, 0, 0, 150));
}

bool GameManager::isOnStartButton(sf::Vector2f point) {
	if (startGameButton.getGlobalBounds().contains(point)) { // hover effect for the start button
		startGameButton.setTexture(&GameManager::startButtonSelectedTexture);
		return true;
	}
	startGameButton.setTexture(&GameManager::startButtonUnSelectedTexture);
	return false;
}

void GameManager::setupAvailableUpgrades() {
	Upgrades tower; // Tower at max upgrades
	tower.attackRadius = red.towerRange * 1.75f;
	tower.attackSpeed = red.towerAttackSpeed / 1.5f;
	tower.damage = (int)(red.towerDamage * 1.5);
	tower.sprayBullets = red.bulletsToSpray;
	tower.classType = "Tower";
	Tower::availableUpgrades.push_back(tower);

	Upgrades roundRobin; // RoundRobinTower at max upgrades
	roundRobin.attackRadius = red.towerRange * 0.5f;
	roundRobin.attackSpeed = red.towerAttackSpeed * 0.4f;
	roundRobin.damage = red.towerDamage;
	roundRobin.sprayBullets = red.bulletsToSpray * 12;
	roundRobin.classType = "RoundRobinTower";
	Tower::availableUpgrades.push_back(roundRobin);

	Upgrades healFountain; // HealFountainTower at max upgrades
	healFountain.attackRadius = red.towerRange * 0.75f;
	healFountain.attackSpeed = red.towerAttackSpeed * 0.25f;
	healFountain.damage = red.towerDamage * 5;
	healFountain.sprayBullets = red.bulletsToSpray * 12;
	healFountain.classType = "HealFountainTower";
	Tower::availableUpgrades.push_back(healFountain);
}