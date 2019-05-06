
#include "Top.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace sf;


int main() {

	//GameManager* GM = new GameManager();

	//GM->addPlayer("rumeysa");
	//GM->addPlayer("serdar");
	//GM->addPlayer("geyOsman");

	//GM->startGame();

	IpAddress ip = IpAddress::getLocalAddress();
	TcpSocket socket;
	char connectionType;

	cout << "(s) for server, (c) for client: ";
	cin >> connectionType;

	if (connectionType == 's') {
		TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
	}
	else
		socket.connect(ip, 2000);

	RectangleShape rect1, rect2;

	rect1.setSize(Vector2f(20, 20));
	rect2.setSize(Vector2f(20, 20));
	
	rect1.setFillColor(Color::Red);
	rect2.setFillColor(Color::Blue);

	RenderWindow Window(VideoMode(800, 600, 32), "Packets");

	Vector2f prevPosition, p2Position;

	socket.setBlocking(false);

	bool update = false;

	while (Window.isOpen()) {

		Event Event;
		while (Window.pollEvent(Event)) {
			if (Event.type == Event::Closed || Event.key.code == Keyboard::Escape)
				Window.close();
			else if (Event.type == Event::GainedFocus)
				update = true;
			else if (Event.type == Event::LostFocus)
				update = false;

		}
	
		prevPosition = rect1.getPosition();

		if (update) {
			if (Keyboard::isKeyPressed(Keyboard::Right))
				rect1.move(1.0f, 0.0f);
			else if (Keyboard::isKeyPressed(Keyboard::Left))
				rect1.move(-1.0f, 0.0f);
			else if (Keyboard::isKeyPressed(Keyboard::Up))
				rect1.move(0.0f, -1.0f);
			else if (Keyboard::isKeyPressed(Keyboard::Down))
				rect1.move(0.0f, 1.0f);
		}

		Packet packet;

		if (prevPosition != rect1.getPosition()) {
			packet << rect1.getPosition().x << rect1.getPosition().y;
			socket.send(packet);
		}

		socket.receive(packet);
		if (packet >> p2Position.x >> p2Position.y)
			rect2.setPosition(p2Position);

		Window.draw(rect1);
		Window.draw(rect2);

		Window.display();
		Window.clear();

	}



	/*GM->placeSoldier("ahmet", "aksaray", 1);
	GM->placeSoldier("ahmet", "ankara", 2);
	GM->placeSoldier("ahmet", "konya", 4);
	GM->placeSoldier("ahmet", "istanbul", 1);
	GM->placeSoldier("serdar", "kars", 1);
	GM->placeSoldier("serdar", "antalya", 2);
	GM->placeSoldier("serdar", "eskisehir", 2);
	GM->placeSoldier("serdar", "edirne", 3);*/


	//GM->fortify("serdar", "eskisehir", "ankara", 1);

	//GM->attack("ahmet", "ahmet", "konya", "ankara", 3);


	//GM->showWorldStatus();

	system("PAUSE");

	return 0;
}