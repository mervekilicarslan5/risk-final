#include <SFML\Graphics.hpp>
#include "WorldMap.h"
#include <iostream>
#include <string>
int main() {
	/*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Red);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}*/

	/*Province* p1 = new Province("Ankara", "");
	Province* p2 = new Province("Ýstanbul", "");
	Province* p3 = new Province("Konya", "");
	Province* p4 = new Province("Antalya", "");
	Province* p5 = new Province("Eskiþehir", "");
	Province* p6 = new Province("Edirne", "");
	Province* p7 = new Province("Kars", "");

	WorldMap* worldMap = new WorldMap();

	worldMap->addProvince(p1);
	worldMap->addProvince(p2);
	worldMap->addProvince(p3);
	worldMap->addProvince(p4);
	worldMap->addProvince(p5);
	worldMap->addProvince(p6);
	worldMap->addProvince(p7);

	worldMap->addNeighbor(p1, p2);
	worldMap->addNeighbor(p2, p3);
	worldMap->addNeighbor(p3, p4);
	worldMap->addNeighbor(p4, p5);
	worldMap->addNeighbor(p6, p5);
	worldMap->addNeighbor(p7, p6);*/
	
	/*Player* player1 = new Player("Ahmet", 1);
	Player* player2 = new Player("GeyOsman", 2);

	player1->captureProvince(worldMap, p1);
	player1->captureProvince(worldMap, p2);
	player1->captureProvince(worldMap, p3);
	player1->captureProvince(worldMap, p4);
	player1->captureProvince(worldMap, p5);
	player1->captureProvince(worldMap, p6);
	player1->captureProvince(worldMap, p7);

	cout << worldMap->hasPath(player1, p1, p7) << endl;
*/
	system("PAUSE");

	return 0;
}