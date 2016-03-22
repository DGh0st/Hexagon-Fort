#include "HealFountainTower.h"
#include "GameManager.h"

HealFountainTower::HealFountainTower(Hexagon hex, bool posDirection, float attackRadius, float attackSpeed, int damage, int maxHealth, sf::Color color, int sprayBullets) : Tower(hex, posDirection, attackRadius, attackSpeed, damage, maxHealth, color, sprayBullets) {
	towerType = "HealFountainTower";
	tower.setTexture(&sf::Texture());
	if (posDirection) { // red_tower2.png texture
		tower.setTexture(&GameManager::redTowerTexture3);
	}
	else { // blue_tower2.png texture
		tower.setTexture(&GameManager::blueTowerTexture3);
	}
	currentBullet = 0;
}

HealFountainTower::~HealFountainTower() {
	// nothing to delete as of yet ...
}

void HealFountainTower::operator=(const HealFountainTower & other) {
	Tower::operator=(other);
	currentBullet = other.currentBullet;
}

void HealFountainTower::attack(std::vector<Minion> & minion, sf::RenderWindow & window) {
	// update tower range (display)
	towerRange = sf::CircleShape(radius, 100);
	towerRange.setFillColor(sf::Color(colorOfHexagon.r / 3, colorOfHexagon.g / 3, colorOfHexagon.b / 3, colorOfHexagon.a / 3));
	towerRange.setOutlineColor(colorOfHexagon);
	towerRange.setOutlineThickness(1.0f);
	towerRange.setOrigin(radius, radius);
	towerRange.setPosition(tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2, tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2);

	for (int j = (int)minion.size() - 1; j >= 0; j--) {
		if (isInRange(minion[j].getPosition())) { // check if in range so that we dont waste time with checking bullet collision later
			if (GameManager::areTimersViable && bulletTimer.getElapsedTime().asMilliseconds() > frequency) {
				sf::CircleShape bullet(minion[j].getSize().x / minion[j].getBodyRadius(), 100);
				bullet.setOrigin(bullet.getGlobalBounds().width / 2, bullet.getGlobalBounds().height / 2);
				bullet.setPosition(sf::Vector2f(tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2, tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2));
				bullet.setScale(5.0f, 5.0f);
				bullet.setTexture(&GameManager::healingballTexture);
				bullets.push_back(bullet);
				bulletsSpeed.push_back(sf::Vector2f(std::cos(2 * 3.14159f * currentBullet / bulletsPerAttack) / radius * window.getSize().x, std::sin(2 * 3.14159f * currentBullet / bulletsPerAttack) / radius * window.getSize().y));
				currentBullet = (currentBullet + 1) % bulletsPerAttack;
				bulletTimer.restart();
			}
			for (int i = 0; i < bullets.size(); i++) { // check bullet collision with minions
				if (isIntersecting(bullets[i].getPosition(), bullets[i].getPosition() + bulletsSpeed[i], minion[j].getPosition(), minion[j].getPosition() + minion[j].getVelocity()) || bullets[i].getGlobalBounds().intersects(sf::FloatRect(minion[j].getPosition(), minion[j].getSize()))) {//std::pow(std::abs((bullets[i].getGlobalBounds().left + bullets[i].getGlobalBounds().width / 2) - minion[j].getPosition().x), 2) + std::pow(std::abs((bullets[i].getGlobalBounds().top + bullets[i].getGlobalBounds().height / 2) - minion[j].getPosition().y), 2) < std::pow(minion[j].getSize().x / 2, 2) + std::pow(minion[j].getSize().y / 2, 2)) {
					minion[j].reduceHealth(-damage);
					bullets.erase(bullets.begin() + i);
					bulletsSpeed.erase(bulletsSpeed.begin() + i);
				}
			}
		}
	}
}