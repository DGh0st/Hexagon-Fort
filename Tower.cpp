#include "Tower.h"
#include "GameManager.h"

Tower::Tower(Hexagon hex, bool posDirection, float attackRadius, float attackSpeed, int damage, int maxHealth, sf::Color color, int sprayBullets) :posDirection(posDirection), radius(attackRadius), frequency(attackSpeed), damage(damage), health(maxHealth), colorOfHexagon(color), bulletsPerAttack(sprayBullets) {
	if (hex.hex != NULL) {
		hex.hex->setFillColor(color); // set as occupied in map (grid)
		center = sf::Vector2f(hex.centerX, hex.centerY);
		tower = sf::ConvexShape(*hex.hex);
		tower.setPointCount(4);
		tower.setPoint(0, sf::Vector2f(hex.hex->getGlobalBounds().left, hex.hex->getGlobalBounds().top));
		tower.setPoint(1, sf::Vector2f(hex.hex->getGlobalBounds().left + hex.hex->getGlobalBounds().width, hex.hex->getGlobalBounds().top));
		tower.setPoint(2, sf::Vector2f(hex.hex->getGlobalBounds().left + hex.hex->getGlobalBounds().width, hex.hex->getGlobalBounds().top + hex.hex->getGlobalBounds().height));
		tower.setPoint(3, sf::Vector2f(hex.hex->getGlobalBounds().left, hex.hex->getGlobalBounds().top + hex.hex->getGlobalBounds().height));
		tower.setOutlineThickness(0);
	}
	else {
		center = sf::Vector2f(0.0f, 0.0f);
		tower = sf::ConvexShape(4);
	}
	towerType = "Tower";
	if (posDirection) { // red_tower.png texture
		tower.setTexture(&GameManager::redTowerTexture);
	}
	else { // blue_tower.png texture
		tower.setTexture(&GameManager::blueTowerTexture);
	}
}

Tower::~Tower() {
	// nothing to delete as of yet ...
}

void Tower::operator=(const Tower & other) {
	tower = other.tower;
	towerRange = other.towerRange;
	bullets = other.bullets;
	bulletsSpeed = other.bulletsSpeed;
	posDirection = other.posDirection;
	radius = other.radius;
	frequency = other.frequency;
	bulletTimer = other.bulletTimer;
	damage = other.damage;
	health = other.health;
	center = other.center;
	colorOfHexagon = other.colorOfHexagon;
	bulletsPerAttack = other.bulletsPerAttack;
	towerType = other.towerType;
}

void Tower::draw(sf::RenderWindow & window) {
	if (tower.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
		window.draw(towerRange); // display tower range on hover
	}
	window.draw(tower);
	for (int i = 0; i < bullets.size(); i++) {
		if (!isInRange(bullets[i].getPosition())) { // remove bullets if out of tower range
			bullets.erase(bullets.begin() + i);
			bulletsSpeed.erase(bulletsSpeed.begin() + i);
		}
		else {
			window.draw(bullets[i]);
		}
	}
}

void Tower::moveBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].setRotation(std::atan2(bulletsSpeed[i].y, bulletsSpeed[i].x) * 180.0f / 3.1415f + 180.0f);
		bullets[i].move(bulletsSpeed[i]);
	}
}

