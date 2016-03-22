#include "Minion.h"
#include "GameManager.h"

Minion::Minion(float radius, int attackRadius, bool posDirection, sf::Color color, int damage, int maxHealth, float speed, sf::RenderWindow & window): attackRadius(attackRadius),posDirection(posDirection), damage(damage), health(maxHealth), maxHealth(maxHealth), speed(speed) {
	minion = sf::CircleShape(radius, 100);
	minion.setFillColor(color);
	minion.setOrigin(radius, radius);
	if (posDirection) { // set texture to red_minion.png
		counterInPath = 0;
		minion.setPosition((sf::Vector2f)path[counterInPath]);
		movement = sf::Vector2f((path[counterInPath + 1].x - minion.getPosition().x) * speed, (path[counterInPath + 1].y - minion.getPosition().y) * speed);
		minionTexture = sf::Sprite(GameManager::redMinionTexture);
	}
	else { // set texture to blue_minion.png
		counterInPath = (int)path.size() - 1;
		minion.setPosition((sf::Vector2f)path[counterInPath]);
		movement = sf::Vector2f((path[counterInPath - 1].x - minion.getPosition().x) * speed, (path[counterInPath - 1].y - minion.getPosition().y) * speed);
		minionTexture = sf::Sprite(GameManager::blueMinionTexture);
	}
	minionTexture.setScale(0.15f, 0.15f);
	minionTexture.setOrigin(minionTexture.getLocalBounds().width / 2, minionTexture.getLocalBounds().height / 2);
	// display health bar on top of minion
	healthBar = sf::RectangleShape(sf::Vector2f(minion.getGlobalBounds().width, minion.getGlobalBounds().height / 5.0f));
	healthBar.setOrigin(healthBar.getGlobalBounds().width / 2, healthBar.getGlobalBounds().height / 2);
	healthBar.setFillColor(sf::Color::Green);
	// health bar background
	healthBarBackground = sf::RectangleShape(sf::Vector2f(minion.getGlobalBounds().width, minion.getGlobalBounds().height / 5.0f));
	healthBarBackground.setOrigin(healthBarBackground.getGlobalBounds().width / 2, healthBarBackground.getGlobalBounds().height / 2);
	healthBarBackground.setFillColor(sf::Color::Red);
	healthBarBackground.setOutlineColor(sf::Color::Black);
	healthBarBackground.setOutlineThickness(1);
	state = 0;
}

Minion::~Minion() {
	// nothing to delete as of yet ...
}

void Minion::operator=(const Minion & other) {
	minion = other.minion;
	minionTexture = other.minionTexture;
	posDirection = other.posDirection;
	counterInPath = other.counterInPath;
	movement = other.movement;
	attackRadius = other.attackRadius;
	damage = other.damage;
	health = other.health;
	maxHealth = other.maxHealth;
	speed = other.speed;
	state = other.state;
	fightingTimer = other.fightingTimer;
	fightingResetTime = other.fightingResetTime;
	waitingTimer = other.waitingTimer;
	waitingResetTime = other.waitingResetTime;
	healthBar = other.healthBar;
	healthBarBackground = other.healthBarBackground;
}

void Minion::draw(sf::RenderWindow & window) {
	// window.draw(minion);
	window.draw(healthBarBackground);
	window.draw(healthBar);
	window.draw(minionTexture);
	for (int i = 0; i < effects.size(); i++) {
		window.draw(effects[i]);
	}
}

void Minion::move() {
	if (posDirection) {
		minion.move(movement);
		if (std::pow(std::abs(path[counterInPath + 1].x - minion.getPosition().x), 2) + std::pow(std::abs(path[counterInPath + 1].y - minion.getPosition().y), 2) < 2.0f) {
			if (++counterInPath == path.size() - 1) { // attack base
				counterInPath--;
				minion.setPosition((sf::Vector2f)path[counterInPath]);
				state = 3;
			}
			movement = sf::Vector2f((path[counterInPath + 1].x - minion.getPosition().x) * speed, (path[counterInPath + 1].y - minion.getPosition().y) * speed);
		}
	}
	else {
		minion.move(movement);
		if (std::pow(std::abs(path[counterInPath - 1].x - minion.getPosition().x), 2) + std::pow(std::abs(path[counterInPath - 1].y - minion.getPosition().y), 2) < 2.0f) {
			if (--counterInPath == 0) { // attack base
				counterInPath++;
				minion.setPosition((sf::Vector2f)path[counterInPath]);
				state = 3;
			}
			movement = sf::Vector2f((path[counterInPath - 1].x - minion.getPosition().x) * speed, (path[counterInPath - 1].y - minion.getPosition().y) * speed);
		}
	}
	if ((state == 2 && fightingTimer.getElapsedTime().asSeconds() > fightingResetTime) || (state == 1 && waitingTimer.getElapsedTime().asSeconds() > waitingResetTime)) {
		state = 0; // reset minion state if not fighting or waiting
	}
	minionTexture.setPosition(minion.getPosition().x, minion.getPosition().y);
	minionTexture.setRotation(std::atan2(movement.y, movement.x) * 180.0f / 3.1415f + 90.0f);
	healthBar.setPosition(minion.getPosition() + sf::Vector2f(0.0f, 0 - minion.getGlobalBounds().height));
	healthBar.setScale(health / (float) maxHealth, 1.0f); // update health bar (visual)
	healthBarBackground.setPosition(minion.getPosition() + sf::Vector2f(0.0f, 0 - minion.getGlobalBounds().height));
	for (int i = 0; i < effects.size(); i++) {
		effects[i].setPosition(effects[i].getPosition() - sf::Vector2f(0, effects[i].getGlobalBounds().height / 4));
		effects[i].setColor(effects[i].getColor() - sf::Color(0, 0, 0, 16));
		if (effects[i].getColor().a <= 0) {
			effects.erase(effects.begin() + i);
		}
	}
}

