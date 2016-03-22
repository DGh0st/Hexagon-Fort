#include "FlatHexagonalGrid.h"

FlatHexagonalGrid::FlatHexagonalGrid(float length, float x, float y, int width, int height, sf::Color color):sideLength(length), startX(x), startY(y), sizeX(width), sizeY(height), color(color){
	radius = sqrt(pow(sideLength, 2) - (pow((sideLength / 2), 2)));
	createGrid(color);
}

FlatHexagonalGrid::~FlatHexagonalGrid() {
	// nothing to destruct as of now...
}

void FlatHexagonalGrid::operator=(const FlatHexagonalGrid & other) {
	sideLength = other.sideLength;
	radius = other.radius;
	startX = other.startX;
	startY = other.startY;
	sizeX = other.sizeX;
	sizeY = other.sizeY;
	grid = other.grid;
	color = other.color;
}

void FlatHexagonalGrid::createGrid(sf::Color color) {
	// creates a sizeX by sizeY grid of hexagons
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			sf::ConvexShape polygon(6);
			calculateVertices(polygon, i, j);
			if (j < sizeX / 2) {
				polygon.setFillColor(sf::Color(1, 0, 0)); // red side
			}
			else {
				polygon.setFillColor(sf::Color(0, 0, 1)); // blue side
			}
			polygon.setOutlineColor(sf::Color::Color(205, 205, 205));
			polygon.setOutlineThickness(1);
			grid.insert(grid.end(), polygon);
		}
	}
}

void FlatHexagonalGrid::calculateVertices(sf::ConvexShape & polygon, int i, int j) {
	// calculates the vertices for a flat hexagon (sf::CircleShape with 6 sides gives same results)
	float X, Y;
	X = startX + j * (3 * sideLength) / 2;
	Y = startY + (j % 2) * radius + 2 * i * radius;
	polygon.setPoint(0, sf::Vector2f(X + sideLength / 2, Y - radius));
	polygon.setPoint(1, sf::Vector2f(X + sideLength, Y));
	polygon.setPoint(2, sf::Vector2f(X + sideLength / 2, Y + radius));
	polygon.setPoint(3, sf::Vector2f(X - sideLength / 2, Y + radius));
	polygon.setPoint(4, sf::Vector2f(X - sideLength, Y));
	polygon.setPoint(5, sf::Vector2f(X - sideLength / 2, Y - radius));
}

void FlatHexagonalGrid::draw(sf::RenderWindow & window) {
	for (int i = 0; i < grid.size(); i++) {
		window.draw(grid[i]);
	}
}

Hexagon FlatHexagonalGrid::getHexagon(int x, int y) {
	Hexagon result;
	for (int i = 0; i < grid.size(); i++) {
		if (isInHexagon(grid[i], (float)x, (float)y)) { // find correct hexagon based on (x, y)
			result.hex = &grid[i];
			result.centerX = result.hex->getGlobalBounds().left + sideLength;
			result.centerY = result.hex->getGlobalBounds().top + radius;
			if (result.hex->getFillColor() + sf::Color(1, 0, 0) == sf::Color(2, 0, 0) || result.hex->getFillColor() + sf::Color(0, 0, 1) == sf::Color(0, 0, 2) || result.hex->getFillColor() == sf::Color(142, 122, 140)) {
				result.isOccupied = false;
			}
			else {
				result.isOccupied = true;
			}
			return result;
		}
	}
	// did not find it return NULL with (-1, -1) as center
	result.hex = NULL;
	result.centerX = -1;
	result.centerY = -1;
	result.isOccupied = true;
	return result;
}

bool FlatHexagonalGrid::isInHexagon(sf::ConvexShape hexagon, float x, float y) {
	if (!hexagon.getGlobalBounds().contains(sf::Vector2f(x, y))) { // if it is not in the rectangle (global bounds) than we should not waste more time
		return false;
	}
	bool result = false;
	for (int i = 0, j = (int)hexagon.getPointCount() - 1; i < hexagon.getPointCount(); j = i++) {
		if (((hexagon.getPoint(i).y > y) != (hexagon.getPoint(j).y > y)) &&
			(x < (hexagon.getPoint(j).x - hexagon.getPoint(i).x) * (y - hexagon.getPoint(i).y) / (hexagon.getPoint(j).y - hexagon.getPoint(i).y) + hexagon.getPoint(i).x)) {
			result = !result;
		}
	}
	return result;
}

bool FlatHexagonalGrid::setColor(Hexagon & hexagon, sf::Color color) {
	if (hexagon.hex != NULL) {
		hexagon.hex->setFillColor(sf::Color(color.r, color.g, color.b));
		if (color == sf::Color::Color(142, 122, 140)) {
			hexagon.hex->setOutlineThickness(0); // get rid of the outline on the minion path
		}
		return hexagon.isOccupied = true;
	}
	else {
		return false;
	}
}