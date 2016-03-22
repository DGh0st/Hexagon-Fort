#ifndef AI_H
#define AI_H

#include <SFML/Graphics.hpp>
#include "Tower.h"

class AI {
public:
	// constructor
	// @param team: team that this AI controls
	AI(bool team = true);
	// destructor
	~AI();
	// equal operator
	// @param other: the other AI object to copy from
	void operator=(const AI & other);
	// determine mode of AI based on gold
	// @param window: the window in which the game is running
	// @param gold: current gold that belongs to this team
	// @param towers:: towers that belong to this team
	void updateMode(sf::RenderWindow & window, int gold, std::vector<Tower *> towers);
private:
	bool team; // true for red and false for blue
	std::map<sf::Vector2f, sf::Vector2i> closestMinionPathPoint;

	void sendUpgradeRequest(std::vector<Tower *> towers); // send upgrade request to GameManager class
	void sendBuildRequest(sf::RenderWindow & window); // send build request to GameManager class
	sf::Vector2i closestMinionPath(int x, int y); // finds the closest coordinate that is in minion path
};

#endif
