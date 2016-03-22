#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include "FlatHexagonalGrid.h"
#include "Minion.h"

struct Upgrades {
	float attackRadius = 0.0f;
	float attackSpeed = 10000.0f;
	int damage = 0;
	int sprayBullets = 1;
	std::string classType = "Tower";
};

class Tower {
public:
	// constructor that creates the tower
	// @param hex: the hexagon on which to build the tower
	// @param posDirection: the tower does not attack these minions
	// @param attackRadius: the attack radius of the tower
	// @param attackSpeed: the attack speed of the tower in milliseconds
	// @param damage: the damage the tower does to opponent minions
	// @param maxHealth: the health that the tower starts with
	// @param color: the color of the team/tower
	Tower(Hexagon hex = Hexagon(), bool posDirection = true, float attackRadius = 100.0f, float attackSpeed = 400.0f, int damage = 20, int maxHealth = 100, sf::Color color = sf::Color::Red, int sprayBullets = 1);
	// destructor
	~Tower();
	// equal operator
	// @param other: the other Tower object to copy from
	void operator=(const Tower & other);
	// draw the tower using sfml
	// @param window: the window to draw the tower into
	void draw(sf::RenderWindow & window);
	// move the bullets
	void moveBullets();
	// attack the minion that is passed in
	// @param minion: the minion to attack
	void attack(std::vector<Minion> & minion, sf::RenderWindow & window);
	// get tower's position
	sf::Vector2f getCenter();

	static std::vector<Upgrades> availableUpgrades; // all available upgrades
	float radius; // attack radius of the tower
	float frequency; // attack speed of the tower
	int damage; // damage of tower
	sf::Color colorOfHexagon; // color of the hexagon that the tower is on top of
	int bulletsPerAttack;
	std::string towerType = "Tower";
protected:
	sf::ConvexShape tower; // shape of the tower
	sf::CircleShape towerRange; // shape of the tower range
	std::vector<sf::CircleShape> bullets; // bullets that the tower shoots out
	std::vector<sf::Vector2f> bulletsSpeed; // speed vector of the bullets
	bool posDirection; // tower does not attack the minion with this direction
	sf::Clock bulletTimer; // timer to track when to shoot another bullet
	int health; // health of tower
	sf::Vector2f center;
	// returns true if the line ab collides with line cd
	// @param a: current position of the bullet
	// @param b: position of bullet in the next frame
	// @param c: current position of the minion
	// @param d: position of the minion in the next frame
	bool isIntersecting(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d);
	// returns true if a is in tower's range
	// @param a: point that needs to be in range
	bool isInRange(sf::Vector2f a);
};

#endif