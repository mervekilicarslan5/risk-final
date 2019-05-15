#include "Top.h"
namespace globalsS {
	int screenWidth;
	int screenHeight;
};
using namespace std;
using namespace globalsS;
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
	if (level == 1) {
		level = 2;
		price += 50;
	}

}

bool Castle::isBuilt() {
	return built;
}

void Castle::build() {
	built = true;
	level = 1;
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
	leftSoldier = 70;
	name = "";
	id = -1;
	battlesLost = 0;
	battlesWon = 0;
	money = 200;
}

Player::Player(string _name, int _id)
{
	leftSoldier = 70;
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
	if (!(this->hasProvince(worldMap, _province))) {
		if (_province->getOwner() != NULL) {
			_province->getOwner()->loseProvince(worldMap, _province);
		}
		provinces.push_back(worldMap->findIndex(_province));
		_province->setOwner(this);
	}

}

void Player::loseProvince(WorldMap* worldMap, Province* _province) {
	vector <int> ::iterator index = find(provinces.begin(), provinces.end(), worldMap->findIndex(_province));
	if (index != provinces.end())
		provinces.erase(index);
}

bool Player::placeSoldier(WorldMap * worldMap, int amount, Province * _province)
{
	int pricesold = 50;
	if (leftSoldier < amount) {
		if (money < (pricesold * amount)) {
			cout << "******* insufficient money for soldier" << endl;
			return false;
		}
		else {
			money -= (pricesold*amount);
			_province->setNumberOfSoldiers(_province->getNumberOfSoldiers() + amount);
			worldMap->showProvinceStatus(_province);
			return true;
		}
	}
		
	if (!hasProvince(worldMap, _province))
		return false;
	_province->setNumberOfSoldiers(_province->getNumberOfSoldiers() + amount);
	worldMap->showProvinceStatus(_province);
	leftSoldier -= amount;
	return true;
}

bool Player::hasProvince(WorldMap * worldMap, Province * _province)
{
	int index = worldMap->findIndex(_province);
	if (provinces.end() == find(provinces.begin(), provinces.end(), index))
		return false;
	return true;
}

int Player::buildCastle(Province* province) {

	if (province->getCastle()->isBuilt() == false) {
		if (province->getCastle()->getPrice() > money) {
			cout << "Insufficient money to build castle!";
			return 0;
		}
		province->getCastle()->build();
		money -= province->getCastle()->getPrice();
		return 1;
	}

	else if (province->getCastle()->getLevel() == 1) {
		if (province->getCastle()->getPrice() > money) {
			cout << "Insufficient money to upgrade castle!";
			return false;
		}
		province->getCastle()->upgradeLevel();
		money -= province->getCastle()->getPrice();
		return 2;
	}
}

int Player::getNumberOfProvinces() {
	return provinces.size();
}

void Player::setLeftSoldier(int n) {
	leftSoldier = n;
}

int Player::getLeftSoldier() {
	return leftSoldier;
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
	if (find(provinceList.begin(), provinceList.end(), _province) == provinceList.end()) {
		provinceList.push_back(_province);
		vector<int> empty;
		map.push_back(empty);
		numberOfProvinces++;
	}
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
	if (p->getOwner() == NULL)
		cout << "Current Owner: None" << endl;
	else
		cout << "Current Owner: " << p->getOwner()->getName() << endl;
	cout << "Number of Soldiers: " << p->getNumberOfSoldiers() << endl;
	cout << "Castle level: ";
	if (p->getCastle()->getLevel() == 0)
		cout << " 0 (is not buit) " << endl;
	else if (p->getCastle()->getLevel() == 1)
		cout << "1 " << endl;
	else if (p->getCastle()->getLevel() == 2)
		cout << "2 " << endl;
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

int WorldMap::ownerCount() {
	int count = 0;
	for (auto it = provinceList.begin(); it != provinceList.end(); it++) {
		if ((*it)->getOwner() != NULL) {
			count++;
		}
	}
	return count;
}

vector<Province*> WorldMap::getNeighbors(Province* _province) {
	vector<Province*> result;
	int index = findIndex(_province);
	for (auto it = map[index].begin(); it != map[index].end(); it++) {
		result.push_back(provinceList[(*it)]);
	}
	return result;
}

Province::Province()
{
	name = "";
	color = "";
	numberOfSoldiers = 0;
	owner = NULL;
	castle = new Castle(50);
}

Province::Province(string _name, string _color)
{
	name = _name;
	color = _color;
	numberOfSoldiers = 0;
	owner = NULL;
	castle = new Castle(50);
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
	currentPlayer = 0;
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
		players.push_back(new Player(_name, players.size()));
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
	int done = player->buildCastle(province);

	if (done == 1)
		cout << player->getName() << " built a castle in " << province->getName() << endl;
	else if (done == 2)
		cout << player->getName() << " has upgraded castle in " << province->getName() << endl;
	else
		return false;
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
	if (result[1] != 0 || result[0] != 0) {
		windowManager->battleNotifier->updateData("Battle of " + to->getName(), attacker->getName(), defender->getName(), to_string(from->getNumberOfSoldiers()), to_string(to->getNumberOfSoldiers()), to_string(result[1]), to_string(result[0]));
		windowManager->battleNotifier->activate();
	}

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
	Province* from, *to;
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

vector<int> GameManager::rollDice(int attacker, int defender) {

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
	//showWorldStatus();
}

void GameManager::loadProvinces() {
	cout << "Provinces are loading" << endl;
	/*createProvince("ankara", "");
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
	*/
	fstream file("assets/provinces.txt");

	int color;
	string cityName;
	bool stepOne = false;
	bool stepTwo = false;
	string line;
	ifstream myfile("assets/provinces.txt");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector<string> prov = split(line, ';');
			cityName = prov[0];
			color = stoi(prov[1]);
			createProvince(cityName, to_string(color));
			colorLookUpTable.insert(pair<int, string>(color, cityName));
			cout << cityName << " " << color << " loaded. " << endl;
		}
		myfile.close();
	}

	createNeighbor("Alaska", "North West");
	createNeighbor("Alaska", "Alberta");
	createNeighbor("Alaska", "Kamchatka");
	createNeighbor("North West", "Alberta");
	createNeighbor("North West", "Ontario");
	createNeighbor("North West", "Greenland");
	createNeighbor("Alberta", "Ontario");
	createNeighbor("Alberta", "Western United States");
	createNeighbor("Ontario", "Greenland");
	createNeighbor("Ontario", "Quebec");
	createNeighbor("Ontario", "Eastern United States");
	createNeighbor("Ontario", "Western United States");
	createNeighbor("Quebec", "Eastern United States");
	createNeighbor("Quebec", "Greenland");
	createNeighbor("Eastern United States", "Western United States");
	createNeighbor("Eastern United States", "Central America");
	createNeighbor("Western United States", "Central America");
	createNeighbor("Central America", "Venezuela");
	createNeighbor("Venezuela", "Brazil");
	createNeighbor("Venezuela", "Peru");
	createNeighbor("Brazil", "Peru");
	createNeighbor("Brazil", "North Africa");
	createNeighbor("Brazil", "Argentina");
	createNeighbor("Peru", "Argentina");
	createNeighbor("North Africa", "Egypt");
	createNeighbor("North Africa", "Congo");
	createNeighbor("North Africa", "East Africa");
	createNeighbor("Congo", "East Africa");
	createNeighbor("Congo", "South Africa");
	createNeighbor("South Africa", "Madagascar");
	createNeighbor("East Africa", "Madagascar");
	createNeighbor("East Africa", "South Africa");
	createNeighbor("East Africa", "Middle East");
	createNeighbor("East Africa", "Egypt");
	createNeighbor("Egypt", "Middle East");
	createNeighbor("Egypt", "Southern Europe");
	createNeighbor("North Africa", "Western Europe");
	createNeighbor("Western Europe", "Northern Europe");
	createNeighbor("Western Europe", "Southern Europe");
	createNeighbor("Northern Europe", "Southern Europe");
	createNeighbor("Northern Europe", "England");
	createNeighbor("Northern Europe", "Scandinavia");
	createNeighbor("Northern Europe", "Russia");
	createNeighbor("England", "Scandinavia");
	createNeighbor("England", "Iceland");
	createNeighbor("Iceland", "Greenland");
	createNeighbor("Iceland", "Scandinavia");
	createNeighbor("Scandinavia", "Russia");
	createNeighbor("Russia", "Southern Europe");
	createNeighbor("Russia", "Ural");
	createNeighbor("Russia", "Afghanistan");
	createNeighbor("Russia", "Middle East");
	createNeighbor("Southern Europe", "Middle East");
	createNeighbor("Middle East", "India");
	createNeighbor("Middle East", "Afghanistan");
	createNeighbor("Afghanistan", "Ural");
	createNeighbor("Afghanistan", "India");
	createNeighbor("Afghanistan", "China");
	createNeighbor("India", "China");
	createNeighbor("India", "Siam");
	createNeighbor("Siam", "China");
	createNeighbor("Siam", "Indonesia");
	createNeighbor("China", "Ural");
	createNeighbor("China", "Mongolia");
	createNeighbor("China", "Siberia");
	createNeighbor("Mongolia", "Siberia");
	createNeighbor("Mongolia", "Irkutsk");
	createNeighbor("Mongolia", "Japan");
	createNeighbor("Mongolia", "Kamchatka");
	createNeighbor("Irkutsk", "Kamchatka");
	createNeighbor("Irkutsk", "Yakutsk");
	createNeighbor("Irkutsk", "Siberia");
	createNeighbor("Siberia", "Ural");
	createNeighbor("Japan", "Kamchatka");
	createNeighbor("Indonesia", "New Guinea");
	createNeighbor("New Guinea", "Western Australia");
	createNeighbor("New Guinea", "Eastern Australia");
	createNeighbor("Western Australia", "Eastern Australia");


	/*
	createNeighbor("kars", "balikesir");
	createNeighbor("kars", "antalya");
	createNeighbor("kars", "istanbul");
	createNeighbor("istanbul", "eskisehir");
	createNeighbor("istanbul", "kocaeli");
	createNeighbor("istanbul", "antalya");
	createNeighbor("kocaeli", "ankara");
	createNeighbor("kocaeli", "eskisehir");
	createNeighbor("ankara", "afyon");
	createNeighbor("kocaeli", "afyon");
	createNeighbor("edirne", "afyon");
	createNeighbor("nevsehir", "afyon");
	createNeighbor("edirne", "konya");
	createNeighbor("edirne", "ankara");
	createNeighbor("konya", "antalya");*/

	cout << "efsfsef" << endl;

}

