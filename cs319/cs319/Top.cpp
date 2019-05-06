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
	money = 200;
}

Player::Player(string _name, int _id)
{
	name = _name;
	id = _id;
	battlesLost = 0;
	battlesWon = 0;
	money = 200;
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

void Player::incBattlesLost() {
	battlesLost++;
}

void Player::incBattlesWon() {
	battlesWon++;
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

int Player::getNumberOfProvinces() {
	return provinces.size();
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

int WorldMap::getNumberOfProvinces() {
	return numberOfProvinces;
}

Province* WorldMap::getProvinceByID(int id) {
	if (id < provinceList.size())
		return provinceList[id];
	return NULL;
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

bool GameManager::attack(Player* attacker, Player* defender, Province* from, Province * to, int amount) {
	if (attacker == defender) {
		cout << "You cannot attack yourself!" << endl;
		return false;
	}
	if (from->getOwner() != attacker) {
		cout << from->getName() << " does not belong to " << attacker->getName() << endl;
		return false;
	}
	if (to->getOwner() != defender) {
		cout << to->getName() << "does not belong to " << defender->getName() << endl;
		return false;
	}
	if (!worldMap->isNeighbor(from, to)) {
		cout << from->getName() << " and " << to->getName() << " are not neighbors!" << endl;
		return false;
	}
	if (amount >= from->getNumberOfSoldiers()) {
		cout << "There has to be at least one soldier left!" << endl;
		return false;
	}
	vector<int> result;
	if (to->getNumberOfSoldiers() == 1) {
		result = rollDice(amount, 1);
	}
	else {
		result = rollDice(amount, 2);
	}
	cout << "Attacker lost " << result[1] << " soldiers" << endl;
	cout << "Defender lost " << result[0] << " soldiers" << endl;
	from->setNumberOfSoldiers(from->getNumberOfSoldiers() - result[1]);
	to->setNumberOfSoldiers(to->getNumberOfSoldiers() - result[0]);
	if (result[0] > result[1]) {
		attacker->incBattlesWon();
		defender->incBattlesLost();
	}
	else {
		attacker->incBattlesLost();
		defender->incBattlesWon();
	}
		
	if (to->getNumberOfSoldiers() == 0) {
		cout << attacker->getName() << " has captured the city " << to->getName() << endl;
		attacker->captureProvince(worldMap, to);

		int number = -1;
		while (number <= 0 || number >= from->getNumberOfSoldiers()) {
			cout << "Enter the number of soldiers you want to place on this city: ";
			cin >> number;
		}
		fortify(attacker, from, to, number);
		return true;
	}
	return false;
}

bool GameManager::attack(string attackerName, string defenderName, string fromStr, string toStr, int amount) {
	int i;
	Player* attacker = getPlayerByName(attackerName, i);
	Player* defender = getPlayerByName(defenderName, i);
	if (attacker == NULL || defender == NULL) {
		cout << "No such players" << endl;
		return false;
	}
	Province* from, * to;
	worldMap->getProvinceByName(fromStr, i, from);
	if (i == -1) {
		cout << fromStr << " does not exist!" << endl;
		return false;
	}
	worldMap->getProvinceByName(toStr, i, to);
	if (i == -1) {
		cout << toStr << " does not exist!" << endl;
		return false;
	}
	return attack(attacker, defender, from, to, amount);
}

void GameManager::showWorldStatus() {
	worldMap->showWorldStatus();
}
void GameManager::showProvinceStatus(string name) {
	int i; Province* ptr;
	worldMap->getProvinceByName(name, i, ptr);
	worldMap->showProvinceStatus(ptr);
}

vector<int> GameManager::rollDice(int attacker, int defender)  {
	
	vector<int> attackerResults, defenderResults;
	vector<int> result;
	result.push_back(0);
	result.push_back(0);

	for (int i = 0; i < attacker; i++) {
		attackerResults.push_back(die->roll());
	}
	std::sort(attackerResults.rbegin(), attackerResults.rend());
	for (int i = 0; i < defender; i++) {
		defenderResults.push_back(die->roll());
	}
	std::sort(defenderResults.rbegin(), defenderResults.rend());

	int mn = min(attacker, defender);

	for (int i = 0; i < mn; i++) {
		cout << "Attacker : " << attackerResults[i] << " vs " << defenderResults[i] << " : Defender" << endl;
		if (attackerResults[i] > defenderResults[i])
			result[0]++;
		else
			result[1]++;
	}

	return result; 
}

void GameManager::startPlacement() {
	cout << "*** INITIAL SOLDIER PLACEMENT ***" << endl << endl;

	int turn = 0;
	int numberOfPlayers = players.size();
	int numberOfProvinces = worldMap->getNumberOfProvinces();

	for (int i = 0; i < numberOfProvinces; i++) {
		Player* currentPlayer = players[turn % numberOfPlayers];
		cout << currentPlayer->getName() << "'s turn" << endl; 
		
		Province* currentProvince;
		string city;
		while (true) {
			cout << "Choose a city to place a soldier: ";
			 cin >> city;
			int dummy;
			
			worldMap->getProvinceByName(city, dummy, currentProvince);
			
			if (currentProvince == NULL) {
				cout << "No such city." << endl;
			}
			else if (currentProvince->getOwner() != NULL) {
				cout << city << " has already been taken." << endl;
			}
			else
				break;
		}
		placeSoldier(currentPlayer, city, 1);
		turn++;
	}

	cout << "*** SOLDIER PLACEMENT IS DONE ***" << endl;
}

void GameManager::startTurn(int id) {
	string playerName;
	Player* currentPlayer = getPlayerByID(id, playerName);
	cout << "*** " << playerName << "'s turn ***" << endl;
	startPlacementPhase(id);
	startAttackingPhase(id);
	if (gameOn) {
		startMarket(id);
		startFortifyPhase(id);
	}
	showWorldStatus();
}

void GameManager::loadProvinces() {
	createProvince("ankara", "");
	createProvince("istanbul", "");
	createProvince("konya", "");
	createProvince("antalya", "");
	createProvince("eskisehir", "");
	createProvince("edirne", "");
	createProvince("kars", "");
	createProvince("aksaray", "");
	createProvince("kocaeli", "");

	createNeighbor("ankara", "konya");
	createNeighbor("ankara", "eskisehir");
	createNeighbor("konya", "antalya");
	createNeighbor("aksaray", "konya");
	createNeighbor("istanbul", "edirne");
	createNeighbor("istanbul", "kocaeli");
	createNeighbor("eskisehir", "kocaeli");
}

void GameManager::startGame() {
	loadProvinces();
	//startPlacement();
	randomPlacement();
	showWorldStatus();
	gameOn = true;

	int turn = 0;
	int numberOfPlayers = players.size();
	int numberOfProvinces = worldMap->getNumberOfProvinces();

	while (gameOn) {
		cout << endl <<  "==================================  " << players[turn]->getName() << "'s turn " << endl;
		startTurn(turn);

		turn = (turn + 1) % numberOfPlayers;
	}
}

void GameManager::startPlacementPhase(int id) {
	string playerName;
	Player* player = getPlayerByID(id, playerName);

	cout << endl << "*** SOLDIER PLACEMENT PHASE ***" << endl;
	int n = player->getNumberOfProvinces() / 3;
	string choice = "";
	while (n > 0) {
		cout << "You have " << n << " soldiers to place." << endl;
		cout << "city : ";
		string cityName; int amount;
		cin >> cityName;
		Province* city;
		worldMap->getProvinceByName(cityName, amount, city);
		if (city != NULL) {
			cout << "amount : ";
			cin >> amount;
			if (amount <= n) {
				if (placeSoldier(player, cityName, amount)) {
					cout << amount << " soldiers have been placed to " << cityName << endl;
					n -= amount;
					showProvinceStatus(cityName);
				}
				else {
					cout << cityName << " does not belong to you." << endl;
				}
			}
			else {
				cout << "The amount cannot be more than " << n << endl;
			} 
		}
		else {
			cout << "No such city" << endl;
		}
	}
}

void GameManager::startAttackingPhase(int id) {
	string playerName;
	Player* player = getPlayerByID(id, playerName);

	cout << endl << "*** ATTACKING PHASE ***" << endl;

	string from, to;
	while (true) {
		cout << "city: ";
		cin >> from;
		if (from == "done")
			break;
		Province* fromPtr; int dummy;
		worldMap->getProvinceByName(from, dummy, fromPtr);
		if (fromPtr == NULL) {
			cout << "No such city " << endl;
		}
		else if (fromPtr->getOwner() != player) {
			cout << "This city does not belong to you" << endl;
		}
		else {
			cout << "attack to: ";
			cin >> to;
			Province* toPtr; 
			worldMap->getProvinceByName(to, dummy, toPtr);
			if (toPtr == NULL) {
				cout << "No such city" << endl;
			}
			else if (toPtr->getOwner() == player) {
				cout << "You cannot attack your own city." << endl;
			}
			else if (!worldMap->isNeighbor(fromPtr, toPtr)) {
				cout << "The cities must be neighbor" << endl;
			}
			else {
				int amount;
				cout << "amount: ";
				cin >> amount;
				if (amount >= fromPtr->getNumberOfSoldiers() ) {
					cout << "There has to be at least one soldier left in the city" << endl;
				}
				else if (amount > 3 || amount < 1){
					cout << "The number needs to be betwen 1 - 3" << endl;
				}
				else {
					attack(player, toPtr->getOwner(), fromPtr, toPtr, amount);
					showProvinceStatus(from);
					showProvinceStatus(to);
					if (player->getNumberOfProvinces() == worldMap->getNumberOfProvinces()) {
						gameOn = false;
						cout << "*** " << player->getName() << " WON" << endl;
						break;
					}
				}
			}
		}
	}
}

void GameManager::startMarket(int id) {
	string playerName;
	Player* player = getPlayerByID(id, playerName);

	cout << endl << "*** MARKET PHASE ***" << endl;
	cout << "Your money: " << player->getMoney() << endl;
	cout << "How many soldiers do you want? (100 gold per soldier): ";
	int amount; 
	while (true) {
		cin >> amount;
		if (amount * 100 > player->getMoney()) {
			cout << "Insufficient money" << endl;
		}
		else {
			player->setMoney(player->getMoney()-amount*100);
			break;
		}
	}

	int n = amount;
	string choice = "";
	while (n > 0) {
		cout << "You have " << n << " soldiers to place." << endl;
		cout << "city : ";
		string cityName; int amount;
		cin >> cityName;
		Province* city;
		worldMap->getProvinceByName(cityName, amount, city);
		if (city != NULL) {
			cout << "amount : ";
			cin >> amount;
			if (amount <= n) {
				if (placeSoldier(player, cityName, amount)) {
					cout << amount << " soldiers have been placed to " << cityName << endl;
					n -= amount;
					showProvinceStatus(cityName);
				}
				else {
					cout << cityName << " does not belong to you." << endl;
				}
			}
			else {
				cout << "The amount cannot be more than " << n << endl;
			}
		}
		else {
			cout << "No such city" << endl;
		}
	}
}

void GameManager::startFortifyPhase(int id) {
	string playerName;
	Player* player = getPlayerByID(id, playerName);

	cout << endl << "*** FORTIFY PHASE ***" << endl;

	string from, to;
	while (true) {
		cout << "from: ";
		cin >> from;
		if (from == "done")
			break;
		Province* fromPtr; int dummy;
		worldMap->getProvinceByName(from, dummy, fromPtr);
		if (fromPtr == NULL) {
			cout << "No such city " << endl;
		}
		else if (fromPtr->getOwner() != player) {
			cout << "This city does not belong to you" << endl;
		}
		else {
			cout << "to: ";
			cin >> to;
			Province* toPtr;
			worldMap->getProvinceByName(to, dummy, toPtr);
			if (toPtr == NULL) {
				cout << "No such city" << endl;
			}
			else if (toPtr->getOwner() != player) {
				cout << "This city does not belong to you" << endl;
			}
			else if (!worldMap->hasPath(player,fromPtr,toPtr)) {
				cout << "There must be a path between the cities" << endl;
			}
			else {
				int amount;
				cout << "amount: ";
				cin >> amount;
				if (amount >= fromPtr->getNumberOfSoldiers()) {
					cout << "There has to be at least one soldier left in the city" << endl;
				}
				else {
					fortify(player, fromPtr, toPtr, amount);
				}
			}
		}
	}

}

void GameManager::randomPlacement() {
	
	int size = worldMap->getNumberOfProvinces();
	Die die(size);
	int* shuffledArray = new int[size];
	for (int i = 0; i < size; i++) {
		shuffledArray[i] = i;
	}
	for (int i = 0; i < size; i++) {
		int r = die.roll() - 1;
		int temp;
		temp = shuffledArray[i];
		shuffledArray[i] = shuffledArray[r];
		shuffledArray[r] = temp;
	}
	int t = 0;
	int numberOfPlayers = players.size();
	for (int i = 0; i < size; i++) {
		Province* curProvince = worldMap->getProvinceByID(shuffledArray[i]);
		placeSoldier(players[t], curProvince->getName(), 1);
		t = (t + 1) % numberOfPlayers;
	}
	cout << "*** PROVINCES HAVE BEEN RANDOMLY DISTRIBUTED TO THE PLAYERS ***" << endl;
	delete shuffledArray;
}




