
#include "Top.h"

#include <iostream>
#include <string>
using namespace std;

int main() {

	GameManager* GM = new GameManager();

	GM->addPlayer("rumeysa");
	GM->addPlayer("serdar");
	GM->addPlayer("geyOsman");

	GM->startGame();


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