void GameManager::startGame(NetworkManager ** NM) {

	loadProvinces();
	if ((*NM)->connectionType == "h") {
		//startPlacement();
		randomPlacement();
		for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
			Province * pro = worldMap->getProvinceByID(i);
			(*NM)->sendDataFromHost(this, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(), pro->getCastle()->getLevel());
			//NM->sendDataFromHost(this, 1, i, 4);
		}
	}
	else {
		for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
			//NM->sendDataFromHost(this, worldMap->getProvinceByID(i)->getOwner()->getId(), i, worldMap->getProvinceByID(i)->getNumberOfSoldiers());
			(*NM)->sendDataFromHost(this, 0, 0, 0, 0);
		}
	}

	showWorldStatus();

	gameOn = true;

	int turn = 0;
	int numberOfPlayers = players.size();
	int numberOfProvinces = worldMap->getNumberOfProvinces();
	cout << endl << "==================================  " << endl;
	while (gameOn) {

		if ((*NM)->connectionType == "h" && turn == 0) {
			cout << endl << "============		YOUR TURN		===============  " << endl;
			currentPlayer = 0;
			startTurn(0);
			sendAllProvincesFromHost(NM);
			turn = 1;
		}
		else if ((*NM)->connectionType == "h" && turn == 1) {
			cout << endl << "============		" << players[1]->getName() << " TURN		===============  " << endl;
			sendAllProvincesClientToHost((*NM)->connectionType, NM);
			sendAllProvincesFromHost(NM);
			cout << endl << "============		" << players[2]->getName() << " TURN		===============  " << endl;
			sendAllProvincesClientToHost((*NM)->connectionType, NM);
			sendAllProvincesFromHost(NM);
			turn = 0;
		}


		else if ((*NM)->connectionType == "c1" && turn == 0) {
			cout << endl << "============		" << players[0]->getName() << " TURN		===============  " << endl;
			sendAllProvincesFromHost(NM);
			cout << endl << "============		YOUR TURN		===============  " << endl;
			currentPlayer = 1;
			startTurn(1);
			sendAllProvincesClientToHost((*NM)->connectionType, NM);
			sendAllProvincesFromHost(NM);
			turn = 1;
		}
		else if ((*NM)->connectionType == "c1" && turn == 1) {
			cout << endl << "============		" << players[2]->getName() << " TURN		===============  " << endl;
			sendAllProvincesFromHost(NM);
			turn = 0;
		}



		else if ((*NM)->connectionType == "c2" && turn == 0) {
			cout << endl << "============		" << players[0]->getName() << " TURN		===============  " << endl;
			sendAllProvincesFromHost(NM);
			cout << endl << "============		" << players[1]->getName() << " TURN		===============  " << endl;
			sendAllProvincesFromHost(NM);
			cout << endl << "============		YOUR TURN		===============  " << endl;
			currentPlayer = 2;
			startTurn(2);
			sendAllProvincesClientToHost((*NM)->connectionType, NM);
			sendAllProvincesFromHost(NM);
		}


		/*else if ((*NM)->connectionType == "c2" && turn == 1) {
		cout << endl << "============		" << players[3]->getName() << " TURN		===============  " << endl;
		sendAllProvincesFromHost(NM);
		}*/
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
			amount = 1;
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
				if (amount >= fromPtr->getNumberOfSoldiers()) {
					cout << "There has to be at least one soldier left in the city" << endl;
				}
				else if (amount > 3 || amount < 1) {
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
			player->setMoney(player->getMoney() - amount * 100);
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

	string buyCastle;
	cout << "Do you wanna buy castle (Build 50 gold, upgrade 100) (y / n): ";
	cin >> buyCastle;
	while (buyCastle == "y") {
		while (true) {
			cout << "city : ";
			string cityName;
			cin >> cityName;
			Province* city;
			worldMap->getProvinceByName(cityName, amount, city);

			if (city != NULL) {
				if (buildCastle(player->getId(), cityName)) {
					break;
				}
			}
			else {
				cout << "No such city" << endl;
			}

		}
		cout << "Your money: " << player->getMoney() << endl;
		cout << "Do you wanna buy castle (y / n): ";
		cin >> buyCastle;
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
			else if (!worldMap->hasPath(player, fromPtr, toPtr)) {
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

	/*int size = worldMap->getNumberOfProvinces();
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
	placeSoldier(players[t], curProvince->getName(), 3);
	t = (t + 1) % numberOfPlayers;
	}
	cout << "*** PROVINCES HAVE BEEN RANDOMLY DISTRIBUTED TO THE PLAYERS ***" << endl;
	delete shuffledArray;


	*/







	int size = worldMap->getNumberOfProvinces();

	/*
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
	}*/

	int t;
	int numberOfPlayers = players.size();
	Die die(numberOfPlayers);
	t = die.roll() - 1;
	int count = 0;
	for (int i = 0; i < size; i++) {
		Province* curProvince = worldMap->getProvinceByID(i);
		if (players[t]->getNumberOfProvinces() < size / 3) {
			placeSoldier(players[t], curProvince->getName(), 5);
		}
		else {
			i--;
		}
		t = die.roll() - 1;
	}

	cout << "* PROVINCES HAVE BEEN RANDOMLY DISTRIBUTED TO THE PLAYERS *" << endl;
	//delete shuffledArray;
}

int GameManager::getPlayerTurn(string _name) {
	int dummy;
	Player * user = getPlayerByName(_name, dummy);
	int index;
	for (int i = 0; i < players.size(); i++) {
		if (players[i] == user)
			index = i;
	}
	return index;

}

void GameManager::sendAllProvincesFromHostString(NetworkManager ** NM) {
	string _provinces = "";

	if ((*NM)->connectionType == "h") {
		for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
			Province * pro = worldMap->getProvinceByID(i);
			if (pro->getOwner() != NULL)
				_provinces += to_string(pro->getOwner()->getId());
			else
				_provinces += "-1";
			_provinces += "," + to_string(i);
			_provinces += "," + to_string(pro->getNumberOfSoldiers());
			_provinces += "," + to_string(pro->getCastle()->getLevel()) + ",";
		}
		(*NM)->sendStringFromHost(_provinces);
	}
	else {
		string received = (*NM)->sendStringFromHost(_provinces);
		vector<string> data = (*NM)->split(received, ',');
		for (int i = 0; i < data.size(); i += 4) {

			string dummy;
			int pId = stoi(data[i]);
			int cId = stoi(data[i + 1]);
			int count = stoi(data[i + 2]);
			int casLev = stoi(data[i + 3]);

			Player * playerChanged = this->getPlayerByID(pId, dummy);
			Province * provinceChanged = this->getWorldMap()->getProvinceByID(cId);
			//provinceChanged->setOwner(playerChanged);
			if (playerChanged != NULL)
				playerChanged->captureProvince(this->getWorldMap(), provinceChanged);
			provinceChanged->setNumberOfSoldiers(count);
			provinceChanged->getCastle()->setLevel(casLev);

		}
	}

	showWorldStatus();
}

void GameManager::sendAllProvincesClientToHostString(NetworkManager ** NM) {
	string _provinces = "";
	while (true) {
		if ((*NM)->connectionType != "h") {
			for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
				Province * pro = worldMap->getProvinceByID(i);
				if (pro->getOwner() != NULL)
					_provinces += to_string(pro->getOwner()->getId());
				else
					_provinces += "-1";
				_provinces += "," + to_string(i);
				_provinces += "," + to_string(pro->getNumberOfSoldiers());
				_provinces += "," + to_string(pro->getCastle()->getLevel()) + ",";
			}
			if ((*NM)->sendStringFromHost(_provinces) == "") {
				break;
			}
			//cout << "!!!!!!!!!!!!!!!!!!" << endl;
		}
		else if ((*NM)->connectionType == "h") {
			string received = (*NM)->sendStringFromHost(_provinces);
			vector<string> data = (*NM)->split(received, ',');
			for (int i = 0; i < data.size(); i += 4) {

				string dummy;
				int pId = stoi(data[i]);
				int cId = stoi(data[i + 1]);
				int count = stoi(data[i + 2]);
				int casLev = stoi(data[i + 3]);

				Player * playerChanged = this->getPlayerByID(pId, dummy);
				Province * provinceChanged = this->getWorldMap()->getProvinceByID(cId);
				//provinceChanged->setOwner(playerChanged);
				if (playerChanged != NULL)
					playerChanged->captureProvince(this->getWorldMap(), provinceChanged);
				provinceChanged->setNumberOfSoldiers(count);
				provinceChanged->getCastle()->setLevel(casLev);

			}
			break;
		}
		else
			break;
	}
}

void GameManager::sendAllProvincesFromHost(NetworkManager ** NM) {
	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
		Province * pro = worldMap->getProvinceByID(i);
		(*NM)->sendDataFromHost(this, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(), pro->getCastle()->getLevel());
	}
	showWorldStatus();
}

void GameManager::sendAllProvincesClientToHost(string _connectionType, NetworkManager ** NM) {
	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
		Province * pro = worldMap->getProvinceByID(i);
		(*NM)->sendDataFromClientToHost(this, _connectionType, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(), pro->getCastle()->getLevel());
	}

}

vector<string> GameManager::split(string strToSplit, char delimeter) {
	stringstream ss(strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}


void GameManager::destroyNearSoldier(Province* province) {
	if (province->getCastle()->isBuilt()) {
		Player* owner = province->getOwner();
		vector<Province*> neighbors = worldMap->getNeighbors(province);
		vector<Province*> opponent;
		int counter = -1;
		for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
			if ((*it)->getOwner() != owner && (*it)->getNumberOfSoldiers() > 1) {
				opponent.push_back((*it));
				counter++;
			}
		}
		if (counter != -1) {
			Die die(counter);
			int index = die.roll() - 1;
			opponent[index]->setNumberOfSoldiers(opponent[index]->getNumberOfSoldiers() - 1);
			cout << opponent[index]->getName() << " has lost 1 soldier due to the castle." << endl;
		}
	}
}

void GameManager::castleAttacks(Player* player) {
	cout << "CASTLE ATTACKS" << endl;
	vector<int> provinces = player->getProvinces();
	for (auto it = provinces.begin(); it != provinces.end(); it++) {
		destroyNearSoldier(worldMap->getProvinceByID((*it)));
	}
}

vector<Player*> GameManager::getPlayers() {
	return players;
}


