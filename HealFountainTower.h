#ifndef HEALFOUNTAIN_H
#define HEALFOUNTAIN_H

#include <SFML/Graphics.hpp>
#include "FlatHexagonalGrid.h"
#include "Minion.h"
#include "Tower.h"

class HealFountainTower : public Tower {
public:
	// constructor that creates the tower
	// @param hex: the hexagon on which to build the tower
	// @param posDirection: the tower does not attack these minions
	// @param attackRadius: the attack radius of the tower
	// @param attackSpeed: the attack speed of the tower in milliseconds
	// @param damage: the damage the tower does to opponent minions
	// @param maxHealth: the health that the tower starts with
	// @param color: the color of the team/tower
	HealFountainTower(Hexagon hex = Hexagon(), bool posDirection = true, float attackRadius = 50.0f, float attackSpeed = 300.0f, int damage = 80, int maxHealth = 100, sf::Color color = sf::Color::Red, int sprayBullets = 12);
	// destructor
	~HealFountainTower();
	// equal operator
	// @param other: the other RoundRobinTower object to copy from
	void operator=(const HealFountainTower & other);
	// attack the minions that are passed in
	// @param minion: the minion to attack
	void attack(std::vector<Minion> & minion, sf::RenderWindow & window);
private:
	int currentBullet; // the current bullet it is on
};

#endif