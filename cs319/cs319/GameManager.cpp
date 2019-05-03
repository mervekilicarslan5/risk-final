//#include "GameManager.h"
//
//
//
//GameManager::GameManager()
//{
//	worldMap = new WorldMap();
//	die = new Die(6);
//}
//
//
//GameManager::~GameManager()
//{
//	delete worldMap;
//	delete die;
//}
//
////void GameManager::addPlayer(string _name)
////{
////	int numberOfPlayers = players.size();
////	if (numberOfPlayers < 6) {
////		players.push_back(new Player(_name, numberOfPlayers));
////	}
////}
//
////
////bool GameManager::placeSoldier(Player * player, Province * province, int amount)
////{
////	if (province->getOwner() != NULL && province->getOwner() != player)
////		return false;
////	if (!player->hasProvince(worldMap, province))
////		player->captureProvince(worldMap, province);
////	return player->placeSoldier(worldMap, amount, province);
////}
