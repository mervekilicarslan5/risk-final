#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
using namespace std;

class Die;
class Castle;
class Province;
class Continent;
class Player;
class WorldMap;
class WorldMap;

class Die
{
public:
	Die();
	Die(int _numberOfFaces);
	~Die();
	int roll();

private:
	int numberOfFaces;
};


class Castle
{
public:
	Castle();
	Castle(int _price);
	~Castle();
	int getPrice();
	void setPrice(int _price);
	int getLevel();
	void setLevel(int _level);
	void upgradeLevel();
	bool isBuilt();
	void build();
	void destroy();
private:
	int price;
	int level;
	bool built;
};

class Province
{
public:
	Province();
	Province(string _name, string _color);
	~Province();
	string getName();
	Player* getOwner();
	string getColor();
	int getNumberOfSoldiers();
	Castle* getCastle();
	void setName(string _name);
	void setOwner(Player* _owner);
	void setColor(string _color);
	void setNumberOfSoldiers(int _numberOfSoldiers);

private:
	string name;
	Player* owner;
	string color;
	int numberOfSoldiers;
	Castle* castle;
};

class Player
{
public:
	Player();
	Player(string _name, int _id);
	~Player();
	vector<int> getProvinces();
	vector<int> getBonusCards();
	string getName();
	int getMoney();
	int getId();
	int getBattlesWon();
	int getBattlesLost();
	void setMoney(int _money);
	void setBattlesWon(int _battlesWon);
	void setBattlesLost(int _battlesLost);
	void incBattlesWon();
	void incBattlesLost();
	void setName(string _name);
	void setId(int _id);
	void captureProvince(WorldMap* worldMap, Province* _province);
	void loseProvince(WorldMap* worldMap, Province* _province);
	bool placeSoldier(WorldMap* worldMap, int amount, Province* _province);
	bool hasProvince(WorldMap * worldMap, Province* _province);
	bool buildCastle(Province* province);
	int getNumberOfProvinces();
private:
	string name;
	int money;
	int id;
	int battlesWon;
	int battlesLost;
	vector<int> provinces;
	vector<int> bonusCards;
};

class WorldMap
{
public:
	WorldMap();
	~WorldMap();
	void addProvince(Province* _province);
	void addNeighbor(Province* first, Province* second);
	void printNeighbors(Province* _province);
	bool isNeighbor(Province* first, Province* second);
	bool hasPath(Player* player, Province* from, Province* to);
	int findIndex(Province* _province);
	void getProvinceByName(string name, int & index, Province* & ptr);
	void showWorldStatus();
	void showProvinceStatus(Province* p);
	int getNumberOfProvinces();
	Province* getProvinceByID(int id);
private:
	int numberOfProvinces;
	vector< Province* > provinceList;
	vector< vector<int> > map;
	void addEdge(int first, int second);
};

class Continent
{
public:
	Continent();
	~Continent();
	vector<int> getProvinces();
	int getPoint();
	Player* getOwner();
	string getName();
	void setName(string _name);
	void setPoint(int _point);
	void setOwner(Player* owner);
	void setProvinces(vector<int> _provinces);
private:
	vector<int> provinces;
	int point;
	Player* owner;
	string name;
};


class GameManager
{
public:

	GameManager();
	~GameManager();

	WorldMap* getWorldMap();
	void createProvince(string name, string color);
	void createNeighbor(string first, string second);
	void addPlayer(string _name);
	Player* getPlayerByID(int id, string & name);
	Player* getPlayerByName(string name, int & id);
	bool buildCastle(Player* player, Province* province);
	bool buildCastle(int playerID, string provinceName);
	bool buildCastle(string playerName, string provinceName);
	bool placeSoldier(Player* player, string provinceName, int amount);
	bool placeSoldier(int playerID, string provinceName, int amount);
	bool placeSoldier(string playerName, string provinceName, int amount);
	bool fortify(Player* player, Province* from, Province * to, int amount);
	bool fortify(int playerID, string from, string to, int amount);
	bool fortify(string name, string from, string to, int amount);
	bool attack(Player* attacker, Player* defender, Province* from, Province * to, int amount);
	bool attack(string attacker, string defender, string from, string to, int amount);
	void showWorldStatus();
	void showProvinceStatus(string name);
	vector<int> rollDice(int attacker, int defender);
	void startPlacement();
	void startTurn(int id);
	void loadProvinces();
	void startGame();
	void startPlacementPhase(int id);
	void startAttackingPhase(int id);
	void startMarket(int id);
	void startFortifyPhase(int id);
	void randomPlacement();

private:
	vector<Player*> players;
	WorldMap* worldMap;
	Die* die;
	bool gameOn;
};

