#ifndef MINION_H
#define MINION_H

#include <SFML/Graphics.hpp>

class Minion {
public:
	// constructor that creates a minion
	// @param radius: the size/radius of the minion
	// @param attackRadius: the attackradius of the minions
	// @param posDirection: true if minion starts from left side and goes to the right side
	// @param color: the color of the minion
	// @param damage: the damage the minion does
	// @param maxHealth: the healt the minion starts of with
	// @param speed: the speed the minion moves at (should be less than 1)
	Minion(float radius = 5.0f, int attackRadius = 1, bool posDirection = true, sf::Color color = sf::Color::Red, int damage = 10, int maxHealth = 100, float speed = 0.1f, sf::RenderWindow & window = sf::RenderWindow());
	// destructor
	~Minion();
	// equal operator
	// @param other: the other Minion object to copy from
	void operator=(const Minion & other);
	// draw the minion using sfml
	// @param window: the window to draw the minion into
	void draw(sf::RenderWindow & window);
	// move the minion
	void move();
	// return true if two minions should fight
	// @param other: the other Minion object to check collision with
	bool shouldFightOrWait(Minion & other);
	// return true if minion is dead
	bool isDead();
	// get the position of the minion
	sf::Vector2f getPosition();
	// get the size of the minion
	sf::Vector2f getSize();
	// get the speed vectors
	sf::Vector2f getVelocity();
	// get the minion's body radius
	float getBodyRadius();
	// reduce the health of minion
	// @param damage: the damage it does to the minion
	void reduceHealth(int damageTaken);

	static std::vector<sf::Vector2i> path; // path that the minion follows

	int counterInPath; // current step in the path
	int damage; // damage of minion
	int health; // health of minion
	int maxHealth; // maximum health the minion can have
	int attackRadius; // attackRadius of minion
	float speed; // velocity of the minion
	int state; // 0 if is normal movement, 1 if is waiting, 2 if fighting, 3 is attacking team base
private:
	sf::CircleShape minion; // shape of the minion
	sf::Sprite minionTexture; // sprite to display texture
	bool posDirection; // direction that minion follows on the path
	sf::Vector2f movement; // speed vector of the minion
	sf::Clock fightingTimer; // timer to reset isFighting
	float fightingResetTime; // time needed to pass by before state is reseted to 0 from 2
	sf::Clock waitingTimer; // timer to reset isWaiting
	float waitingResetTime; // time needed to pass by before state is reseted to 0 from 1
	sf::RectangleShape healthBar; // health bar rectange
	sf::RectangleShape healthBarBackground; // health bar background rectange
	std::vector<sf::Text> effects; // effects that are displayed on top of minions
};

#endif