WindowManager::WindowManager()
{

	GM = new GameManager();
	NM = new NetworkManager(this);



	zoom = 1;

	phase = INITIAL_PHASE;

	screenWidth =  GetSystemMetrics(SM_CXSCREEN) / 2; // 1920; //
	screenHeight = GetSystemMetrics(SM_CYSCREEN) / 2; // 1080; //

	cout << screenHeight << ", " << screenWidth << endl;

	leftMargin = screenWidth / 10;
	rightMargin = (screenWidth * 9) / 10;
	topMargin = screenHeight / 10;
	bottomLowerMargin = (screenHeight * 7) / 10;
	bottomUpperMargin = (screenHeight * 8) / 10;

	if (!mapImg.loadFromFile("assets/mapp.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!hoverImg.loadFromFile("assets/hover.jpeg")) {
		cout << "Unable to open file" << endl;
	}
	if (!roundedSquare.loadFromFile("assets/roundedSquare.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!lineImg.loadFromFile("assets/line.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!topPanelImg.loadFromFile("assets/topPanel.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!coinImg.loadFromFile("assets/coin.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!actionImg.loadFromFile("assets/swords.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!timerImg.loadFromFile("assets/time.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!timerImg.loadFromFile("assets/time.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!crownImg.loadFromFile("assets/crown.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!castleImg.loadFromFile("assets/castle.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!attackPhaseImg.loadFromFile("assets/attackPhase.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!placementPhaseImg.loadFromFile("assets/placementPhase.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!marketPhaseImg.loadFromFile("assets/marketPhase.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!fortifyPhaseImg.loadFromFile("assets/fortifyPhase.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!postAttackPhaseImg.loadFromFile("assets/postAttackPhase.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!sceleteImg.loadFromFile("assets/rip.png")) {
		cout << "Unable to open file" << endl;
	}
	if (!warriorImg.loadFromFile("assets/warrior.png")) {
		cout << "Unable to open file" << endl;
	}


	font;
	if (!font.loadFromFile("assets/font.ttf"))
	{
		// error...
	}

	mapTex.loadFromImage(mapImg);
	mapSprite.setTexture(mapTex);
	mainView.setSize(screenWidth, screenHeight);
	/*mainView.setViewport(sf::FloatRect(0, 0, 1, 0.8f));*/
	lowerPanel.setSize(sf::Vector2f(screenWidth, screenHeight - bottomUpperMargin));
	lowerPanel.setPosition(0, bottomUpperMargin);
	lowerPanel.setFillColor(sf::Color(255, 255, 255));
	miniMap = MiniMap(mapTex);
	topPanel = new TopBar(topPanelImg, coinImg, actionImg, timerImg, crownImg,font);


	mapSprite.setPosition(0, 0);
	mainView.setCenter(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);

	fstream file("assets/coordinates.txt");

	int color;
	string cityName;
	bool stepOne = false;
	bool stepTwo = false;
	string line;
	ifstream myfile("assets/coordinates.txt");

	listOfPhaseNotifiers.push_back(new PhaseNotifier(1,placementPhaseImg, screenWidth/2,screenHeight/2, screenWidth *0.75, 400));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(2,attackPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 400));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(3, postAttackPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.70, 400));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(4,fortifyPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 400));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(5,marketPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 400));

	battleNotifier = new BattleNotifier(actionImg, sceleteImg, warriorImg, screenWidth / 2, screenHeight / 2, font);

	if (myfile.is_open()) {
		while (getline(myfile, line)) {

			vector<string> temp = GM->split(line, ',');
			listOfArmyBage.push_back(new ArmyBage(roundedSquare, stoi(temp[1]), stoi(temp[2]), temp[0], font));
			listOfCastleBage.push_back(new CastleBage(castleImg, stoi(temp[3]), stoi(temp[4]), temp[0]));
			//listOfArmyBage.push_back(ArmyBage(temp));
			//cityName = prov[0];
			//color = stoi(prov[1]);
			//createProvince(cityName, to_string(color));
			//colorLookUpTable.insert(pair<int, string>(color, cityName));
			//cout << cityName << " " << color << " loaded. " << endl;
		}


		myfile.close();
	}
	lineForProvinces = new LineBetweenProvinces(lineImg);


	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));

	

	for (int i = 0; i < 6; i++) {
		Text* b = new Text();
		b->setFont(font);
		b->setCharacterSize(15);
		b->setString("\n");
		b->setOrigin(0, b->getLocalBounds().height);
		playerStatus.push_back(b);
	}

	playerStatus[0]->setPosition(10, bottomUpperMargin - 10);
	playerStatus[1]->setPosition(10, playerStatus[0]->getPosition().y - playerStatus[0]->getLocalBounds().height - 10);
	playerStatus[2]->setPosition(10, playerStatus[1]->getPosition().y - playerStatus[1]->getLocalBounds().height - 10);
	playerStatus[3]->setPosition(10, playerStatus[2]->getPosition().y - playerStatus[2]->getLocalBounds().height - 10);
	playerStatus[4]->setPosition(10, playerStatus[3]->getPosition().y - playerStatus[3]->getLocalBounds().height - 10);
	playerStatus[5]->setPosition(10, playerStatus[4]->getPosition().y - playerStatus[4]->getLocalBounds().height - 10);

	playerStatus[0]->setFillColor(Color::Red);
	playerStatus[1]->setFillColor(Color::Blue);
	playerStatus[2]->setFillColor(Color::Green);
	playerStatus[3]->setFillColor(Color::Cyan);
	playerStatus[4]->setFillColor(Color::Black);
	playerStatus[5]->setFillColor(Color::Magenta);


	wheelStr.push_back("Draw bonus card");
	wheelStr.push_back("Bankruptcy");
	wheelStr.push_back("250 Gold");
	wheelStr.push_back("Pass");
	wheelStr.push_back("Build Castle");
	wheelStr.push_back("Take somebody's province");
	wheelStr.push_back("Province Lost");
	wheelStr.push_back("Take 3 soldiers");


	images.push_back(new MyImage("soldier.png"));
	images.push_back(new MyImage("wheel.png"));
	images.push_back(new MyImage("triangle.png"));
	images.push_back(new MyImage("castle-icon.png"));

	images[3]->setInitialPosition(lowerPanel.getPosition().x + 30, lowerPanel.getPosition().y + lowerPanel.getSize().y / 2 - images[3]->getTextureRect().height / 2);
	images[3]->setPosition(images[3]->getInitialPosition());
	images[3]->setScale(0.8, 0.8);

	images[0]->setInitialPosition(images[3]->getInitialPosition().x + images[3]->getSize().x + 20, lowerPanel.getPosition().y + lowerPanel.getSize().y / 2 - images[0]->getTextureRect().height / 2);
	images[0]->setPosition(images[0]->getInitialPosition());



	images[1]->setInitialPosition(screenWidth / 2, bottomUpperMargin / 2);
	images[1]->setOrigin(images[1]->getSize().x / 2, images[1]->getSize().y / 2);
	images[1]->setPosition(images[1]->getInitialPosition());
	images[1]->setScale(0.7, 0.7);


	images[2]->rotate(90);
	images[2]->setOrigin(images[2]->getSize().x / 2, images[2]->getSize().y / 2);
	images[2]->setInitialPosition(images[1]->getInitialPosition().x + 280, images[1]->getInitialPosition().y);
	//images[2]->setInitialPosition(screenWidth / 2, bottomUpperMargin / 2);
	images[2]->setPosition(images[2]->getInitialPosition());
	images[2]->setScale(0.3, 0.3);
	int numberTextSize = 30;

	buttons[2]->setTextSize(numberTextSize);
	buttons[2]->setSize(numberTextSize, numberTextSize);
	buttons[2]->setText("-");
	buttons[2]->setPosition(images[0]->getInitialPosition().x + images[0]->getSize().x + 20, bottomUpperMargin + 10);
	buttons[2]->setTextColor(sf::Color::Black);
	buttons[2]->setFillColor(sf::Color::Red);
	buttons[2]->setOutlineThickness(2);
	buttons[2]->setOutlineColor(Color::Black);
	buttons[2]->setFlag(true);



	buttons[4]->setTextSize(numberTextSize);
	buttons[4]->setSize(numberTextSize, numberTextSize);
	buttons[4]->setText("1");
	buttons[4]->setPosition(buttons[2]->getPosition().x + buttons[2]->getGlobalBounds().width, buttons[2]->getPosition().y);
	buttons[4]->setTextColor(sf::Color::Black);
	buttons[4]->setFillColor(sf::Color::Red);
	buttons[4]->setOutlineThickness(2);
	buttons[4]->setOutlineColor(Color::Black);
	buttons[4]->setFlag(true);

	buttons[3]->setTextSize(numberTextSize);
	buttons[3]->setSize(numberTextSize, numberTextSize);
	buttons[3]->setText("+");
	buttons[3]->setPosition(buttons[4]->getPosition().x + buttons[4]->getGlobalBounds().width, buttons[2]->getPosition().y);
	buttons[3]->setTextColor(sf::Color::Black);
	buttons[3]->setFillColor(sf::Color::Red);
	buttons[3]->setOutlineThickness(2);
	buttons[3]->setOutlineColor(Color::Black);
	buttons[3]->setFlag(true);


	buttons[0]->setSize(100, 40);
	buttons[0]->setPosition(buttons[2]->getPosition().x , buttons[2]->getPosition().y +buttons[2]->getSize().y + 20);
	buttons[0]->setText("Next Phase");
	buttons[0]->setTextSize(10);
	buttons[0]->setTextColor(sf::Color::White);
	buttons[0]->setFillColor(sf::Color::Blue);
	buttons[0]->setOutlineThickness(2);
	buttons[0]->setOutlineColor(Color::Black);
	buttons[0]->setFlag(false);



	buttons[1]->setSize(100, numberTextSize);
	buttons[1]->setText("Attack");
	buttons[1]->setPosition(buttons[0]->getPosition().x + buttons[0]->getSize().x + 20, buttons[2]->getPosition().y);
	//buttons[1]->setPosition(buttons[3]->getPosition().x + buttons[3]->getSize().x + 20, lowerPanel.getPosition().y + lowerPanel.getSize().y / 2 + 10);
	buttons[1]->setTextSize(numberTextSize-2);
	buttons[1]->setTextColor(sf::Color::White);
	buttons[1]->setFillColor(sf::Color::Red);
	buttons[1]->setOutlineThickness(2);
	buttons[1]->setOutlineColor(Color::Black);
	buttons[1]->setFlag(false);



	buttons[5]->setSize(100, 40);
	buttons[5]->setText("Host");
	buttons[5]->setPosition(screenWidth / 2, screenHeight / 2);
	buttons[5]->setTextSize(20);
	buttons[5]->setTextColor(sf::Color::White);
	buttons[5]->setFillColor(sf::Color::Red);

	buttons[6]->setSize(100, 40);
	buttons[6]->setText("C1");
	buttons[6]->setPosition(screenWidth / 2 + 120, screenHeight / 2);
	buttons[6]->setTextSize(20);
	buttons[6]->setTextColor(sf::Color::White);
	buttons[6]->setFillColor(sf::Color::Red);

	buttons[7]->setSize(100, 40);
	buttons[7]->setText("C2");
	buttons[7]->setTextSize(20);
	buttons[7]->setPosition(screenWidth / 2 + 120, screenHeight / 2 + 60);
	buttons[7]->setTextColor(sf::Color::White);
	buttons[7]->setFillColor(sf::Color::Red);

	buttons[8]->setSize(100, 40);
	buttons[8]->setText("Start");
	buttons[8]->setPosition(screenWidth / 2, screenHeight / 2 + 60);
	buttons[8]->setTextSize(20);
	buttons[8]->setTextColor(sf::Color::White);
	buttons[8]->setFillColor(sf::Color::Red);

	buttons[9]->setSize(100, 40);
	buttons[9]->setText("Turn Wheel");
	buttons[9]->setTextSize(20);
	buttons[9]->setPosition(buttons[1]->getPosition().x, buttons[1]->getPosition().y + buttons[1]->getSize().y + 20);
	buttons[9]->setTextColor(sf::Color::Black);
	buttons[9]->setFillColor(sf::Color::Green);
	buttons[9]->setOutlineThickness(2);
	buttons[9]->setOutlineColor(Color::Black);
	buttons[9]->setFlag(false);


	provinceNameTxt.setFont(font);
	provinceNameTxt.setCharacterSize(20);
	provinceNameTxt.setFillColor(sf::Color(0, 0, 0));
	provinceNameTxt.setPosition(buttons[1]->getPosition().x + buttons[1]->getSize().x + 20, bottomUpperMargin);

	infoText.setFont(font);
	infoText.setCharacterSize(20);
	infoText.setFillColor(sf::Color::Black);
	infoText.setPosition(buttons[1]->getPosition().x + buttons[1]->getSize().x + 20, bottomUpperMargin + 40);

	GM->windowManager = this;
}

WindowManager::~WindowManager() {

}

void WindowManager::displayPlayerStatus() {
	vector<Player*> players = GM->getPlayers();
	int size = players.size();

	for (int i = 0; i < size; i++) {
		string msg = "";
		msg += players[i]->getName() + " -> ";
		msg += to_string(players[i]->getProvinces().size()) + " Provinces, ";
		msg += to_string(players[i]->getMoney()) + " Gold";
		playerStatus[i]->setString(msg);
	}
}

void WindowManager::menuScreen(RenderWindow & window, Event & event) {
	static int x = 0;
	while (window.pollEvent(event)) {
		cout << "we are checking" << x++ << endl;
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				checkClickEvents(event);
			}
		}
	}

	window.setView(mainView);

	window.clear(sf::Color::Black);

	window.setView(window.getDefaultView());

	buttons[5]->draw(window);
	buttons[6]->draw(window);
	buttons[7]->draw(window);
	buttons[8]->draw(window);

	window.display();
}

void WindowManager::multGameLan(RenderWindow & window, Event & event) {
	while (window.pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (zoom > 0.5 && event.mouseWheelScroll.delta > 0) {
				mainView.zoom(0.80);
				zoom *= 0.8;
				//mainView.setCenter(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
			}
			else if (zoom < 2 && event.mouseWheelScroll.delta < 0) {
				mainView.zoom(1.25);
				zoom *= 1.25;
				//mainView.setCenter(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				checkClickEvents(event);
				string provinceName = getProvinceName(window, mouse);
				if (provinceName != "") {
					int index = -1; Province* ptr;
					GM->getWorldMap()->getProvinceByName(provinceName, index, ptr);
					if (event.mouseButton.y < bottomUpperMargin)
						provinceClicked(index);
				}
			}
		}
	}




	float speed = 3;

	if (counter > 10) {

		if (turnWheel) {
			images[1]->rotate(2.5);
			rotateAmount = rotateAmount + 2.5;
			if (rotateAmount == 360)
				rotateAmount = 0;
		}

		sf::Vector2i mousePos = mouse.getPosition(window);
		if (mousePos.x < leftMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().x >= mainView.getSize().x / 2) {
				mainView.move(-zoom * speed, 0);
			}
		}
		else if (mousePos.x > rightMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().x < mapTex.getSize().x - mainView.getSize().x / 2) {
				mainView.move(zoom* speed, 0);
			}
		}
		if (mousePos.y < topMargin) {
			if (mainView.getCenter().y >= mainView.getSize().y / 2) {
				mainView.move(0, -zoom * speed);
			}
		}
		else if (mousePos.y > bottomLowerMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().y - lowerPanel.getSize().y * zoom < mapTex.getSize().y - mainView.getSize().y / 2) {
				mainView.move(0, zoom * speed);
			}
		}
		counter = 0;
	}


	counter++;
	//mapSprite.setPosition(mainView.getCenter().x, mainView.getCenter().y);
	//lowerPanel.setPosition(mainView.getCenter().x - screenWidth / 2, mainView.getCenter().y + screenHeight * 3 / 10);
	window.setView(mainView);

	window.clear(sf::Color(224, 253, 255));
	window.draw(mapSprite);



	drawAllArmies(window, event);
	drawAllCastles(window, event);
	/*for (auto it = castles.begin(); it != castles.end(); it++) {
		window.draw(*(*it));
	}*/




	window.setView(window.getDefaultView());
	window.draw(lowerPanel);
	dragObject(window, event, 3);
	dragObject(window, event, 0);
	window.draw(*images[0]);
	//window.draw(*images[1]);
	//window.draw(*images[2]);
	window.draw(*images[3]);

	window.draw(provinceNameTxt);
	window.draw(infoText);

	for (int i = 0; i < buttons.size(); i++) {
		if (i != 5 && i != 6 && i != 7 && i != 8)
			buttons[i]->draw(window);
	}


	//miniMap Staff
	window.setView(miniMap);
	miniMap.update(mainView);
	miniMap.draw(window);
	window.setView(window.getDefaultView());
	/*window.setView(miniMap);
	miniMap.setCenter(mapTex.getSize().x / 2, mapTex.getSize().y / 2);
	mapSprite.setTextureRect(IntRect(0, 0, mapTex.getSize().x, mapTex.getSize().y));
	RectangleShape miniMapRectangle;
	miniMapRectangle.setFillColor(Color::Transparent);
	miniMapRectangle.setOutlineThickness(20);
	miniMapRectangle.setOutlineColor(Color::Black);
	miniMapRectangle.setSize(mainView.getSize());
	miniMapRectangle.setPosition(mainView.getCenter().x - mainView.getSize().x / 2, mainView.getCenter().y - mainView.getSize().y / 2);
	miniMap.setViewport(FloatRect(float(0.8), float(0.8), 0.2, 0.2));
	window.draw(mapSprite);
	window.draw(miniMapRectangle);
*/

	window.display();

}