bool Minion::shouldFightOrWait(Minion & other) {
	if (posDirection != other.posDirection && std::pow(std::abs(minion.getGlobalBounds().top - other.minion.getGlobalBounds().top), 2) + std::pow(std::abs(minion.getGlobalBounds().left - other.minion.getGlobalBounds().left), 2) < std::pow(minion.getGlobalBounds().height, 2) + std::pow(minion.getGlobalBounds().width, 2)) {
		// enemy ahead so fight
		fightingResetTime = fightingTimer.getElapsedTime().asSeconds() * 1.5f;
		other.fightingResetTime = other.fightingTimer.getElapsedTime().asSeconds() * 1.5f;
		fightingTimer.restart();
		other.fightingTimer.restart();
		minion.setPosition((sf::Vector2f)path[counterInPath]);
		other.minion.setPosition((sf::Vector2f)other.path[other.counterInPath]);
		if (posDirection) {
			movement = sf::Vector2f((path[counterInPath + 1].x - minion.getPosition().x) * speed, (path[counterInPath + 1].y - minion.getPosition().y) * speed);
			other.movement = sf::Vector2f((other.path[other.counterInPath - 1].x - other.minion.getPosition().x) * other.speed, (other.path[other.counterInPath - 1].y - other.minion.getPosition().y) * other.speed);
		}
		else {
			movement = sf::Vector2f((path[counterInPath - 1].x - minion.getPosition().x) * speed, (path[counterInPath - 1].y - minion.getPosition().y) * speed);
			other.movement = sf::Vector2f((other.path[other.counterInPath + 1].x - other.minion.getPosition().x) * other.speed, (other.path[other.counterInPath + 1].y - other.minion.getPosition().y) * other.speed);
		}
		// both lose health
		reduceHealth(other.damage);
		other.reduceHealth(damage);
		state = other.state = 2;
		return true;
	}
	else if (posDirection == other.posDirection && (posDirection && counterInPath == other.counterInPath - 1 || !posDirection && counterInPath == other.counterInPath + 1)) {
		// teammate ahead so wait
		minion.setPosition((sf::Vector2f)path[counterInPath]);
		waitingResetTime = waitingTimer.getElapsedTime().asSeconds() * 1.5f;
		waitingTimer.restart();
		if (posDirection) {
			movement = sf::Vector2f((path[counterInPath + 1].x - minion.getPosition().x) * speed, (path[counterInPath + 1].y - minion.getPosition().y) * speed);
		}
		else {
			movement = sf::Vector2f((path[counterInPath - 1].x - minion.getPosition().x) * speed, (path[counterInPath - 1].y - minion.getPosition().y) * speed);
		}
		state = 1;
		return true;
	}
	return false;
}

bool Minion::isDead() {
	return health <= 0;
}

sf::Vector2f Minion::getPosition() {
	return sf::Vector2f(minion.getGlobalBounds().left + minion.getGlobalBounds().width / 2, minion.getGlobalBounds().top + minion.getGlobalBounds().height / 2);
}

void Minion::reduceHealth(int damageTaken) {
	health -= damageTaken;
	if (health > maxHealth) {
		health = maxHealth;
	}
	// display gaining health text with animation
	if (damageTaken < 0) {
		sf::Text gainHealth;
		gainHealth.setFont(GameManager::blkchcry);
		gainHealth.setCharacterSize(21);
		gainHealth.setString("+ " + std::to_string(std::abs(damageTaken)));
		gainHealth.setOrigin(gainHealth.getGlobalBounds().width / 2, gainHealth.getGlobalBounds().height / 2);
		gainHealth.setColor(sf::Color(0, 178, 0));
		gainHealth.setPosition(healthBar.getPosition() - sf::Vector2f(0, healthBar.getGlobalBounds().height + gainHealth.getGlobalBounds().height));
		effects.push_back(gainHealth);
	}
}

sf::Vector2f Minion::getSize() {
	return sf::Vector2f(minion.getGlobalBounds().width, minion.getGlobalBounds().height);
}

sf::Vector2f Minion::getVelocity() {
	return movement;
}

float Minion::getBodyRadius() {
	return minion.getRadius();
}