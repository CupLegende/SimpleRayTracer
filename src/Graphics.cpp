#include <SFML/Graphics.hpp>
#include "Graphics.h"
Graphics::Graphics()
{
	rw = new sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");
}


Graphics::~Graphics()
{
}

void Graphics::add(RenderPixel* rd) {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(rd->x, rd->y);
	vertex.color = sf::Color(rd->r, rd->g, rd->b);
	vertices.push_back(vertex);
	delete rd;
}

void Graphics::draw() {
	//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	while (rw->isOpen())
	{
		sf::Event event;
		while (rw->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				rw->close();
		}

		rw->clear();

		rw->draw(&(vertices[0]), vertices.size(), sf::Points);
		
		
		rw->display();
	}
}
