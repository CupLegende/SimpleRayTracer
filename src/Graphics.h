#pragma once
#include <SFML/Graphics.hpp>

struct RenderPixel {
	int x;
	int y;
	int r;
	int g;
	int b;
};

class Graphics
{
public:
	Graphics();
	~Graphics();
	sf::RenderWindow* rw;
	std::vector<sf::Vertex> vertices;
	//sf::Vertex vertices[][];
	void add(RenderPixel* rd);
	void draw();
};