void WindowManager::multGameComp(RenderWindow & window, Event & event) {
	time = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	time = time / 1200;
	if (time > 20)
		time = 20;
	
	while (window.pollEvent(event))
	{
		screenWidth = window.getSize().x; // 1920; //
		screenHeight = window.getSize().y ; // 1080; //
		//::ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
		leftMargin = screenWidth / 10;
		rightMargin = (screenWidth * 9) / 10;
		topMargin = screenHeight / 10;
		bottomLowerMargin = (screenHeight * 7) / 10;
		bottomUpperMargin = (screenHeight * 8) / 10;

		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			if (zoom > 0.5 && event.mouseWheelScroll.delta > 0) {
				mainView.zoom(0.80);
				zoom *= 0.8;
				//mainView.setCenter(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
			}
			else if (zoom < 2 && event.mouseWheelScroll.delta < 0) {
				mainView.zoom(1.25);
				zoom *= 1.25;
				//mainView.setCenter(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				checkClickEvents(event);
				string provinceName = getProvinceName(window, mouse);
				if (provinceName != "") {
					int index = -1; Province* ptr;
					GM->getWorldMap()->getProvinceByName(provinceName, index, ptr);
					if (event.mouseButton.y < bottomUpperMargin)
						provinceClicked(index);
				}
			}
		}
	}



	// DO NOT CHANGE THIS PART
	// float speed = 3;
	if (counter > 10) {
		if (turnWheel && rotateRandom == 0) {
			if (rotateStep > 0)
				rotateStep -= 0.01;
			else {
				rotateStep = 0;
				turnWheel = false;
				handleWheel();
			}
				
		}
		else {
			rotateRandom--;
		}
		
		if (turnWheel) {
			images[1]->rotate(rotateStep);
			rotateAmount += rotateStep;
			if (rotateAmount >= 360) {
				rotateAmount = 0;
			}
		}
		counter = 0;
		// TEST
		displayPlayerStatus();
	}

	sf::Vector2i mousePos = mouse.getPosition(window);
	if (insideTheWindow(mousePos)) {
		if (mousePos.x < leftMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().x >= mainView.getSize().x / 2) {
				mainView.move(-zoom * time, 0);
			}
		}
		else if (mousePos.x > rightMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().x < mapTex.getSize().x - mainView.getSize().x / 2) {
				mainView.move(zoom* time, 0);
			}
		}
		if (mousePos.y < topMargin) {
			if (mainView.getCenter().y >= mainView.getSize().y / 2) {
				mainView.move(0, -zoom * time);
			}
		}
		else if (mousePos.y > bottomLowerMargin && mousePos.y < bottomUpperMargin) {
			if (mainView.getCenter().y - lowerPanel.getSize().y * zoom < mapTex.getSize().y - mainView.getSize().y / 2) {
				mainView.move(0, zoom * time);
			}
		}
	}

	counter++;
	//mapSprite.setPosition(mainView.getCenter().x, mainView.getCenter().y);
	//lowerPanel.setPosition(mainView.getCenter().x - screenWidth / 2, mainView.getCenter().y + screenHeight * 3 / 10);
	window.setView(mainView);

	window.clear(sf::Color(224, 253, 255));

	window.draw(mapSprite);
	lineForProvinces->draw(window,time);
	drawAllArmies(window, event);
	drawAllCastles(window, event);
	//for (auto it = castles.begin(); it != castles.end(); it++) {
	//	window.draw(*(*it));
	//}


	window.setView(window.getDefaultView());
	window.draw(lowerPanel);
	battleNotifier->draw(window, time);
	for (int i = 0; i < listOfPhaseNotifiers.size(); i++) {
		if (phase == listOfPhaseNotifiers[i]->phaseId)
			listOfPhaseNotifiers[i]->activate();
		else
			listOfPhaseNotifiers[i]->deactivate();
	}

	for (int i = 0; i < listOfPhaseNotifiers.size(); i++) {
		listOfPhaseNotifiers[i]->draw(window,time);
	}

	dragObject(window, event, 3);
	dragObject(window, event, 0);
	window.draw(*images[0]);

	// ===================== TURN WHEEL
	if (turnWheel) {
		window.draw(*images[1]);
		window.draw(*images[2]);
	}
	// =====================



	window.draw(*images[3]);

	window.draw(provinceNameTxt);
	window.draw(infoText);

	for (int i = 0; i < buttons.size(); i++) {
		if (i != 5 && i != 6 && i != 7 && i != 8) {
			if(buttons[i]->getFlag())
				buttons[i]->draw(window);
		}
	}

	for (int i = 0; i < playerStatus.size(); i++) {
		window.draw(*playerStatus[i]);
	}

	int currentPlayer = GM->currentPlayer;
	string currentPlayerName;
	Player * curPlayer = GM->getPlayerByID(currentPlayer, currentPlayerName);


	topPanel->update(curPlayer->getMoney(), curPlayer->getLeftSoldier(), totalTurn, currentPlayerName);
	topPanel->draw(window);
	//miniMap Staff
	window.setView(miniMap);
	miniMap.update(mainView);
	miniMap.draw(window);

	window.display();
}

bool WindowManager::insideTheWindow(Vector2i mousePos) {
	return mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x <= screenWidth && mousePos.y <= screenHeight;
}

void WindowManager::createWindow() {

	window.setKeyRepeatEnabled(false);
	window.create(sf::VideoMode(screenWidth, screenHeight), "Risk",Style::Default);
	window.setPosition(Vector2i(0,0))  ;


	while (window.isOpen()) {
		sf::Event event;

		if (page == 0) {
			menuScreen(window, event);
			continue;
		}

		else if (page == 1) {
			multGameLan(window, event);
			continue;
		}

		else if (page == 2) {
			multGameComp(window, event);
			continue;
		}
	}
}

string WindowManager::getProvinceByColor(int color) {
	return "";
}

int WindowManager::getPixelColor(int x, int y) {
	cout << "Mouse : " << x << ", " << y << endl;
	cout << "Mouse : " << x << ", " << y << endl;
	return (int)mapImg.getPixel(x, y).toInteger();
}

string WindowManager::getProvinceName(sf::RenderWindow & window, sf::Mouse & m) {
	
	sf::Vector2i PixelPos = m.getPosition(window);
	sf::Vector2f MousePos = window.mapPixelToCoords(PixelPos, mainView);
	if (MousePos.x <= mapImg.getSize().x && MousePos.y <= mapImg.getSize().y) {
		cout << "XXX   " << MousePos.x << "      YYY " << MousePos.y << endl;
		int colorInInt = (int)mapImg.getPixel(MousePos.x, MousePos.y).toInteger();
		cout << colorInInt << "**********************" << endl;
		
		auto it = GM->colorLookUpTable.find(colorInInt);
		if (it != GM->colorLookUpTable.end())
			return GM->colorLookUpTable[colorInInt];
	}
	return "";
}

void WindowManager::checkClickEvents(sf::Event & e) {
	int id = 0;

	for (auto it = buttons.begin(); it != buttons.end(); it++) {
		if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + (*it)->getSize().x &&
			(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + (*it)->getSize().y) {
			buttonClicked(id, e, window);
			return;
		}
		id++;
	}
	id = 0;
	for (auto it = images.begin(); it != images.end(); it++) {
		if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + (*it)->getSize().x &&
			(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + (*it)->getSize().y) {
			imageClicked(id);
			return;
		}
		id++;
	}


}

void WindowManager::buttonClicked(int id, sf::Event &event, sf::RenderWindow & window) {
	cout << "button clicked" << endl;
	if (page == MENU_SCREEN) {
		cout << "clickked weghwioefng" << endl;
		if (id == 5) {
			//userName = "host";
			//NM->createNetwork(&GM,"h", userName);

			playerCount = 3; // will be taken from user
			const string name = "player";
			for (int i = 0; i < playerCount; i++) {
				this->GM->addPlayer(name + to_string(i + 1));
			}
			if (_randomPlacement == true) {
				GM->randomPlacement();
				string dummy;
				for (int i = 0; i < playerCount; i++)
					GM->getPlayerByID(i, dummy)->setLeftSoldier(GM->getPlayerByID(i, dummy)->getNumberOfProvinces() / 3);
				phase = PLACEMENT_PHASE;
			}
			else {
				phase = INITIAL_PHASE;
			}
			cout << "clickked weghwioefng" << endl;
			turn = 0;
			page = COMPUTER_GAME_SCREEN;
		}
		else if (id == 6) {
			userName = "client1";
			NM->createNetwork(&GM, "c1", userName);
		}
		else if (id == 7) {
			userName = "client2";
			NM->createNetwork(&GM, "c2", userName);
		}
		else if (id == 8) {
			NM->startGame();
			userTurn = GM->getPlayerTurn(userName);
			cout << "MY TURN " << userTurn << endl;
			GM->randomPlacement();
			GM->sendAllProvincesFromHostString(&NM);
			phase = INITIAL_PHASE;
			page = 1;
		}
		return;
	}

	string dummy;
	Player* player = GM->getPlayerByID(GM->currentPlayer, dummy);
	if (id == NEXT_PHASE_BUTTON) {
		if (phase == INITIAL_PHASE) {
			/*  player->setLeftSoldier(player->getNumberOfProvinces() / 3);
			if (page == 2 )
			phase = PLACEMENT_PHASE; //YOU WILL NOT BE ABLE TO CLICK ANYTHING IN INITIAL PHASE JUST SOLDIER */
		}
		else if (phase == PLACEMENT_PHASE) {
			if(player->getLeftSoldier() == 0)
				phase = ATTACKING_PHASE;
			buttons[ATTACK_BUTTON]->setText("Attack");
			buttons[NEXT_PHASE_BUTTON]->setText("Fortify Phase");

		}
		else if (phase == ATTACKING_PHASE) {
			phase = FORTIFY_PHASE;

			buttons[NEXT_PHASE_BUTTON]->setText("Market Phase");
			buttons[ATTACK_BUTTON]->setText("Fortify");
		}
		else if (phase == FORTIFY_PHASE) {
			phase = MARKET_PHASE;
			buttons[TURN_WHEEL_BUTTON]->setFlag(true);
			buttons[ATTACK_BUTTON]->setFlag(false);
			if (page == 1) {
				if (turn = 0)
					this->GM->sendAllProvincesFromHostString(&NM);
				else {
					this->GM->sendAllProvincesClientToHostString(&NM);
					this->GM->sendAllProvincesFromHostString(&NM);
				}
			}
			// ------------------------------ CASTLE ATTACKS BEGINS
			string dum;
			cout << "End of turn : " << turn << endl;
			GM->castleAttacks(GM->getPlayerByID(turn, dum));
			// ------------------------------ CASTLE ATTACKS ENDS

			buttons[NEXT_PHASE_BUTTON]->setText("End Turn");
			int soldierPrice = 50;
			soldierAmount = player->getMoney() / soldierPrice;
			buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
			
		}
		else if(phase == MARKET_PHASE) {
			turn++;
			if (turn == playerCount) {
				turn = 0;
				totalTurn++;
			}
			if (page == COMPUTER_GAME_SCREEN) {
				string dummy;
				for (int i = 0; i < playerCount; i++)
					GM->getPlayerByID(i, dummy)->setLeftSoldier(GM->getPlayerByID(i, dummy)->getNumberOfProvinces() / 3);
				phase = PLACEMENT_PHASE;

			}
			else if (page == GAME_SCREEN) {
				cout << "Player" << turn + 1 << " 's turn!!" << endl;
				phase = END_TURN;
			}
			cout << "Player" << turn + 1 << " 's turn!!" << endl;
			phase = PLACEMENT_PHASE;
			buttons[TURN_WHEEL_BUTTON]->setFlag(false);
			buttons[ATTACK_BUTTON]->setFlag(false);
			buttons[NEXT_PHASE_BUTTON]->setFlag(false);
			
			GM->currentPlayer = turn;
			wheel = false;
			countForWheel = 0;
			takeCastle = false;
			wonSoldier = false;

		}
		else if (phase == END_TURN) {
		}

		cout << "Phase: " << phase << endl;
	}

	else if (id == ATTACK_BUTTON) {
		if (phase == ATTACKING_PHASE) {
			if (isProvinceClicked == 2) {
				if (GM->attack(player, second->getOwner(), first, second, soldierAmount)) {
					wheel = true;
					phase = POST_ATTACK; //change phase 
					buttons[ATTACK_BUTTON]->setText("Place");
					provinceNameTxt.setString("Enter the number of soldiers you want to place on this city:");
					soldierAmount = first->getNumberOfSoldiers() - 1;
					buttons[NUMBER_TEXT]->setText(to_string(soldierAmount)); 
					cout << "lelelellelee" << endl;
				}
				else if (first->getNumberOfSoldiers() > 3) {
					soldierAmount = 3;
					buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
					displayProvinceInfo(first);
				}
				else {
					soldierAmount = first->getNumberOfSoldiers() - 1;
					buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
					displayProvinceInfo(first);
				}
				

			}
		}
		else if (phase == POST_ATTACK && second->getNumberOfSoldiers() == 0) {
			GM->fortify(player, first, second, soldierAmount);
			buttons[ATTACK_BUTTON]->setText("Attack");
			phase = ATTACKING_PHASE;
			displayProvinceInfo(second);
		}
		else if (phase == FORTIFY_PHASE) {
			soldierAmount = first->getNumberOfSoldiers() - 1;
			buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
			if (GM->fortify(player, first, second, soldierAmount)) {
				displayProvinceInfo(second);
			}
		}
		else if (phase == END_TURN) {


			/*if (page == 1) {
			if (turn == 0)
			this->GM->sendAllProvincesFromHostString(&NM);
			else {

			if (userTurn == 0) {
			this->GM->sendAllProvincesClientToHostString(&NM);
			this->GM->sendAllProvincesFromHostString(&NM);
			}
			else
			this->GM->sendAllProvincesFromHostString(&NM);
			}
			}
			turn++;
			if (turn == playerCount -1 )
			turn = 0;
			if (turn == userTurn) {
			if (GM->getWorldMap()->ownerCount() < 42)
			phase = INITIAL_PHASE;
			else {
			player->setLeftSoldier(player->getNumberOfProvinces() / 3);
			phase = PLACEMENT_PHASE;
			}

			}*/
		}
		lineForProvinces->setVisible(false);
	}

	else if (id == INC_BUTTON) {
		if (phase == ATTACKING_PHASE) {
			if (soldierAmount < 3) {
				soldierAmount++;
			}
		}
		else if (phase == POST_ATTACK || phase == FORTIFY_PHASE) {
			if (first != NULL) {
				if (soldierAmount < first->getNumberOfSoldiers() - 1) {
					soldierAmount++;
				}
			}
		}
		else if (phase == PLACEMENT_PHASE) {
			if (player->getLeftSoldier() > soldierAmount) {
				soldierAmount++;
			}
		}

		else if (phase == MARKET_PHASE && !wonSoldier) {
			int soldierPrice = 50;
			if (player->getMoney() / soldierPrice > soldierAmount)
				soldierAmount++;
		}
		buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
	}

	else if (id == DEC_BUTTON) {
		if (phase == ATTACKING_PHASE) {
			if (soldierAmount > 1) {
				soldierAmount--;
			}
		}

		else if (phase == POST_ATTACK || phase == FORTIFY_PHASE || phase == PLACEMENT_PHASE || (phase == MARKET_PHASE && !wonSoldier)) {
			if (soldierAmount > 1) {
				soldierAmount--;
			}
		}
		buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
	}
	//  
	else if (id == TURN_WHEEL_BUTTON && countForWheel < 1 && phase == MARKET_PHASE && wheel) {
		countForWheel++;
		buttons[TURN_WHEEL_BUTTON]->setFlag(false);
		wonSoldier = false;
		string temp;
		
		if (turnWheel) {
			turnWheel = false;
		}
		else {
			rotateStep = 2.5;
			turnWheel = true;
			Die die(200);
			rotateRandom = die.roll() + 100;
		}
	}
}

