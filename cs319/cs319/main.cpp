
#include "Top.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <conio.h>

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
	UdpSocket socket;
	char connectionType, mode;

	map<unsigned short, IpAddress> computerID;
	string text = " asd";
	Packet packet;
	
	cout << ip << endl;

	cout << "(s) for server, (c) for client: ";
	cin >> connectionType;

	unsigned short port;

	cout << "Set port number: ";
	cin >> port;

	if (socket.bind(port) != Socket::Done) {
		cout << "Couldnt binded. ";
	}
	else {
		cout << "BINDED !! " << endl;
	}

	//socket.setBlocking(false);

	if (connectionType == 's') {
		char answer = 'b';
		do {
			IpAddress rIP;
			unsigned short port ;
			if (socket.receive(packet, rIP, port) == Socket::Done) {
				computerID[port] = rIP;
			}
			cout << "Enter to continue: (press a to continue)";
			cin >> answer;
		} while (answer != 'a');
	}

	else if (connectionType == 'c') {
		string sIp;
		cout << "Enter server ip: ";
		cin >> sIp;
		//sIp = "139.179.210.187";
		IpAddress sendIP(sIp);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "Socket SENT!!" << endl;
		cout << sIp << endl;
	}

	bool done = false;
	while (!done) {
		if (connectionType == 's') {
			getline(cin, text);
			Packet packet;
			packet << text;
			map<unsigned short, IpAddress> ::iterator tempIterator;
			for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
				if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done)  {} // the socket send or not 
				
		}
		else if (connectionType == 'c') {
			IpAddress tempId;
			unsigned short tempPort;
			if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
				string receivedText;
				packet >> receivedText;
				cout << "Received: " << receivedText << endl;
			}

		}
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