void Tower::attack(std::vector<Minion> & minion, sf::RenderWindow & window) {
	// update tower range radius (display)
	towerRange = sf::CircleShape(radius, 100);
	towerRange.setFillColor(sf::Color(colorOfHexagon.r / 3, colorOfHexagon.g / 3, colorOfHexagon.b / 3, colorOfHexagon.a / 3));
	towerRange.setOutlineColor(colorOfHexagon);
	towerRange.setOutlineThickness(1.0f);
	towerRange.setOrigin(radius, radius);
	towerRange.setPosition(tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2, tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2);

	for (int j = (int)minion.size() - 1; j >= 0; j--) {
		if (isInRange(minion[j].getPosition())) { // check if in range so that we dont waste time with checking bullet collision later
			if (GameManager::areTimersViable && bulletTimer.getElapsedTime().asMilliseconds() > frequency && isInRange(minion[j].getPosition())) { // attack speed
				sf::CircleShape bullet(minion[j].getSize().x / minion[j].getBodyRadius(), 100);
				bullet.setOrigin(bullet.getGlobalBounds().width / 2, bullet.getGlobalBounds().height / 2);
				bullet.setPosition(sf::Vector2f(tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2, tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2));
				bullet.setScale(5.0f, 5.0f);
				if (posDirection) { // set texture based on posDirection
					bullet.setTexture(&GameManager::fireballTexture);
				}
				else {
					bullet.setTexture(&GameManager::iceballTexture);
				}
				bullets.push_back(bullet);
				if (minion[j].state != 2 && minion[j].state != 1) {
					if (!posDirection && minion[j].counterInPath < minion[j].path.size()) { // predict movement of red minion
						bulletsSpeed.push_back(sf::Vector2f((minion[j].getPosition().x + minion[j].getVelocity().x * minion[j].getSize().x / 3.0f - (tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2)) / radius * (window.getSize().x / 60.5f), (minion[j].getPosition().y + minion[minion.size() - 1].getVelocity().y * minion[minion.size() - 1].getSize().y / 3.0f - (tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2)) / radius * (window.getSize().y / 60.5f)));
					}
					else if (posDirection && minion[j].counterInPath > 0) { // predict movement of blue minion
						bulletsSpeed.push_back(sf::Vector2f((minion[j].getPosition().x + minion[j].getVelocity().x * minion[j].getSize().x / 3.0f - (tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2)) / radius * (window.getSize().x / 60.5f), (minion[j].getPosition().y + minion[j].getVelocity().y * minion[j].getSize().y / 3.0f - (tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2)) / radius * (window.getSize().y / 60.5f)));
					}
				}
				else { // no movement prediction
					bulletsSpeed.push_back(sf::Vector2f((minion[j].getPosition().x - (tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2)) / radius * (window.getSize().x / 60.5f), (minion[j].getPosition().y - (tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2)) / radius * (window.getSize().y / 60.5f)));
				}
				bulletTimer.restart();
			}
			for (int i = 0; i < bullets.size(); i++) { // check bullet collision with minions
				if (isIntersecting(bullets[i].getPosition(), bullets[i].getPosition() + bulletsSpeed[i], minion[j].getPosition(), minion[j].getPosition() + minion[j].getVelocity()) || bullets[i].getGlobalBounds().intersects(sf::FloatRect(minion[j].getPosition(), minion[j].getSize()))) {//std::pow(std::abs((bullets[i].getGlobalBounds().left + bullets[i].getGlobalBounds().width / 2) - minion[j].getPosition().x), 2) + std::pow(std::abs((bullets[i].getGlobalBounds().top + bullets[i].getGlobalBounds().height / 2) - minion[j].getPosition().y), 2) < std::pow(minion[j].getSize().x / 2, 2) + std::pow(minion[j].getSize().y / 2, 2)) {
					minion[j].reduceHealth(damage);
					bullets.erase(bullets.begin() + i);
					bulletsSpeed.erase(bulletsSpeed.begin() + i);
				}
			}
		}
	}
}

bool Tower::isIntersecting(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d) {
	// is minion path and bullet path intersecting
	float denom = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
	float num1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
	float num2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));
	if (denom == 0) {
		return  num1 == 0 && num1 == num2;
	}
	float r = num1 / denom;
	float s = num2 / denom;
	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

bool Tower::isInRange(sf::Vector2f a) {
	return std::pow(std::abs(a.x - (tower.getGlobalBounds().left + tower.getGlobalBounds().width / 2)), 2) + std::pow(std::abs(a.y - (tower.getGlobalBounds().top + tower.getGlobalBounds().height / 2)), 2) < std::pow(radius, 2);
}

sf::Vector2f Tower::getCenter() {
	return center;
}