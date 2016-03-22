#include "AI.h"
#include "GameManager.h"

AI::AI(bool team) : team(team) {
	srand((int)time(NULL));
}

AI::~AI() {
	// nothing to delete as of yet ...
}

void AI::operator=(const AI & other) {
	team = other.team;
}

void AI::updateMode(sf::RenderWindow & window, int gold, std::vector<Tower *> towers) {
	if (rand() % 5 > 3.25f && gold >= 150 && towers.size() > 0) { // 35% probability of upgrading tower
		sendUpgradeRequest(towers);
	}
	else if (gold >= 450) { // 65% probability of building tower
		sendBuildRequest(window);
	}
}

void AI::sendUpgradeRequest(std::vector<Tower *> towers) {
	int k = (int)rand() % towers.size(); // pick a random tower (generate index)
	for (int j = 0; j < Tower::availableUpgrades.size(); j++) {
		if (Tower::availableUpgrades[j].classType == towers[k]->towerType) { // find correct tower based on generated index
			int i = (int)rand() % (towers[k]->towerType == "Tower" ? 3 : 4); // pick a random upgrade (generate index)
			Upgrade temp;
			temp.tower = *(towers[k]);
			if (i == 0 && Tower::availableUpgrades[j].attackRadius > towers[k]->radius) {
				temp.towerUpgrades.attackRadius = std::min(Tower::availableUpgrades[j].attackRadius, towers[k]->radius + 75.0f);
			}
			else if (i == 1 && Tower::availableUpgrades[j].attackSpeed < towers[k]->frequency) {
				temp.towerUpgrades.attackSpeed = std::max(Tower::availableUpgrades[j].attackSpeed, towers[k]->frequency - 50.0f);
			}
			else if (i == 2 && Tower::availableUpgrades[j].damage > towers[k]->damage) {
				temp.towerUpgrades.damage = std::min(Tower::availableUpgrades[j].damage, towers[k]->damage + 5);
			}
			else if (i == 3 && towers[k]->towerType == "RoundRobinTower" && Tower::availableUpgrades[j].sprayBullets > towers[k]->bulletsPerAttack) {
				temp.towerUpgrades.sprayBullets = std::min(Tower::availableUpgrades[j].sprayBullets, towers[k]->bulletsPerAttack * 2);
			}
			GameManager::upgradesQueue.push_back(temp); // send request
			return;
		}
	}
}

void AI::sendBuildRequest(sf::RenderWindow & window) {
	Build tower;
	tower.x = rand() % (window.getSize().x / 2); // generate the x coordinate to build tower on
	if (!team) {
		tower.x += (window.getSize().x / 2); // move x coordinate to blue side of team is blue side
	}
	tower.y = rand() % window.getSize().y; // generate a y coordinate
	sf::Vector2i path = closestMinionPath(tower.x, tower.y);
	tower.team = team;
	if (std::sqrt(std::pow(std::abs(tower.x - path.x), 2) + std::pow(std::abs(tower.y - path.y), 2)) < 50.0f) { // increase probability of building RoundRobinTower near the minion path
		tower.classType = rand() % 3 > 0.25f ? "RoundRobinTower" : "Tower";
	}
	else if (std::sqrt(std::pow(std::abs(tower.x - path.x), 2) + std::pow(std::abs(tower.y - path.y), 2)) < 75.0f) {
		tower.classType = rand() % 3 > 0.25f ? rand() % 3 > 1.25f ? "RoundRobinTower" : "HealFountainTower" : "Tower"; // ternary operator inception
	}
	else if (std::sqrt(std::pow(std::abs(tower.x - path.x), 2) + std::pow(std::abs(tower.y - path.y), 2)) < 175.0f) { // otherwise increase probability of building Tower away form minion path
		tower.classType = "Tower";
	}
	GameManager::buildingQueue.push_back(tower);
}

sf::Vector2i AI::closestMinionPath(int x, int y) {
	sf::Vector2i smallest = Minion::path[Minion::path.size() / 2]; // set the middle of the minion path as smallest
	for (size_t i = (team ? 0 : Minion::path.size() / 2); i < Minion::path.size() * (team ? 1 / 2 : 1); i++) {
		if (std::pow(std::abs(smallest.x - x), 2) + std::pow(std::abs(smallest.y - y), 2) > std::pow(std::abs(Minion::path[i].x - x), 2) + std::pow(std::abs(Minion::path[i].y - y), 2)) {
			smallest = Minion::path[i];
		}
	}
	return smallest;
}