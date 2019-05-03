#include "Top.h"

Die::Die()
{
	numberOfFaces = 6;
}

Die::Die(int _numberOfFaces)
{
	numberOfFaces = _numberOfFaces;
}


Die::~Die()
{
}

int Die::roll()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(1, numberOfFaces + 1);
	return dist(mt);
}


Castle::Castle()
{
}

Castle::Castle(int _price)
{
	price = _price;
	level = 0;
}

Castle::~Castle()
{
}

int Castle::getPrice()
{
	return price;
}

void Castle::setPrice(int _price)
{
	price = _price;
}

int Castle::getLevel()
{
	return level;
}

void Castle::setLevel(int _level)
{
	level = _level;
}

void Castle::upgradeLevel()
{
	if (level == 0)
		level = 1;
}


Continent::Continent()
{
}


Continent::~Continent()
{
}

vector<int> Continent::getProvinces()
{
	return provinces;
}

int Continent::getPoint()
{
	return point;
}

Player * Continent::getOwner()
{
	return owner;
}

string Continent::getName()
{
	return name;
}

void Continent::setName(string _name)
{
	name = _name;
}

void Continent::setPoint(int _point)
{
	point = _point;
}

void Continent::setOwner(Player * _owner)
{
	owner = _owner;
}

void Continent::setProvinces(vector<int> _provinces)
{
	provinces = _provinces;
}

Player::Player()
{
	name = "";
	id = -1;
	battlesLost = 0;
	battlesWon = 0;
}

Player::Player(string _name, int _id)
{
	name = _name;
	id = _id;
	battlesLost = 0;
	battlesWon = 0;
}


Player::~Player()
{
}

vector<int> Player::getProvinces()
{
	return provinces;
}

vector<int> Player::getBonusCards()
{
	return bonusCards;
}

string Player::getName()
{
	return name;
}

int Player::getMoney()
{
	return money;
}

int Player::getId()
{
	return id;
}

int Player::getBattlesWon()
{
	return battlesWon;
}

int Player::getBattlesLost()
{
	return battlesLost;
}

void Player::setMoney(int _money)
{
	money = _money;
}

void Player::setBattlesWon(int _battlesWon)
{
	battlesWon = _battlesWon;
}

void Player::setBattlesLost(int _battlesLost)
{
	battlesLost = _battlesLost;
}

void Player::setName(string _name)
{
	name = _name;
}

void Player::setId(int _id)
{
	id = _id;
}

void Player::captureProvince(WorldMap* worldMap, Province * _province)
{

	provinces.push_back(worldMap->findIndex(_province));
	_province->setOwner(this);
}

bool Player::placeSoldier(WorldMap * worldMap, int amount, Province * _province)
{
	if (!hasProvince(worldMap, _province))
		return false;
	_province->setNumberOfSoldiers(_province->getNumberOfSoldiers() + amount);
	return true;
}

bool Player::hasProvince(WorldMap * worldMap, Province * _province)
{
	int index = worldMap->findIndex(_province);
	if (provinces.end() == find(provinces.begin(), provinces.end(), index))
		return false;
	return true;
}

WorldMap::WorldMap()
{
	numberOfProvinces = 0;
}

WorldMap::~WorldMap()
{
	provinceList.clear();
	map.clear(); // Test it 
}

void WorldMap::addProvince(Province * _province)
{
	provinceList.push_back(_province);
	vector<int> empty;
	map.push_back(empty);
	numberOfProvinces++;
}

void WorldMap::addEdge(int first, int second)
{
	map[first].push_back(second);
	map[second].push_back(first);
}

int WorldMap::findIndex(Province * _province)
{	
	int count = 0;
	for (auto it = provinceList.begin(); it != provinceList.end(); it++) {
		if (*it == _province) {
			return count;
		}
		count++;
	}
	return -1;
}

void WorldMap::addNeighbor(Province * first, Province * second)
{
	addEdge(findIndex(first), findIndex(second));
}

void WorldMap::printNeighbors(Province * _province)
{
	int index = findIndex(_province);
	for (auto it = map[index].begin(); it != map[index].end(); it++) {
		cout << provinceList[(*it)]->getName() << endl;
	}
}

bool WorldMap::isNeighbor(Province * first, Province * second)
{
	int index = findIndex(first);
	for (auto it = map[index].begin(); it != map[index].end(); it++) {
		if (provinceList[*it] == second)
			return true;
	}
	return false;
}

bool WorldMap::hasPath(Player * player, Province * from, Province * to)
{
	bool mark[100];
	for (int i = 0; i < 100; i++) {
		mark[i] = false;
	}
	int first = findIndex(from);
	int second = findIndex(to);
	queue<int> q;
	q.push(first);
	mark[first] = true;

	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		if (cur == second)
			return true;

		for (auto it = map[cur].begin(); it != map[cur].end(); it++) {
			if (mark[*it] || provinceList[*it]->getOwner() != player)
				continue;
			q.push(*it);
			mark[*it] = true;
		}

	}

	return false;

}


Province::Province()
{
	name = "";
	color = "";
	numberOfSoldiers = 0;
	owner = NULL;
	castle = NULL;
}

Province::Province(string _name, string _color)
{
	name = _name;
	color = _color;
	numberOfSoldiers = 0;
	owner = NULL;
	castle = NULL;
}


Province::~Province()
{

}

string Province::getName()
{
	return name;
}

Player* Province::getOwner()
{
	return owner;
}

string Province::getColor()
{
	return color;
}

int Province::getNumberOfSoldiers()
{
	return numberOfSoldiers;
}

Castle* Province::getCastle()
{
	return castle;
}

void Province::setName(string _name)
{
	name = _name;
}

void Province::setOwner(Player* _owner)
{
	owner = _owner;
}

void Province::setColor(string _color)
{
	color = _color;
}

void Province::setNumberOfSoldiers(int _numberOfSoldiers)
{
	numberOfSoldiers = _numberOfSoldiers;
}

GameManager::GameManager()
{
	worldMap = new WorldMap();
	die = new Die(6);
}


GameManager::~GameManager()
{
	delete worldMap;
	delete die;
}

void GameManager::addPlayer(string _name)
{
	int numberOfPlayers = players.size();
	if (numberOfPlayers < 6) {
		players.push_back(new Player(_name, numberOfPlayers));
	}
}


bool GameManager::placeSoldier(Player * player, Province * province, int amount)
{
	if (province->getOwner() != NULL && province->getOwner() != player)
		return false;
	if (!player->hasProvince(worldMap, province))
		player->captureProvince(worldMap, province);
	return player->placeSoldier(worldMap, amount, province);
}
