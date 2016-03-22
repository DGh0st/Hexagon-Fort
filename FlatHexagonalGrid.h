#ifndef FLATHEXAGONALGRID_H
#define FLATHEXAGONALGRID_H

#include <SFML/Graphics.hpp>

struct Hexagon {
	float centerX = 0.0f; // the center x of the hexagon
	float centerY = 0.0f; // the center y of the hexagon
	bool isOccupied = false; // it is occupied if color of hexagon is not the same as original color
	sf::ConvexShape *hex = NULL; // the pointer to the hexagon shape that is drawn
};

class FlatHexagonalGrid {
public:
	// constructor
	// @param pointy: true for pointy hexagon and false for flat hexagon
	// @param length: the length of the edges of the hexagon
	// @param x: x-position to start drawing grid at
	// @param y: y-position to start drawing grid at
	// @param width: the amount of hexagons in the x-axis
	// @param height: the amount of hexagons in the y-axis
	// @param fillColor: the color to fill the hexagons with
	FlatHexagonalGrid(float length = 40.0f, float x = 0.0f, float y = 0.0f, int width = 1, int heigth = 1, sf::Color fillColor = sf::Color(142, 122, 140));
	// destructor
	~FlatHexagonalGrid();
	// equal operator
	// @param other: the other HexagonGrid object to copy from
	void operator=(const FlatHexagonalGrid & other);
	// draw the hexagonal grid using sfml
	// @param window: the window to draw the grid into
	void draw(sf::RenderWindow & window);
	// get the grid position based on the pixel coordinates
	// @param x: the x-coordinate of the pixel
	// @param y: the y-coordinate of the pixel
	Hexagon getHexagon(int x, int y);
	// set the color of the hexagon and set the occupied to true
	// @param hexagon: the hexagon that you want to change the color of
	// @param color: the color to set the hexagon to
	bool setColor(Hexagon & hexagon, sf::Color color);

	float sideLength; // length of the edges
	float radius; // the small radius from the center of the hexagon
private:
	float startX, startY; // the starting x and y coordinates of the hexagon grid
	int sizeX, sizeY; // the width and the height of the hexagon grid
	std::vector<sf::ConvexShape> grid; // vector of hexagons

	sf::ConvexShape colorshape; // the shape to color on hover
	sf::Color color; // the fill color
	
	// initialize the grid
	void createGrid(sf::Color color);
	// checks if the point (x, y) is in the hexagon
	bool isInHexagon(sf::ConvexShape hexagon, float x, float y);
	// calcuate the vertices for the hexagon based on pointy or flat
	// @param polygon: to set the vertices for
	// @param i: ith hexagon in the grid
	// @param j: jth hexagon in the grid
	void calculateVertices(sf::ConvexShape & polygon, int i, int j);
};

#endif