void WindowManager::imageClicked(int id) {
	if (id == 0) {
		cout << "Image clicked" << endl;
	}
}

void WindowManager::handleWheel() {
	string temp;
	int index = ((int)(rotateAmount / 45)) % 8;
			cout << "Rotation: " << rotateAmount << wheelStr[index] << endl;
			if (index == 0) {
				//draw bonus card
			}
			else if (index == 1) {
				//bankrupt
				GM->getPlayerByID(turn, temp)->setMoney(0);
			}
			else if (index == 2) {
				//250 gold
				GM->getPlayerByID(turn, temp)->setMoney(GM->getPlayerByID(turn, temp)->getMoney() + 250);
			}
			else if (index == 3) {
				//pass
			}
			else if (index == 4) {
				//build castle
			
				GM->getPlayerByID(turn, temp)->setMoney(GM->getPlayerByID(turn, temp)->getMoney() + 50);
				images[3]->inMove = true;
				takeCastle = true;
				
			}
			else if (index == 5) {
				//take province
				Die die(GM->getWorldMap()->getNumberOfProvinces());
				int random = die.roll();
				Province* prov = GM->getWorldMap()->getProvinceByID(random);
				string pname;
				GM->getPlayerByID(turn, pname);
				while (prov->getOwner()->getName() == pname) {
					int random = die.roll();
					prov = GM->getWorldMap()->getProvinceByID(random);
					GM->getPlayerByID(turn, pname);
				}
				prov->setOwner(GM->getPlayerByID(turn, pname));
				cout << pname << "gets " << prov->getName() << endl;
			}
			else if (index == 6) {
				//give province
				Die die(GM->getWorldMap()->getNumberOfProvinces());
				int random = die.roll();
				Province* prov = GM->getWorldMap()->getProvinceByID(random);
				string pname;
				GM->getPlayerByID(turn, pname);
				while (prov->getOwner()->getName() != pname) {
					int random = die.roll();
					prov = GM->getWorldMap()->getProvinceByID(random);
					GM->getPlayerByID(turn, pname);
				}
				prov->setOwner(GM->getPlayerByID((turn+1)%3, pname));
				cout << pname << "gets " << prov->getName() << endl;
			}
			else if (index == 7) {
				//take 3 soldiers
				GM->getPlayerByID(turn, temp)->setLeftSoldier(GM->getPlayerByID(turn, temp)->getLeftSoldier() + 3);
				string dum;
				soldierAmount = GM->getPlayerByID(turn, dum)->getLeftSoldier();
				buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
				wonSoldier = true;
			}
}

void WindowManager::provinceClicked(int id) {
	Province* city = GM->getWorldMap()->getProvinceByID(id);
	Player* player = GM->getPlayers()[turn];
	
	//
	//if (isProvinceClicked == 0) {
	//	for (int i = 0; i < listOfArmyBage.size(); i++) {
	//		if (listOfArmyBage[i]->nameOfProvince == city->getName())
	//			listOfArmyBage[i]->setScale(2, 2);
	//		else
	//			listOfArmyBage[i]->setScale(1, 1);
	//	}
	//}

	//if (phase == ATTACKING_PHASE || phase == FORTIFY_PHASE) {
	//	if (isProvinceClicked == 0) {
	//		first = city;
	//		second = NULL;
	//		isProvinceClicked = 1;
	//		if (page == 1)
	//			provinceNameTxt.setString(first->getName());

	//		//-------------------------------------------------
	//		//This code is to Scale a square with army size of a province.
	//		highlight(city);
	//		//--------------------------------------------------


	//		//----------------------------------
	//		//This is when you release from the probince. Can be changed????
	//		lineForProvinces->setVisible(false);
	//		//-------------------------------
	//	}
	//	else if (isProvinceClicked == 1) {
	//		cout << "we are here" << endl;
	//		second = city;
	//		isProvinceClicked = 2;

	//		//------------------------------------------------------
	//		//this part is to draw line between two provinces
	//		Vector2f firstCoordinates, secondCoordinates;
	//		for (int i = 0; i < listOfArmyBage.size(); i++) {
	//			if (listOfArmyBage[i]->nameOfProvince == first->getName())
	//				firstCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
	//			if (listOfArmyBage[i]->nameOfProvince == second->getName())
	//				secondCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
	//		}

	//		lineForProvinces->setCoordinates(firstCoordinates, secondCoordinates);
	//		lineForProvinces->setVisible(true);
	//		//--------------------------------------------------------


	//		//-------------------------------------------------
	//		//This code is to Scale a square with army size of a province.
	//		for (int i = 0; i < listOfArmyBage.size(); i++) {
	//			if (listOfArmyBage[i]->nameOfProvince == first->getName())
	//				listOfArmyBage[i]->setScale(2, 2);
	//			else if (listOfArmyBage[i]->nameOfProvince == second->getName())
	//				listOfArmyBage[i]->setScale(2, 2);
	//			else
	//				listOfArmyBage[i]->setScale(1, 1);
	//		}
	//		//-------------------------------------------------



	//		if (phase == ATTACKING_PHASE)
	//			provinceNameTxt.setString(first->getName() + " attacks to " + second->getName());
	//		else
	//			provinceNameTxt.setString("From " + first->getName() + " to " + second->getName());
	//	}
	//	else if (isProvinceClicked == 2) {
	//		second = NULL;
	//		first = city;
	//		isProvinceClicked = 1;
	//		provinceNameTxt.setString(first->getName());
	//		lineForProvinces->setVisible(false);
	//		//-------------------------------------------------
	//		//This code is to Scale a square with army size of a province.
	//		for (int i = 0; i < listOfArmyBage.size(); i++) {
	//			if (listOfArmyBage[i]->nameOfProvince == city->getName())
	//				listOfArmyBage[i]->setScale(2, 2);
	//			else
	//				listOfArmyBage[i]->setScale(1, 1);
	//		}
	//		//-------------------------------------------------

	//	}
	//	if (phase != FORTIFY_PHASE) {
	//		if (first->getNumberOfSoldiers() > 3)
	//			soldierAmount = 3;
	//		else
	//			soldierAmount = first->getNumberOfSoldiers() - 1;
	//	}
	//	else
	//		soldierAmount = first->getNumberOfSoldiers() - 1;
	//	buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
	//}
	//else if (phase == POST_ATTACK) {
	//	soldierAmount = first->getNumberOfSoldiers() - 1;
	//	buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));

	//	//-------------------------------------------------
	//	//This code is to Scale a square with army size of a province.
	//	for (int i = 0; i < listOfArmyBage.size(); i++) {
	//		if (listOfArmyBage[i]->nameOfProvince == city->getName())
	//			listOfArmyBage[i]->setScale(2, 2);
	//		else
	//			listOfArmyBage[i]->setScale(1, 1);
	//	}
	//	//-------------------------------------------------
	//}

	if (phase == PLACEMENT_PHASE){}
	else if (phase == ATTACKING_PHASE){
		if (isProvinceClicked == 0){
			if (city->getOwner() == player) {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
			}
			else {
				resetHighlights();
			}
			lineForProvinces->setVisible(false);
		}
		else if (isProvinceClicked == 1){
			if (city->getOwner() != player) {
				second = city;
				isProvinceClicked = 2;
				resetHighlights();
				highlight(first);
				highlight(second);
				drawLine(first, second);
				provinceNameTxt.setString(first->getName() + " attacks to " + second->getName());
			}
			else {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
				lineForProvinces->setVisible(false);
				
			}
		}
		else if (isProvinceClicked == 2){
			if (city->getOwner() == player) {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
				lineForProvinces->setVisible(false);
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
			}
		}
	}
	else if (phase == FORTIFY_PHASE) {
		if (isProvinceClicked == 0) {
			if (city->getOwner() == player) {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
			}
			else {
				resetHighlights();
			}
			lineForProvinces->setVisible(false);
		}
		else if (isProvinceClicked == 1) {
			if (city->getOwner() == player) {
				second = city;
				isProvinceClicked = 2;
				resetHighlights();
				highlight(first);
				highlight(second);
				drawLine(first, second);
				provinceNameTxt.setString("From " + first->getName() + " to " + second->getName());
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				if (page == 1)
					provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
			}
		}
		else if (isProvinceClicked == 2) {
			if (city->getOwner() == player) {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
				lineForProvinces->setVisible(false);
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
			}
		}
	}
	else if (phase == MARKET_PHASE) {
	}

	displayProvinceInfo(city);
}

void WindowManager::highlight(Province* city) {
	for (int i = 0; i < listOfArmyBage.size(); i++) {
		if (listOfArmyBage[i]->nameOfProvince == city->getName()) {
			listOfArmyBage[i]->setScale(2, 2);
			return;
		}
	}
}

void WindowManager::resetHighlights() {
	for (int i = 0; i < listOfArmyBage.size(); i++) {
		listOfArmyBage[i]->setScale(1, 1);
	}
}

void WindowManager::drawLine(Province* from, Province* to) {
	Vector2f firstCoordinates, secondCoordinates;
	for (int i = 0; i < listOfArmyBage.size(); i++) {
		if (listOfArmyBage[i]->nameOfProvince == from->getName())
			firstCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
		if (listOfArmyBage[i]->nameOfProvince == to->getName())
			secondCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
	}

	lineForProvinces->setCoordinates(firstCoordinates, secondCoordinates);
	lineForProvinces->setVisible(true);
}

