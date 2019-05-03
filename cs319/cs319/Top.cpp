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
	built = false;
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

bool Castle::isBuilt() {
	return built;
}
void Castle::build() {
	built = true;
}
void Castle::destroy() {
	built = false;
	level = 0;
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

void Player::loseProvince(WorldMap* worldMap, Province* _province) {
	provinces.erase(provinces.begin() + worldMap->findIndex(_province));
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

bool Player::buildCastle(Province* province) {
	if (province->getCastle()->getPrice() > money) {
		cout << "Insufficient money to build castle!";
		return false;
	}
	province->getCastle()->build();
	money -= province->getCastle()->getPrice();
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

void WorldMap::getProvinceByName(string name, int & index, Province* & ptr) {
	int i = 0;
	for (auto it = provinceList.begin(); it != provinceList.end(); it++) {
		if ((*it)->getName() == name) {
			index = i;
			ptr = *it;
			return;
		}
		i++;
	}
	index = -1;
	ptr = NULL;
}

void WorldMap::showProvinceStatus(Province* p) {
	cout << "------------------------------------" << endl;
	cout << "Province Name: " << p->getName() << endl;
	if(p->getOwner() == NULL)
		cout << "Current Owner: None" << endl;
	else
		cout << "Current Owner: " << p->getOwner()->getName() << endl;
	cout << "Number of Soldiers: " << p->getNumberOfSoldiers() << endl;
}

void WorldMap::showWorldStatus() {
	for (auto it = provinceList.begin(); it != provinceList.end(); it++) {
		showProvinceStatus(*it);
	}
}

Province::Province()
{
	name = "";
	color = "";
	numberOfSoldiers = 0;
	owner = NULL;
	castle = new Castle(150);
}

Province::Province(string _name, string _color)
{
	name = _name;
	color = _color;
	numberOfSoldiers = 0;
	owner = NULL;
	castle = new Castle(150);
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

WorldMap* GameManager::getWorldMap() {
	return worldMap;
}

void GameManager::createProvince(string name, string color) {
	Province* p = new Province(name, color);
	worldMap->addProvince(p);
}

void GameManager::createNeighbor(string first, string second) {
	Province* firstPtr;
	Province* secondPtr;
	int i;
	worldMap->getProvinceByName(first, i, firstPtr);
	worldMap->getProvinceByName(second, i, secondPtr);
	worldMap->addNeighbor(firstPtr, secondPtr);
}

void GameManager::addPlayer(string _name)
{
	int numberOfPlayers = players.size();
	if (numberOfPlayers < 6) {
		players.push_back(new Player(_name, numberOfPlayers));
	}
}

Player* GameManager::getPlayerByID(int id, string & name) {
	for (auto it = players.begin(); it != players.end(); it++) {
		if ((*it)->getId() == id) {
			name = (*it)->getName();
			return *it;
		}
	}
	name = "";
	cout << "There is no such player with ID: " << id << endl;
	return NULL;
}

Player* GameManager::getPlayerByName(string name, int & id) {
	for (auto it = players.begin(); it != players.end(); it++) {
		if ((*it)->getName() == name) {
			id = (*it)->getId();
			return *it;
		}
	}
	id = -1;
	cout << "There is no such player with name: " << name << endl;
	return NULL;
}

bool GameManager::buildCastle(Player* player, Province* province) {
	if (province->getOwner() != player) {
		cout << "This province does not belong to the player." << endl;
		return false;
	}
	player->buildCastle(province);
	cout << player->getName() << " built a castle in " << province->getName() << endl;
	return true;
}
bool GameManager::buildCastle(int playerId, string provinceName) {
	string str;
	Player* player = getPlayerByID(playerId, str);
	Province* province; int i;
	worldMap->getProvinceByName(provinceName, i, province);
	return buildCastle(player, province);
}
bool GameManager::buildCastle(string playerName, string provinceName) {
	int i;
	Player* player = getPlayerByName(playerName, i);
	Province* province;
	worldMap->getProvinceByName(provinceName, i, province);
	return buildCastle(player, province);
}

bool GameManager::placeSoldier(Player * player, string provinceName, int amount)
{	
	Province* province; int i;
	worldMap->getProvinceByName(provinceName, i, province);
	if (province->getOwner() != NULL && province->getOwner() != player)
		return false;
	if (!player->hasProvince(worldMap, province))
		player->captureProvince(worldMap, province);
	return player->placeSoldier(worldMap, amount, province);
}

bool GameManager::placeSoldier(string playerName, string provinceName, int amount)
{
	int i;
	Player* p = getPlayerByName(playerName, i);
	if (p == NULL)
		return false;
	return placeSoldier(p, provinceName, amount);
}

bool GameManager::placeSoldier(int playerId, string provinceName, int amount)
{
	string str;
	Player* p = getPlayerByID(playerId, str);
	if (p == NULL)
		return false;
	return placeSoldier(p, provinceName, amount);
}

bool GameManager::fortify(Player* player, Province* from, Province * to, int amount) {
	if (from->getNumberOfSoldiers() <= amount) {
		cout << "There has to be at least one soldier in a province!" << endl;
		return false;
	}
	if (!worldMap->hasPath(player, from, to)) {
		cout << "No path between " << from->getName() << " and " << to->getName() << endl;
		return false;
	}
		
	from->setNumberOfSoldiers(from->getNumberOfSoldiers() - amount);
	to->setNumberOfSoldiers(to->getNumberOfSoldiers() + amount);
	cout << amount << " soldiers have been fortified from " << from->getName() << " to " << to->getName() << endl;
	return true;
}

bool GameManager::fortify(int playerID, string fromStr, string toStr, int amount) {
	string str;
	Player* player = getPlayerByID(playerID, str);
	if (player == NULL)
		return false;

	Province* from, *to;
	int i;
	worldMap->getProvinceByName(fromStr, i, from);
	if (i == -1)
		return false;
	worldMap->getProvinceByName(fromStr, i, to);
	if (i == -1)
		return false;

	return fortify(player, from, to, amount);
}

bool GameManager::fortify(string name, string fromStr, string toStr, int amount) {
	int ind;
	Player* player = getPlayerByName(name, ind);
	if (player == NULL)
		return false;
	Province* from, *to;
	int i;
	worldMap->getProvinceByName(fromStr, i, from);
	if (i == -1)
		return false;
	worldMap->getProvinceByName(toStr, i, to);
	if (i == -1)
		return false;

	return fortify(player, from, to, amount);
}

void GameManager::showWorldStatus() {
	worldMap->showWorldStatus();
}
void GameManager::showProvinceStatus(string name) {
	int i; Province* ptr;
	worldMap->getProvinceByName(name, i, ptr);
	worldMap->showProvinceStatus(ptr);
}