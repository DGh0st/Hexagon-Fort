#ifndef ROUNDROBINTOWER_H
#define ROUNDROBINTOWER_H

#include <SFML/Graphics.hpp>
#include "FlatHexagonalGrid.h"
#include "Minion.h"
#include "Tower.h"

class RoundRobinTower : public Tower {
public:
	// constructor that creates the tower
	// @param hex: the hexagon on which to build the tower
	// @param posDirection: the tower does not attack these minions
	// @param attackRadius: the attack radius of the tower
	// @param attackSpeed: the attack speed of the tower in milliseconds
	// @param damage: the damage the tower does to opponent minions
	// @param maxHealth: the health that the tower starts with
	// @param color: the color of the team/tower
	RoundRobinTower(Hexagon hex = Hexagon(), bool posDirection = true, float attackRadius = 50.0f, float attackSpeed = 300.0f, int damage = 15, int maxHealth = 100, sf::Color color = sf::Color::Red, int sprayBullets = 3);
	// destructor
	~RoundRobinTower();
	// equal operator
	// @param other: the other RoundRobinTower object to copy from
	void operator=(const RoundRobinTower & other);
	// attack the minions that are passed in
	// @param minion: the minion to attack
	void attack(std::vector<Minion> & minion, sf::RenderWindow & window);
private:

};

#endif