void WindowManager::dragObject(sf::RenderWindow & window, sf::Event & event, int id) {
	string dummy;
	if (id == 0 && !(phase == PLACEMENT_PHASE || phase == INITIAL_PHASE || phase == MARKET_PHASE) /*&& this->GM->getPlayerByID(turn, dummy)->getLeftSoldier() == 0*/)
		return;
	if (id == 3 && !(phase == MARKET_PHASE))
		return;
	if (images[id]->inMove) {
		images[id]->setScale(sf::Vector2f(0.5, 0.5));
		images[id]->setPosition(sf::Vector2f(mouse.getPosition(window).x - 0.5*images[id]->getLocalBounds().width / 2, mouse.getPosition(window).y - 0.5*images[id]->getLocalBounds().height / 2));
	}
	if (mouse.isButtonPressed(sf::Mouse::Left) && event.key.code == mouse.Left && !images[id]->inMove) {
		if (sf::IntRect(images[id]->getPosition().x, images[id]->getPosition().y, images[id]->getLocalBounds().width, images[id]->getLocalBounds().height).contains(mouse.getPosition(window))) {
			images[id]->setPosition(sf::Vector2f(mouse.getPosition(window)));
			images[id]->inMove = true;
		}
	}
	else if ((event.type == event.MouseButtonReleased&& event.key.code == mouse.Left&& images[id]->inMove && !takeCastle) || (takeCastle && mouse.isButtonPressed(sf::Mouse::Right) && event.key.code == mouse.Right&& images[id]->inMove && id == 3)) {
		sf::Vector2i PixelPos = mouse.getPosition(window);
		sf::Vector2f MousePos = window.mapPixelToCoords(PixelPos, mainView);
		images[id]->inMove = false;
		takeCastle = false;
		images[id]->setPosition(images[id]->getInitialPosition());
		images[id]->setScale(sf::Vector2f(1, 1));
		if (sf::IntRect(0, 0, mapImg.getSize().x, mapImg.getSize().y).contains(sf::Vector2i(MousePos.x, MousePos.y)) && mouse.getPosition(window).y < 500)
		{
			if (id == 0) {
				string provinceName = getProvinceName(window, mouse);
				if (provinceName == "")
					return;
				cout << provinceName << endl;
				if (phase == INITIAL_PHASE) {
					cout << "*******" << buttons[NUMBER_TEXT]->getText() << endl;
					if (GM->placeSoldier(turn, provinceName, (buttons[NUMBER_TEXT]->getText()))) {
						if (page == 1) {
							if (turn = 0)
								this->GM->sendAllProvincesFromHostString(&NM);
							else {
								this->GM->sendAllProvincesClientToHostString(&NM);
								this->GM->sendAllProvincesFromHostString(&NM);
							}
							phase = END_TURN;
						}

						if (page == COMPUTER_GAME_SCREEN) {
							if (GM->getWorldMap()->ownerCount() != 42)
								phase = INITIAL_PHASE;
							else
								phase = PLACEMENT_PHASE;
						}
						turn++;
						if (turn == playerCount)
							turn = 0;


						cout << turn << "**********************";
						provinceNameTxt.setString("Player" + to_string(turn + 1) + "'s turn");

					}
				}
				else if (phase == PLACEMENT_PHASE || phase == MARKET_PHASE) {
					if (GM->placeSoldier(turn, provinceName, (buttons[NUMBER_TEXT]->getText()))) {
						if (wonSoldier) {
							wonSoldier = false;
						}
						int dummy; Province* province; string dum;
						GM->getWorldMap()->getProvinceByName(provinceName, dummy, province);
						provinceNameTxt.setString(provinceName + "\nSoldier number: " + to_string(province->getNumberOfSoldiers()));
						if (GM->getPlayerByID(turn, dum)->getLeftSoldier() == 0 && phase == PLACEMENT_PHASE) {
							phase = ATTACKING_PHASE;
							buttons[ATTACK_BUTTON]->setText("Attack");
							buttons[NEXT_PHASE_BUTTON]->setText("Fortify Phase");
							buttons[ATTACK_BUTTON]->setFlag(true);
							buttons[NEXT_PHASE_BUTTON]->setFlag(true);
						}
						else if (GM->getPlayerByID(turn, dum)->getLeftSoldier() != 0) {
							soldierAmount = GM->getPlayerByID(turn, dum)->getLeftSoldier();
							buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
						}
						else {
							soldierAmount = GM->getPlayerByID(turn, dum)->getMoney() / 50;
							buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
						}
					}
					else {
						provinceNameTxt.setString("It is not your city");
					}
				}
			}
			else if (id == 3 && phase == MARKET_PHASE) {
				string provinceName = getProvinceName(window, mouse);
				if (provinceName == "")
					return;
				int id; Province* ptr;
				GM->getWorldMap()->getProvinceByName(provinceName, id, ptr);
				if (GM->buildCastle(turn, provinceName)) {
					int index = castles.size();
					castles.push_back(new MyImage("castle.png"));
					castles[index]->setPosition(listOfArmyBage[id]->getPosition());
				}
			}
		}
	}
}

void WindowManager::drawAllCastles(RenderWindow & window, Event & e) {
	for (int i = 0; i < listOfCastleBage.size(); i++) {
		Province* ptr;
		WorldMap* wmPtr = GM->getWorldMap();
		int temp;
		int playerId;
		wmPtr->getProvinceByName(listOfCastleBage[i]->nameOfProvince, temp, ptr);
		if (ptr != NULL && (ptr->getCastle())->isBuilt() ) {
			if (ptr->getOwner() != NULL) {
				playerId = ptr->getOwner()->getId();
				if (playerId == 0)
					listOfCastleBage[i]->setBageColor(Color::Color(255, 0, 0, 255));
				if (playerId == 1)
					listOfCastleBage[i]->setBageColor(Color::Color(0, 0, 255, 255));
				if (playerId == 2)
					listOfCastleBage[i]->setBageColor(Color::Color(0, 255, 0, 255));
				(listOfCastleBage[i])->draw(window);
			}
			
		}
	}



}

void WindowManager::drawAllArmies(RenderWindow & window, Event & e) {
	for (int i = 0; i < listOfArmyBage.size(); i++) {
		Province* ptr;
		WorldMap* wmPtr = GM->getWorldMap();
		int temp;
		int playerId;
		wmPtr->getProvinceByName(listOfArmyBage[i]->nameOfProvince, temp, ptr);
		if (ptr != NULL) {
			int sizeOfArmy = ptr->getNumberOfSoldiers();
			if (sizeOfArmy < 100 && sizeOfArmy >= 0)
				listOfArmyBage[i]->setSizeOfArmy(sizeOfArmy);
			if (ptr->getOwner() != NULL) {
				playerId = ptr->getOwner()->getId();
				if (playerId == 0)
					listOfArmyBage[i]->setBageColor(Color::Color(255, 0, 0, 255));
				if (playerId == 1)
					listOfArmyBage[i]->setBageColor(Color::Color(0, 0, 255, 255));
				if (playerId == 2)
					listOfArmyBage[i]->setBageColor(Color::Color(0, 255, 0, 255));
				(listOfArmyBage[i])->draw(window);
			}

		}
	}



}

void WindowManager::displayProvinceInfo(Province* province) {
	if (province->getOwner() != NULL)

		infoText.setString("Name: " + province->getName() + "\nOwner: " + province->getOwner()->getName() + "\nSoldiers: " + to_string(province->getNumberOfSoldiers()) + "     Castle: " + (to_string(province->getCastle()->getLevel())));
	else
		infoText.setString("Name: " + province->getName() + "\nOwner: " + "None" + "\nSoldiers: " + to_string(province->getNumberOfSoldiers()) + "     Castle: " + (to_string(province->getCastle()->getLevel())));
}

Button::Button() {
	flag = false;
}

Button::Button(sf::Font & font) {
	RectangleShape::RectangleShape();
	text.setFont(font);
}

Button::~Button() {

}

void Button::setText(string text) {
	this->text.setString(text);
}

void Button::draw(sf::RenderWindow & window) {
	window.draw(*this);
	window.draw(text);
}

void Button::setPosition(float x, float y) {
	sf::RectangleShape::setPosition(x, y);
	text.setPosition(this->getPosition().x + this->getSize().x / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + (this->getSize().y / 2) - (text.getCharacterSize()/2 ));
	//text.setPosition(x + sf::RectangleShape().getPosition().x / 2, y + sf::RectangleShape().getPosition().y / 2);
}

int Button::getText() {
	string a = text.getString();
	return stoi(a);
}

bool Button::getFlag() {
	return flag;
}

void Button::setFlag(bool flag) {
	this->flag = flag;
}

void Button::setSize(int width, int height) {
	sf::RectangleShape::setSize(sf::Vector2f(width, height));
	//text.setPosition(this->getPosition().x + this->getSize().x / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + this->getSize().y / 2 - text.getGlobalBounds().height / 2);
	//text.setOrigin(width/2, height/2);
}

void Button::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void Button::setTextSize(int size) {
	text.setCharacterSize(size);
	text.setPosition(this->getPosition().x + this->getSize().x / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + (this->getSize().y / 2) - (text.getCharacterSize()/2));
}

MyImage::MyImage() {

}

MyImage::~MyImage() {
	inMove = false;
}

MyImage::MyImage(string fileName) {
	img.loadFromFile("assets/" + fileName);
	tex.loadFromImage(img);
	this->setTexture(tex);
	inMove = false;
}

sf::Vector2f MyImage::getSize() {
	return sf::Vector2f(this->getTextureRect().width, this->getTextureRect().height);
}

void MyImage::setInitialPosition(float x, float y) {
	initialPosition = sf::Vector2f(x, y);
}

sf::Vector2f MyImage::getInitialPosition() {
	return initialPosition;
}

NetworkManager::NetworkManager(WindowManager * WM) {
	this->WM = WM;
}

void NetworkManager::createNetwork(GameManager ** const GM, string _connectionType, string _name) {
	ip = IpAddress::getLocalAddress();
	string text = " ";
	int playerCount = 0;
	Packet packet;

	cout << "(h) for server, (c) for client: ";
	connectionType = _connectionType;

	unsigned short port = 2000;

	if (connectionType == "h") {
		port = 2000;
	}
	else if (connectionType == "c1")
		port = 2001;
	else if (connectionType == "c2")
		port = 2002;
	//else if (connectionType == "c3")
	//	port = 2003;
	//else if (connectionType == "c4")
	//	port = 2004;
	//else if (connectionType == "c5")
	//	port = 2005;


	if (socket.bind(port) != Socket::Done) {
		cout << "Couldnt binded. ";
	}
	else {
		cout << "BINDED !! " << endl;
	}

	if (connectionType == "h") {
		string name;
		playersName = "";
		cout << "Enter your name(HOST): ";
		name = _name;
		playersName += name + ",";
		(*GM)->addPlayer(name);
		do {
			IpAddress rIP;
			unsigned short port;
			Packet packet;

			if (socket.receive(packet, rIP, port) == Socket::Done) {
				computerID[port] = rIP;
				playerCount++;

				String name;
				packet >> name;
				string display = name;
				if (port == 2001) {
					cout << display << " has joined the room." << endl;
					(*GM)->addPlayer(display);
					playersName += name + ",";
				}
				else if (port == 2002) {
					cout << display << " has joined the room." << endl;
					(*GM)->addPlayer(display);
					playersName += name + ",";

					//if (port == 2003)
					//	cout << "Client3 has joined the room." << endl;
					//if (port == 2004)
					//	cout << "Client4 has joined the room." << endl;
					//if (port == 2005)
					//	cout << "Client5 has joined the room." << endl;
				}
			}
			cout << "Player in the game: " << playerCount << endl;
		} while (playerCount != 3);

	}

	else if (connectionType == "c1" || connectionType == "c2") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		string name;
		cout << "Enter your name: ";
		name = _name;
		String playerName = name;
		packet << playerName;
		sIP = "139.179.202.124";
		IpAddress sendIP(sIP);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "You have joined the room." << endl;
		//cout << sIP << endl;

		IpAddress tempId;
		unsigned short tempPort;
		Packet packet;

		if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
			String received;
			packet >> received;
			string str = received;
			cout << "The Game is starting" << endl;
			players = split(str, ',');

			for (int i = 0; i < players.size(); i++) {
				(*GM)->addPlayer(players[i]);
			}
			if (WM != NULL) {
				WM->page = WM->GAME_SCREEN;
				WM->userTurn = WM->GM->getPlayerTurn(WM->userName);
				cout << "MY TURN " << WM->userTurn << endl;
				if (WM->_randomPlacement)
					WM->GM->sendAllProvincesFromHostString(&(WM->NM));
				WM->phase = WM->END_TURN;
				cout << "MY PHAAASSEEE " << WM->phase << endl;
				//GM->sendAllProvincesFromHost(this);
			}
		}
	}
}

void NetworkManager::startGame() {

	if (connectionType == "h") {
		String sendPlayersName;
		sendPlayersName = "" + playersName;
		Packet packet;
		packet << sendPlayersName;
		string display = sendPlayersName;
		cout << display << endl;
		map<unsigned short, IpAddress> ::iterator tempIterator;
		for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
			if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {
			}
	}
	else {
		cout << "You cannot start the game " << endl;
		return;
	}
}

string NetworkManager::sendStringFromHost(string _sendText) {
	while (true) {
		if (connectionType == "h") {
			String sentText = _sendText;
			cout << _sendText << "send string :::::::::" << endl;
			Packet packet;
			packet << sentText;
			map<unsigned short, IpAddress> ::iterator tempIterator;
			for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
				if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {
					packet >> sentText;
					string  display = sentText;
					cout << display << "send string :::::::::" << endl;
				} // the socket send or not 
			return "";
		}

		else if (connectionType == "c1" || connectionType == "c2") {

			IpAddress tempId;
			unsigned short tempPort;
			Packet packet;

			if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
				String receivedString;
				packet >> receivedString;
				string stringRec = receivedString;
				cout << "The received string : ************ " << stringRec << endl;
				return stringRec;
			}

		}
	}
}

