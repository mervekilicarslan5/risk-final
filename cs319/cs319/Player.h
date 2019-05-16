//#pragma once
//
//#ifndef PLAYER_H
//#define PLAYER_H
//
//using namespace std;
//
//#include "WorldMap.h"
//#include <vector>
//#include <string>
//
//class Player
//{
//public:
//	Player();
//	Player(string _name, int _id);
//	~Player();
//	vector<int> getProvinces();
//	vector<int> getBonusCards();
//	string getName();
//	int getMoney();
//	int getId();
//	int getBattlesWon();
//	int getBattlesLost();
//	void setMoney(int _money);
//	void setBattlesWon(int _battlesWon);
//	void setBattlesLost(int _battlesLost);
//	void setName(string _name);
//	void setId(int _id);
//	/*void captureProvince(WorldMap* worldMap, Province* _province);
//	bool placeSoldier(WorldMap* worldMap, int amount, Province* _province);
//	bool hasProvince(WorldMap * worldMap, Province* _province);*/
//private:
//	string name;
//	int money;
//	int id;
//	int battlesWon;
//	int battlesLost;
//	vector<int> provinces;
//	vector<int> bonusCards;
//};
//
//#endif