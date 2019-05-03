#pragma once
#include "Continent.h"
#include "Die.h"
#include <vector>

class GameManager
{
public:

	GameManager();
	~GameManager();

	/*void addPlayer(string _name);
	bool placeSoldier(Player* player, Province* province, int amount);
	bool fortify(Player* player, Province* from, Province * to, int amount);
*/
private:
	vector<Player*> players;
	WorldMap* worldMap;
	Die* die;
};