void NetworkManager::sendDataFromHost(GameManager * const GM, int _playerID, int _cityID, int _count, int _castleLevel) {

	while (true) {
		if (connectionType == "h") {
			Uint16 playerID = _playerID;
			Uint16 cityID = _cityID;
			Uint16 count = _count;
			Uint16 castleLevel = _castleLevel;
			Packet packet;
			packet << playerID << cityID << count << castleLevel;
			map<unsigned short, IpAddress> ::iterator tempIterator;
			for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
				if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {} // the socket send or not 
			break;
		}

		else if (connectionType == "c1" || connectionType == "c2") {

			IpAddress tempId;
			unsigned short tempPort;
			if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
				Uint16 receivedPlayerID;
				Uint16 receivedCityID;
				Uint16 receivedCount;

				Uint16 receivedcastleLevel;
				packet >> receivedPlayerID >> receivedCityID >> receivedCount >> receivedcastleLevel;

				string dummy;
				int pId = receivedPlayerID;
				int cId = receivedCityID;
				int count = receivedCount;
				int casLev = receivedcastleLevel;
				Player * playerChanged = GM->getPlayerByID(pId, dummy);
				Province * provinceChanged = GM->getWorldMap()->getProvinceByID(cId);
				//provinceChanged->setOwner(playerChanged);
				playerChanged->captureProvince(GM->getWorldMap(), provinceChanged);
				provinceChanged->setNumberOfSoldiers(count);
				provinceChanged->getCastle()->setLevel(casLev);
				break;
			}

		}
	}
}

void NetworkManager::sendDataFromClientToHost(GameManager * const GM, string _connectionType, int _playerID, int _cityID, int _count, int _castleLevel) {
	int pID, cID, cou, cast;
	while (true) {
		if (_connectionType != "h") {
			Uint16 playerID = _playerID;
			Uint16 cityID = _cityID;
			Uint16 count = _count;
			Uint16 castleLevel = _castleLevel;
			Packet packet;
			packet << playerID << cityID << count << castleLevel;
			//cout << "!!!!!!!!!!!!!!!!!!" << endl;
			IpAddress sendIP(sIP);
			if (socket.send(packet, sendIP, 2000) == Socket::Done)
				break;
		}
		else if (_connectionType == "h") {
			IpAddress tempId;
			unsigned short tempPort;
			Packet packet;
			if (socket.receive(packet, tempId, tempPort) == Socket::Done) {
				Uint16 receivedPlayerID;
				Uint16 receivedCityID;
				Uint16 receivedCount;
				Uint16 receivedcastleLevel;
				packet >> receivedPlayerID >> receivedCityID >> receivedCount >> receivedcastleLevel;
				//cout << "Received Data from client: " << receivedPlayerID << receivedCityID << receivedCount << endl;
				pID = receivedPlayerID;
				cID = receivedCityID;
				cou = receivedCount;
				cast = receivedcastleLevel;

				string dummy;
				Player * playerChanged = GM->getPlayerByID(pID, dummy);
				Province * provinceChanged = GM->getWorldMap()->getProvinceByID(cID);
				//provinceChanged->setOwner(playerChanged);
				playerChanged->captureProvince(GM->getWorldMap(), provinceChanged);
				provinceChanged->setNumberOfSoldiers(receivedCount);
				provinceChanged->getCastle()->setLevel(cast);

				break;
			}
		}
		else
			break;
	}

}

void NetworkManager::buildNewtwork() {
	IpAddress ip = IpAddress::getLocalAddress();
	UdpSocket socket;
	string connectionType;
	string sIp;
	int playerCount = 0;
	map<unsigned short, IpAddress> computerID;
	string text = " ";
	Packet packet;

	cout << ip << endl;

	cout << "(h) for server, (c) for client: ";
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

	// TO connect all players !! s is the host which will be the server in our game
	if (connectionType == "h") {
		string startGame = "no";
		do {
			IpAddress rIP;
			unsigned short port;
			if (socket.receive(packet, rIP, port) == Socket::Done) {
				computerID[port] = rIP;
			}
			playerCount++;
			cout << "Player in the game (except host): " << playerCount << endl;
		} while (playerCount != 2);

		do {
			cout << "Enter 'start' to create the game: ";
			cin >> startGame;
		} while (startGame != "start");

	}

	// clients are the other player who joined the room
	else if (connectionType == "c1") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		sIp = "139.179.202.124";
		IpAddress sendIP(sIp);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "Client1 has joined the room." << endl;
		cout << sIp << endl;
	}

	else if (connectionType == "c2") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		sIp = "139.179.202.124";
		IpAddress sendIP(sIp);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "Client2 has joined the room." << endl;
		cout << sIp << endl;
	}


	bool done = false;
	while (!done) {
		if (connectionType == "h") {
			getline(cin, text);
			Packet packet;
			packet << text;
			map<unsigned short, IpAddress> ::iterator tempIterator;
			for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
				if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {} // the socket send or not 
		}

		else if (connectionType == "c1" || connectionType == "c2") {

			IpAddress tempId;
			unsigned short tempPort;
			if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
				string receivedText;
				packet >> receivedText;
				cout << "The Game Started" << receivedText << endl;
			}


		}
	}
}

vector<string> NetworkManager::split(string strToSplit, char delimeter) {
	stringstream ss(strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

MiniMap::MiniMap(sf::Texture  mapTexture) {
	sf::View::View();
	this->setSize(GetSystemMetrics(SM_CXSCREEN) / 5, GetSystemMetrics(SM_CYSCREEN) / 5);
	this->zoom(10);
	mapTex = mapTexture;
	mapSprite.setTexture(mapTex);
	mapSprite.setTextureRect(IntRect(0, 0, mapTex.getSize().x, mapTex.getSize().y));
}

MiniMap::MiniMap() {
	sf::View::View();
}

void MiniMap::update(sf::View & mainView) {
	this->setCenter(mapTex.getSize().x / 2, mapTex.getSize().y / 2);
	mapSprite.setTexture(mapTex);
	mapSprite.setTextureRect(IntRect(0, 0, mapTex.getSize().x, mapTex.getSize().y));
	miniMapRectangle.setFillColor(Color::Transparent);
	miniMapRectangle.setOutlineThickness(20);
	miniMapRectangle.setOutlineColor(Color::Black);
	miniMapRectangle.setSize(mainView.getSize());
	miniMapRectangle.setPosition(mainView.getCenter().x - mainView.getSize().x / 2, mainView.getCenter().y - mainView.getSize().y / 2);
	this->setViewport(FloatRect(float(0.8), float(0.8), 0.2, 0.2));

}

void MiniMap::draw(sf::RenderWindow & window) {
	window.draw(mapSprite);
	window.draw(miniMapRectangle);


}

ArmyBage::ArmyBage(Image img, int x, int y, string nameOfProvince, Font &font) {
	this->nameOfProvince = nameOfProvince;
	this->text.setFont(font);
	this->img = img;
	tex.loadFromImage(img);
	this->setTexture(tex);
	this->setScale(1, 1);
	sizeOfArmy = 10;
	string temp = to_string(sizeOfArmy);
	text.setCharacterSize(20);
	text.setString(String(temp));
	centerCoordinates = Vector2f(x, y);

	this->setPosition(x - this->getGlobalBounds().width / 2, y - this->getGlobalBounds().height / 2);
	text.setPosition(this->getPosition().x + this->getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + this->getGlobalBounds().height / 2 - text.getGlobalBounds().height);
}

ArmyBage::ArmyBage() {

}

void ArmyBage::setSizeOfArmy(int size) {
	sizeOfArmy = size;
	string temp = to_string(sizeOfArmy);
	text.setString(String(temp));
	text.setPosition(this->getPosition().x + this->getGlobalBounds().width / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + this->getGlobalBounds().height / 2 - text.getGlobalBounds().height);
}

void ArmyBage::setBageColor(Color color) {
	this->setColor(color);
	this->color = color;
}

void ArmyBage::draw(sf::RenderWindow & window) {
	this->setTexture(tex);
	this->text.setString(String(to_string(sizeOfArmy)));
	text.setCharacterSize(20);
	this->setColor(this->color);

	text.setFillColor(Color(255, 255, 255, 255));
	window.draw(*this);
	window.draw(this->text);
}



CastleBage::CastleBage() {

}


CastleBage::CastleBage(Image img, int x, int y, string nameOfProvince) {
	this->nameOfProvince = nameOfProvince;
	this->img = img;
	tex.loadFromImage(img);
	this->setTexture(tex);
	this->setScale(1, 1);
	centerCoordinates = Vector2f(x, y);

	this->setPosition(x - this->getGlobalBounds().width / 2, y - this->getGlobalBounds().height / 2);
}


void CastleBage::draw(sf::RenderWindow & window) {
		this->setColor(this->color);
		window.draw(*this);
}
void CastleBage::setBageColor(sf::Color color) {
	this->setColor(color);
	this->color = color;
}

LineBetweenProvinces::LineBetweenProvinces(Image &img) {
	this->img = img;
	tex.setRepeated(true);
	this->setTexture(tex);
	step = 0;
	degree = 0;
	visible = false;

	this->setTextureRect(IntRect(step, 0, img.getSize().x, 100));
	this->setPosition(Vector2f(0, 0));
	first = Vector2f(0, 0);
	second = Vector2f(0, 0);
	this->setScale(1, 0.25);
	this->setOrigin(0, 12.5);

}

LineBetweenProvinces::LineBetweenProvinces() {

}

void LineBetweenProvinces::setCoordinates(Vector2f first,Vector2f second) {
	this->first = first;
	this->second = second;

	lenght = (int) sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2) );
	degree = (float) atan2(second.y - first.y, second.x - first.x) * 180 / 3.14159;
	
	this->setOrigin(0, img.getSize().y/2);

	this->setTextureRect(IntRect(step, 0, img.getSize().x, lenght));
	this->setPosition(first);
	
	this->setRotation(degree);
}

void LineBetweenProvinces::draw(RenderWindow &window,float time) {
	if (visible) {
		step= step-0.1*time;
		tex.loadFromImage(img);
		tex.setRepeated(true);
		this->setTexture(tex);

		this->setTextureRect(IntRect((int)step, 0, max(0,lenght), img.getSize().y));
		this->setOrigin(0, img.getSize().y / 2);
		window.draw(*this);
	}
}

void LineBetweenProvinces::setVisible(bool flag) {
	this->visible = flag;
}


TopBar::TopBar(Image img, Image coinImg, Image actionImage,  Image turnImage, Image playerImage, Font &font) {
	this->img = img;
	this->coinImg = coinImg;
	this->actionImg = actionImage;
	this->turnImg = turnImage;
	this->playerImg = playerImage;

	int lenght = screenWidth;
	float ratio = float(lenght) / img.getSize().x;

	panelTexture.loadFromImage(this->img);
	coinTexture.loadFromImage(this->coinImg);
	actionTexture.loadFromImage(this->actionImg);
	turnTexture.loadFromImage(this->turnImg);
	playerTexture.loadFromImage(this->playerImg);

	this->setTexture(panelTexture);
	
	coinSprite.setTexture(coinTexture);
	coinSprite.setScale(ratio,ratio);
	
	actionSprite.setTexture(actionTexture);
	actionSprite.setScale(ratio, ratio);

	turnSprite.setTexture(turnTexture);
	turnSprite.setScale(ratio, ratio);

	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(ratio, ratio);

	//---------------
	coinText.setFont(font);
	coinText.setCharacterSize(20);
	coinText.setString("120");

	actionText.setFont(font);
	actionText.setCharacterSize(20);
	actionText.setString("7");

	turnText.setFont(font);
	turnText.setCharacterSize(20);
	turnText.setString("1");

	playerText.setFont(font);
	playerText.setCharacterSize(20);
	playerText.setString("Elnur");
	//-------------------------
	coinField.setSize(Vector2f(100 * ratio,55*ratio));
	coinField.setFillColor(Color(0, 0, 0, 30));
	
	actionField.setSize(Vector2f(100 * ratio, 55 * ratio));
	actionField.setFillColor(Color(0, 0, 0, 30));

	turnField.setSize(Vector2f(100*ratio, 55 * ratio));
	turnField.setFillColor(Color(0, 0, 0, 30));
	
	playerField.setSize(Vector2f(180*ratio, 55 * ratio));
	playerField.setFillColor(Color(0, 0, 0, 30));

	//this->setColor(Color(255, 255, 255, 100));
	this->setPosition(0, 0);
	this->setTextureRect(IntRect(0,0,panelTexture.getSize().x, 100));
	this->setScale(float (lenght) /panelTexture.getSize().x, 0.65*ratio);



	coinField.setPosition(195*ratio, 2 * ratio);
	coinSprite.setPosition(coinField.getPosition().x + 5, 5*ratio);
	coinText.setPosition(coinSprite.getPosition().x + coinSprite.getGlobalBounds().width + 10*ratio, coinSprite.getPosition().y+ coinSprite.getGlobalBounds().height/2- coinText.getGlobalBounds().height / 2);

	actionField.setPosition(coinField.getPosition().x + coinField.getGlobalBounds().width + 35, 2 * ratio);
	actionSprite.setPosition(actionField.getPosition().x +5, 7 * ratio);
	actionText.setPosition(actionSprite.getPosition().x + actionSprite.getGlobalBounds().width + 10 * ratio, actionSprite.getPosition().y + actionSprite.getGlobalBounds().height / 2- actionText.getGlobalBounds().height/2);

	turnField.setPosition(actionField.getPosition().x + actionField.getGlobalBounds().width + 35, 2 * ratio);
	turnSprite.setPosition(turnField.getPosition().x +5, 7 * ratio);
	turnText.setPosition(turnSprite.getPosition().x + turnSprite.getGlobalBounds().width + 10 * ratio, turnSprite.getPosition().y + turnSprite.getGlobalBounds().height / 2 - turnText.getGlobalBounds().height / 2);


	playerField.setPosition(turnField.getPosition().x + turnField.getGlobalBounds().width + 35, 2 * ratio);
	playerSprite.setPosition(playerField.getPosition().x + 5, 7 * ratio);
	playerText.setPosition(playerSprite.getPosition().x + playerSprite.getGlobalBounds().width + 10 * ratio, playerSprite.getPosition().y + playerSprite.getGlobalBounds().height / 2 - playerText.getGlobalBounds().height / 2);





}

