
#include "Top.h"

#include <iostream>
#include <string>
using namespace std;

int main() {

	GameManager* GM = new GameManager();

	GM->createProvince("ankara", "");
	GM->createProvince("istanbul", "");
	GM->createProvince("konya", "");
	GM->createProvince("antalya", "");
	GM->createProvince("eskisehir", "");
	GM->createProvince("edirne", "");
	GM->createProvince("kars", "");
	GM->createProvince("aksaray", "");

	GM->createNeighbor("ankara", "konya");
	GM->createNeighbor("ankara", "eskisehir");
	GM->createNeighbor("konya", "antalya");
	GM->createNeighbor("aksaray", "konya");
	GM->createNeighbor("istanbul", "edirne");

	GM->addPlayer("ahmet");
	GM->addPlayer("serdar");

	GM->placeSoldier("ahmet", "aksaray", 1);
	GM->placeSoldier("ahmet", "ankara", 2);
	GM->placeSoldier("ahmet", "konya", 4);
	GM->placeSoldier("ahmet", "istanbul", 1);
	GM->placeSoldier("serdar", "kars", 1);
	GM->placeSoldier("serdar", "antalya", 4);
	GM->placeSoldier("serdar", "eskisehir", 2);
	GM->placeSoldier("serdar", "edirne", 3);

	GM->fortify("serdar", "eskisehir", "ankara", 1);

	GM->roleDice(3, 2);

	//GM->showWorldStatus();

	system("PAUSE");

	return 0;
}