TopBar::TopBar() {

}


void TopBar::draw(RenderWindow & window) {
	window.draw(*this);

	window.draw(coinField);
	window.draw(coinSprite);
	window.draw(coinText);

	window.draw(actionField);
	window.draw(actionSprite);
	window.draw(actionText);

	window.draw(turnField);
	window.draw(turnSprite);
	window.draw(turnText);

	window.draw(playerField);
	window.draw(playerSprite);
	window.draw(playerText);

}

void TopBar::update(int moneyAmount, int actionAmount, int currentTurn, string player) {
	coinText.setString(to_string(moneyAmount));
	actionText.setString(to_string(actionAmount));
	turnText.setString(to_string(currentTurn));
	playerText.setString(player);

}

PhaseNotifier::PhaseNotifier() {

}

PhaseNotifier::PhaseNotifier(int phaseId,Image img, int x, int y, int fx,int fy) {
	this->phaseId = phaseId;
	this->img = img;
	this->tex.loadFromImage(img);
	this->setTexture(this->tex);
	visible = false;
	moveTimer = 0;
	this->setPosition( x - img.getSize().x/2,  y- img.getSize().y / 2);

	this->bigCoordinates = Vector2f(x - img.getSize().x / 2, y - img.getSize().y / 2);
	this->smallCoordinates = Vector2f(fx, fy);
	this->ck.restart();
	this->deactivate();
}


void PhaseNotifier::draw(sf::RenderWindow & window,float time) {
	if (!visible)
		return;
	window.draw(*this);
	moveTimer = this->ck.getElapsedTime().asSeconds()+moveTimer;
	this->ck.restart();

	if (moveTimer > 0.7) {



		if (this->getPosition().x < smallCoordinates.x)
			this->setPosition(this->getPosition().x + 2*time, this->getPosition().y);
		
		if (this->bigCoordinates.y < this->smallCoordinates.y) {
			if (this->getPosition().y < smallCoordinates.y)
				this->setPosition(this->getPosition().x, this->getPosition().y + 2 * time);
		}
		else {
			if (this->getPosition().y > smallCoordinates.y)
				this->setPosition(this->getPosition().x, this->getPosition().y - 2 * time);
		}

		if (this->getScale().x > 0.4)
			this->setScale(this->getScale().x - 0.01*time, this->getScale().x - 0.01*time);

	}


}

void PhaseNotifier::activate() {
	if (visible)
		return;
	visible = true;
	moveTimer = 0;
	this->setScale(1, 1);
	this->setPosition(this->bigCoordinates);
	ck.restart();
}
void PhaseNotifier::deactivate() {
	if (!visible)
		return;
	moveTimer = 0;
	visible = false;

}

BattleNotifier::BattleNotifier() {

}

BattleNotifier::BattleNotifier(Image img1, Image img2, Image img3, int x, int y,Font font) {
	imageOfBattle = img1;
	imageOfDeath = img2;
	warriorsImg = img3;
	this->font = font;
	visible = false;


	texureOfBattle.loadFromImage(imageOfBattle);
	textureOfDeath.loadFromImage(imageOfDeath);
	textureOfWarriors.loadFromImage(warriorsImg);

	this->setTexture(texureOfBattle);
	spriteOfDeath.setTexture(textureOfDeath);
	spriteOfWarriors.setTexture(textureOfWarriors);

	this->setPositionCenter(x, y);

	battleText.setFont(this->font);
	battleText.setCharacterSize(40);
	battleText.setString("Battle");
	battleText.setFillColor(Color(0, 0, 0));

	playerName1.setFont(this->font);
	playerName1.setCharacterSize(40);
	playerName1.setStyle(1<<2);
	playerName1.setFillColor(Color(0, 0, 0));

	playerName2.setFont(this->font);
	playerName2.setCharacterSize(40);
	playerName2.setStyle(1 << 2);
	playerName2.setFillColor(Color(0, 0, 0));

	soldiers1.setFont(this->font);
	soldiers1.setCharacterSize(40);
	soldiers1.setFillColor(Color(0, 0, 0));

	soldiers2.setFont(this->font);
	soldiers2.setCharacterSize(40);
	soldiers2.setFillColor(Color(0, 0, 0));


	soldierLost1.setFont(this->font);
	soldierLost1.setCharacterSize(40);
	soldierLost1.setFillColor(Color(0, 0, 0));

	soldierLost2.setFont(this->font);
	soldierLost2.setCharacterSize(40);
	soldierLost2.setFillColor(Color(0, 0, 0));

}

void BattleNotifier::setPositionCenter(int x, int y) {
	this->setPosition(x - this->getGlobalBounds().width / 2, y - this->getGlobalBounds().height / 2);
	centerCoordinates = Vector2f(x,y);
}

void BattleNotifier::updateData(string battleString, string playerNameString1, string playerNameString2, string soldiersString1, string soldiersString2, string soldierLostString1, string soldierLostString2) {
	this->battleString = battleString;
	this->playerName1 = playerName1;
	this->playerName2 = playerName2;
	this->soldiersString1 = soldiersString1;
	this->soldiersString2 = soldiersString2;
	this->soldierLostString1 = soldierLostString1;
	this->soldierLostString2 = soldierLostString2;

	this->battleText.setString(battleString);
	playerName1.setString(playerNameString1);
	playerName2.setString(playerNameString2);
	soldiers1.setString(soldiersString1);
	soldiers2.setString(soldiersString2);
	soldierLost1.setString(soldierLostString1);
	soldierLost2.setString(soldierLostString2);

	battleText.setPosition(Vector2f(this->centerCoordinates.x - battleText.getGlobalBounds().width / 2, this->centerCoordinates.y - battleText.getGlobalBounds().height / 2 - this->getGlobalBounds().height) );
	playerName1.setPosition(this->centerCoordinates.x - this->getGlobalBounds().width - playerName1.getGlobalBounds().width, this->centerCoordinates.y - this->getGlobalBounds().height / 2);
	playerName2.setPosition(this->centerCoordinates.x + this->getGlobalBounds().width, this->centerCoordinates.y - this->getGlobalBounds().height / 2);


	spriteOfWarriors.setPosition(this->centerCoordinates.x - spriteOfWarriors.getGlobalBounds().width / 2, this->centerCoordinates.y + spriteOfWarriors.getGlobalBounds().height / 2);
	soldiers1.setPosition(this->centerCoordinates.x - 10 - this->getGlobalBounds().width / 2 - soldiers1.getGlobalBounds().width, this->centerCoordinates.y + spriteOfWarriors.getGlobalBounds().height / 2);
	soldiers2.setPosition(this->centerCoordinates.x + this->getGlobalBounds().width / 2 + soldiers2.getGlobalBounds().width, this->centerCoordinates.y + spriteOfWarriors.getGlobalBounds().height / 2);


	spriteOfDeath.setPosition(this->centerCoordinates.x - spriteOfDeath.getGlobalBounds().width / 2, spriteOfWarriors.getPosition().y + spriteOfWarriors.getGlobalBounds().height + 10);
	soldierLost1.setPosition(this->centerCoordinates.x - 10 - this->getGlobalBounds().width / 2 - soldierLost1.getGlobalBounds().width, spriteOfWarriors.getPosition().y + spriteOfWarriors.getGlobalBounds().height + 10);
	soldierLost2.setPosition(this->centerCoordinates.x + this->getGlobalBounds().width / 2 + soldierLost2.getGlobalBounds().width, spriteOfWarriors.getPosition().y + spriteOfWarriors.getGlobalBounds().height + 10);

}
void BattleNotifier::draw(sf::RenderWindow & window, float time) {
	
	if (!visible)
		return;
	if (opacityParam < 100)
		this->deactivate();
	cout << removeTimer << endl;
	removeTimer = this->ckk.getElapsedTime().asSeconds() + removeTimer;
	this->ckk.restart();
	
	if (removeTimer > 2) {
		opacityParam = opacityParam - 0.3*time ;
		this->setColor(Color(this->getColor().r, this->getColor().g, this->getColor().b, opacityParam));

		battleText.setFillColor(Color(battleText.getFillColor().r, battleText.getFillColor().g, battleText.getFillColor().b, (int)opacityParam));
		playerName1.setFillColor(Color(playerName1.getFillColor().r, playerName1.getFillColor().g, playerName1.getFillColor().b, (int)opacityParam));
		playerName2.setFillColor(Color(playerName2.getFillColor().r, playerName2.getFillColor().g, playerName2.getFillColor().b, (int)opacityParam));
		soldiers1.setFillColor(Color(soldiers1.getFillColor().r, soldiers1.getFillColor().g, soldiers1.getFillColor().b, (int)opacityParam));
		soldiers2.setFillColor(Color(soldiers2.getFillColor().r, soldiers2.getFillColor().g, soldiers2.getFillColor().b, (int)opacityParam));
		spriteOfDeath.setColor(Color(spriteOfDeath.getColor().r, spriteOfDeath.getColor().g, spriteOfDeath.getColor().b, (int)opacityParam));

		soldierLost1.setFillColor(Color(soldierLost1.getFillColor().r, soldierLost1.getFillColor().g, soldierLost1.getFillColor().b, (int)opacityParam));
		soldierLost2.setFillColor(Color(soldierLost2.getFillColor().r, soldierLost2.getFillColor().g, soldierLost2.getFillColor().b, (int)opacityParam));
	}

	window.draw(*this);
	window.draw(battleText);

	window.draw(playerName1);
	window.draw(playerName2);
	window.draw(spriteOfWarriors);
	window.draw(soldiers1);
	window.draw(soldiers2);

	window.draw(spriteOfDeath);

	window.draw(soldierLost1);
	window.draw(soldierLost2);


}




void BattleNotifier::activate() {
	visible = true;
	removeTimer = 0;
	opacityParam = 255.f;
	this->setColor(Color(this->getColor().r, this->getColor().g, this->getColor().b, opacityParam));

	battleText.setFillColor(Color(battleText.getFillColor().r, battleText.getFillColor().g, battleText.getFillColor().b, (int)opacityParam));
	playerName1.setFillColor(Color(playerName1.getFillColor().r, playerName1.getFillColor().g, playerName1.getFillColor().b, (int)opacityParam));
	playerName2.setFillColor(Color(playerName2.getFillColor().r, playerName2.getFillColor().g, playerName2.getFillColor().b, (int)opacityParam));
	soldiers1.setFillColor(Color(soldiers1.getFillColor().r, soldiers1.getFillColor().g, soldiers1.getFillColor().b, (int)opacityParam));
	soldiers2.setFillColor(Color(soldiers2.getFillColor().r, soldiers2.getFillColor().g, soldiers2.getFillColor().b, (int)opacityParam));
	spriteOfDeath.setColor(Color(spriteOfDeath.getColor().r, spriteOfDeath.getColor().g, spriteOfDeath.getColor().b, (int)opacityParam));

	soldierLost1.setFillColor(Color(soldierLost1.getFillColor().r, soldierLost1.getFillColor().g, soldierLost1.getFillColor().b, (int)opacityParam));
	soldierLost2.setFillColor(Color(soldierLost2.getFillColor().r, soldierLost2.getFillColor().g, soldierLost2.getFillColor().b, (int)opacityParam));
	this->ckk.restart();

}
void BattleNotifier::deactivate() {
	visible = false;
	removeTimer = 0;
	opacityParam = 0.f;

}