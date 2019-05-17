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
	leftSoldier = 14;
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


int Player::howManyCont(int &res) {

	int count = 0;
	int earnedSold = 0;
	res = 0;

	sort(provinces.begin(), provinces.end());


	for (int i = 0; i < provinces.size() - 1; i++) {
		if (provinces.at(i) + 1 == provinces.at(i + 1)) {
			count++;
		}
		else
			count = 0;

		if (provinces.at(i) == 8 || provinces.at(i) == 12 || provinces.at(i) == 19 || provinces.at(i) == 25 || provinces.at(i) == 37 || provinces.at(i) == 41)
			count = 0;

		if (provinces.at(i) == 7 && count == 8) {
			res++;
			earnedSold += 5;
		}

		else if (provinces.at(i) == 11 && count == 3) {
			res++;
			earnedSold += 2;
		}

		else if (provinces.at(i) == 18 && count == 6) {
			res++;
			earnedSold += 5;
		}

		else if (provinces.at(i) == 24 && count == 5) {
			res++;
			earnedSold += 3;
		}

		else if (provinces.at(i) == 36 && count == 11) {
			res++;
			earnedSold += 7;
		}

		else if (provinces.at(i) == 40 && count == 3) {
			count = 0;
			res++;
			earnedSold += 2;
		}

	}
	return earnedSold;

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

vector<Province*> WorldMap::getPossiblePaths(Province* city) {
	vector<Province*> result;
	Player* player = city->getOwner();
	bool mark[100];
	for (int i = 0; i < 100; i++) {
		mark[i] = false;
	}
	int first = findIndex(city);
	queue<int> q;
	q.push(first);
	mark[first] = true;

	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		for (auto it = map[cur].begin(); it != map[cur].end(); it++) {
			if (mark[*it] || provinceList[*it]->getOwner() != player)
				continue;
			q.push(*it);
			mark[*it] = true;
			result.push_back(provinceList[*it]);
		}

	}

	return result;
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

vector<Province* > WorldMap::getShortestPath(Province* from, Province * to) {
	vector<Province*> result;
	Player* player = from->getOwner();
	int parent[100];
	bool mark[100];
	for (int i = 0; i < 100; i++) {
		mark[i] = false;
		parent[i] = -1;
	}
	int first = findIndex(from);
	queue<int> q;
	q.push(first);
	mark[first] = true;

	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		if (cur == findIndex(to)) {
			break;
		}

		for (auto it = map[cur].begin(); it != map[cur].end(); it++) {
			if (mark[*it] || provinceList[*it]->getOwner() != player)
				continue;
			q.push(*it);
			parent[*it] = cur;
			mark[*it] = true;
			//result.push_back(provinceList[*it]);
		}
	}
	int cur = findIndex(to);
	while (parent[cur] != -1) {
		result.push_back(provinceList[cur]);
		cur = parent[cur];
	}
	result.push_back(from);

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
	return NULL;
}

bool GameManager::buildCastle(Player* player, Province* province) {
	if (province->getOwner() != player) {
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
		return false;
	}
	if (!worldMap->hasPath(player, from, to)) {
		return false;
	}

	from->setNumberOfSoldiers(from->getNumberOfSoldiers() - amount);
	to->setNumberOfSoldiers(to->getNumberOfSoldiers() + amount);
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
		return false;
	}
	if (from->getOwner() != attacker) {
		return false;
	}
	if (to->getOwner() != defender) {
		return false;
	}
	if (!worldMap->isNeighbor(from, to)) {
		return false;
	}
	if (amount >= from->getNumberOfSoldiers()) {
		return false;
	}
	vector<int> result;
	if (to->getNumberOfSoldiers() == 1) {
		result = rollDice(amount, 1);
	}
	else {
		result = rollDice(amount, 2);
	}
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
		return false;
	}
	Province* from, *to;
	worldMap->getProvinceByName(fromStr, i, from);
	if (i == -1) {
		return false;
	}
	worldMap->getProvinceByName(toStr, i, to);
	if (i == -1) {
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

}

void GameManager::startGame(NetworkManager ** NM) {

	//loadProvinces();
	//if ((*NM)->connectionType == "h") {
	//	//startPlacement();
	//	randomPlacement();
	//	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
	//		Province * pro = worldMap->getProvinceByID(i);
	//		(*NM)->sendDataFromHost(this, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(), pro->getCastle()->getLevel());
	//		//NM->sendDataFromHost(this, 1, i, 4);
	//	}
	//}
	//else {
	//	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
	//		//NM->sendDataFromHost(this, worldMap->getProvinceByID(i)->getOwner()->getId(), i, worldMap->getProvinceByID(i)->getNumberOfSoldiers());
	//		(*NM)->sendDataFromHost(this, 0, 0, 0, 0);
	//	}
	//}

	//showWorldStatus();

	//gameOn = true;

	//int turn = 0;
	//int numberOfPlayers = players.size();
	//int numberOfProvinces = worldMap->getNumberOfProvinces();
	//cout << endl << "==================================  " << endl;
	//while (gameOn) {

	//	if ((*NM)->connectionType == "h" && turn == 0) {
	//		cout << endl << "============		YOUR TURN		===============  " << endl;
	//		currentPlayer = 0;
	//		startTurn(0);
	//		sendAllProvincesFromHost(NM);
	//		turn = 1;
	//	}
	//	else if ((*NM)->connectionType == "h" && turn == 1) {
	//		cout << endl << "============		" << players[1]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesClientToHost((*NM)->connectionType, NM);
	//		sendAllProvincesFromHost(NM);
	//		cout << endl << "============		" << players[2]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesClientToHost((*NM)->connectionType, NM);
	//		sendAllProvincesFromHost(NM);
	//		turn = 0;
	//	}


	//	else if ((*NM)->connectionType == "c1" && turn == 0) {
	//		cout << endl << "============		" << players[0]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesFromHost(NM);
	//		cout << endl << "============		YOUR TURN		===============  " << endl;
	//		currentPlayer = 1;
	//		startTurn(1);
	//		sendAllProvincesClientToHost((*NM)->connectionType, NM);
	//		sendAllProvincesFromHost(NM);
	//		turn = 1;
	//	}
	//	else if ((*NM)->connectionType == "c1" && turn == 1) {
	//		cout << endl << "============		" << players[2]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesFromHost(NM);
	//		turn = 0;
	//	}



	//	else if ((*NM)->connectionType == "c2" && turn == 0) {
	//		cout << endl << "============		" << players[0]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesFromHost(NM);
	//		cout << endl << "============		" << players[1]->getName() << " TURN		===============  " << endl;
	//		sendAllProvincesFromHost(NM);
	//		cout << endl << "============		YOUR TURN		===============  " << endl;
	//		currentPlayer = 2;
	//		startTurn(2);
	//		sendAllProvincesClientToHost((*NM)->connectionType, NM);
	//		sendAllProvincesFromHost(NM);
	//	}


	//	/*else if ((*NM)->connectionType == "c2" && turn == 1) {
	//	cout << endl << "============		" << players[3]->getName() << " TURN		===============  " << endl;
	//	sendAllProvincesFromHost(NM);
	//	}*/
	//}
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

bool GameManager::sendAllProvincesFromHost(NetworkManager ** NM) {
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
		int playersize = players.size();
		for (int i = 0; i < playersize; i++) {
			_provinces += to_string(players[i]->getMoney()) + ",";
		}
		if ((*NM)->sendStringFromHost(_provinces) != "")
			return true;
		return false;
	}
}

bool GameManager::getAllProvincesFromHost(NetworkManager ** NM) {
	string _provinces = "";
	if ((*NM)->connectionType != "h") {
		string received = (*NM)->sendStringFromHost(_provinces);
		if (received != "") {
			vector<string> data = (*NM)->split(received, ',');
			for (int i = 0; i < 168; i += 4) {

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
				if (casLev == 1) {
					buildCastle(playerChanged, provinceChanged);
				}

			}
			int a = 0;
			for (int j = 168; j < data.size(); j++) {
				players[a++]->setMoney(stoi(data[j]));
			}
			return true;
		}
		//cout << "Couldnt get it from host!" << endl;
		return false;
	}
	//showWorldStatus();
}

void GameManager::sendAllProvincesFromClientToHost(NetworkManager ** NM) {
	string _provinces;
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
		int playersize = players.size();
		for (int i = 0; i < playersize; i++) {
			_provinces += to_string(players[i]->getMoney()) + ",";
		}

		if ((*NM)->sendStringFromClientToHost(_provinces) == "") {}
	}
}

bool GameManager::getAllProvincesFromClient(NetworkManager ** NM) {
	string _provinces;
	if ((*NM)->connectionType == "h") {
		string received = (*NM)->getStringFromClient();
		if (received != "") {
			vector<string> data = (*NM)->split(received, ',');
			for (int i = 0; i < 168; i += 4) {

				string dummy;
				int pId = stoi(data[i]);
				int cId = stoi(data[i + 1]);
				int count = stoi(data[i + 2]);
				int casLev = stoi(data[i + 3]);

				Player * playerChanged = this->getPlayerByID(pId, dummy);
				Province * provinceChanged = this->getWorldMap()->getProvinceByID(cId);
				if (playerChanged != NULL)
					playerChanged->captureProvince(this->getWorldMap(), provinceChanged);
				provinceChanged->setNumberOfSoldiers(count);
				provinceChanged->getCastle()->setLevel(casLev);
				if (casLev == 1) {
					buildCastle(playerChanged, provinceChanged);
				}
			}
			int a = 0;
			for (int j = 168; j < data.size(); j++) {
				players[a++]->setMoney(stoi(data[j]));
			}
			return true;
		}
		return false;
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

	screenWidth = GetSystemMetrics(SM_CXSCREEN) / 2; //  1920; //
	screenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;  // 1080; //

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
	if (!battleNotificationImg.loadFromFile("assets/armyNotifier.png")) {
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
	topPanel = new TopBar(topPanelImg, coinImg, actionImg, timerImg, crownImg, font);


	mapSprite.setPosition(0, 0);
	mainView.setCenter(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);

	fstream file("assets/coordinates.txt");

	int color;
	string cityName;
	bool stepOne = false;
	bool stepTwo = false;
	string line;
	ifstream myfile("assets/coordinates.txt");

	listOfPhaseNotifiers.push_back(new PhaseNotifier(1, placementPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 5));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(2, attackPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 5));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(3, postAttackPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.70, 5));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(4, fortifyPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 5));
	listOfPhaseNotifiers.push_back(new PhaseNotifier(5, marketPhaseImg, screenWidth / 2, screenHeight / 2, screenWidth *0.75, 5));

	battleNotifier = new BattleNotifier(actionImg, sceleteImg, warriorImg, battleNotificationImg, screenWidth / 2, screenHeight / 2, font);

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

	menuButton.push_back(new MyImage("B1.png"));
	menuButton.push_back(new MyImage("B4.png"));
	menuButton.push_back(new MyImage("B3.png"));
	menuButton.push_back(new MyImage("B1K.png"));
	menuButton.push_back(new MyImage("B4K.png"));
	menuButton.push_back(new MyImage("creditsK.png"));
	menuButton.push_back(new MyImage("B0.png"));

	createGameButtons.push_back(new Button(font));//-
	createGameButtons.push_back(new Button(font));//number
	createGameButtons.push_back(new Button(font));//+
	createGameButtons.push_back(new Button(font));//back
	createGameButtons.push_back(new Button(font));//start game
	createGameButtons.push_back(new Button(font));//random placement
	createGameButtons.push_back(new Button(font));


	joinGameButtons.push_back(new Button(font));//back
	joinGameButtons.push_back(new Button(font));//joinGame

	namesOfRulers.push_back(new Button(font));
	namesOfRulers.push_back(new Button(font));
	namesOfRulers.push_back(new Button(font));
	namesOfRulers.push_back(new Button(font));
	namesOfRulers.push_back(new Button(font));
	namesOfRulers.push_back(new Button(font));


	menuButton[0]->setInitialPosition(screenWidth / 9, 3 * (screenHeight / 8));
	menuButton[0]->setPosition(menuButton[0]->getInitialPosition());
	menuButton[0]->setScale(0.5, 0.5);

	menuButton[1]->setInitialPosition((screenWidth / 9), 4 * (screenHeight / 8));
	menuButton[1]->setPosition(menuButton[1]->getInitialPosition());
	menuButton[1]->setScale(0.5, 0.5);

	menuButton[2]->setInitialPosition((screenWidth / 9), 5 * (screenHeight / 8));
	menuButton[2]->setPosition(menuButton[2]->getInitialPosition());
	menuButton[2]->setScale(0.5, 0.5);

	menuButton[3]->setInitialPosition(screenWidth / 9, 3 * (screenHeight / 8));
	menuButton[3]->setPosition(menuButton[3]->getInitialPosition());
	menuButton[3]->setScale(0.5, 0.5);

	menuButton[4]->setInitialPosition(screenWidth / 9, 4 * (screenHeight / 8));
	menuButton[4]->setPosition(menuButton[4]->getInitialPosition());
	menuButton[4]->setScale(0.5, 0.5);

	menuButton[5]->setInitialPosition(screenWidth / 9, 5 * (screenHeight / 8));
	menuButton[5]->setPosition(menuButton[5]->getInitialPosition());
	menuButton[5]->setScale(0.5, 0.5);

	menuButton[6]->setInitialPosition(screenWidth / 9, 2 * (screenHeight / 8));
	menuButton[6]->setPosition(menuButton[6]->getInitialPosition());
	menuButton[6]->setScale(0.5, 0.5);
	//MENU BUTTON

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
	buttons[0]->setPosition(buttons[2]->getPosition().x, buttons[2]->getPosition().y + buttons[2]->getSize().y + 20);
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
	buttons[1]->setTextSize(numberTextSize - 2);
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







	//CreateGame page
	this->backForCreateGame = RectangleShape(Vector2f(screenWidth / 2, screenHeight / 2));
	this->backForCreateGame.setFillColor(Color(0, 0, 0, 120));
	//this->backForCreateGame.setSize();
	this->backForCreateGame.setPosition(screenWidth / 2 - backForCreateGame.getSize().x / 2, screenHeight / 2 - backForCreateGame.getSize().y / 2);

	if (!this->backGround.loadFromFile("assets/background2.png")) {
		cout << "Unable to open file" << endl;
	}
	this->textureOfBackGround.loadFromImage(backGround);
	this->spriteOfBackground.setTexture(textureOfBackGround);
	this->spriteOfBackground.setTextureRect(IntRect(0, 0, (int)textureOfBackGround.getSize().x, (int)textureOfBackGround.getSize().y));
	this->spriteOfBackground.setScale(screenWidth / backGround.getSize().x, screenHeight / backGround.getSize().y);
	this->spriteOfBackground.setPosition(0, 0);



	createGameButtons[1]->setTextSize(numberTextSize);
	createGameButtons[1]->setSize(numberTextSize, numberTextSize);
	createGameButtons[1]->setText("1");
	createGameButtons[1]->setPosition(screenWidth / 2 - createGameButtons[1]->getGlobalBounds().width / 2, screenHeight / 2);
	createGameButtons[1]->setTextColor(sf::Color::Black);
	createGameButtons[1]->setFillColor(sf::Color::Red);
	createGameButtons[1]->setOutlineThickness(2);
	createGameButtons[1]->setOutlineColor(Color::Black);
	createGameButtons[1]->setFlag(true);

	createGameButtons[0]->setTextSize(numberTextSize);
	createGameButtons[0]->setSize(numberTextSize, numberTextSize);
	createGameButtons[0]->setText("-");
	createGameButtons[0]->setPosition(createGameButtons[1]->getPosition().x - createGameButtons[1]->getGlobalBounds().width, screenHeight / 2);
	createGameButtons[0]->setTextColor(sf::Color::Black);
	createGameButtons[0]->setFillColor(sf::Color::Red);
	createGameButtons[0]->setOutlineThickness(2);
	createGameButtons[0]->setOutlineColor(Color::Black);
	createGameButtons[0]->setFlag(true);


	createGameButtons[2]->setTextSize(numberTextSize);
	createGameButtons[2]->setSize(numberTextSize, numberTextSize);
	createGameButtons[2]->setText("+");
	createGameButtons[2]->setPosition(createGameButtons[1]->getPosition().x + createGameButtons[1]->getGlobalBounds().width, screenHeight / 2);
	createGameButtons[2]->setTextColor(sf::Color::Black);
	createGameButtons[2]->setFillColor(sf::Color::Red);
	createGameButtons[2]->setOutlineThickness(2);
	createGameButtons[2]->setOutlineColor(Color::Black);
	createGameButtons[2]->setFlag(true);

	createGameButtons[3]->setTextSize(numberTextSize);
	createGameButtons[3]->setSize(100, numberTextSize);
	createGameButtons[3]->setText("Back");
	createGameButtons[3]->setPosition(0, 0);
	createGameButtons[3]->setTextColor(sf::Color::White);
	createGameButtons[3]->setFillColor(sf::Color::Black);
	createGameButtons[3]->setOutlineThickness(2);
	createGameButtons[3]->setOutlineColor(Color::Black);
	createGameButtons[3]->setFlag(true);

	createGameButtons[4]->setTextSize(numberTextSize);
	createGameButtons[4]->setSize(200, numberTextSize);
	createGameButtons[4]->setText("Create Game");
	createGameButtons[4]->setPosition(screenWidth / 2 - createGameButtons[4]->getGlobalBounds().width / 2, createGameButtons[2]->getPosition().y + 40);
	createGameButtons[4]->setTextColor(sf::Color::White);
	createGameButtons[4]->setFillColor(sf::Color::Black);
	createGameButtons[4]->setOutlineThickness(2);
	createGameButtons[4]->setOutlineColor(Color::Black);
	createGameButtons[4]->setFlag(true);

	createGameButtons[5]->setTextSize(numberTextSize);
	createGameButtons[5]->setSize(300, numberTextSize);
	createGameButtons[5]->setText("Random Placement");
	createGameButtons[5]->setPosition(screenWidth / 2 - createGameButtons[5]->getGlobalBounds().width / 2, createGameButtons[2]->getPosition().y - 40);
	createGameButtons[5]->setTextColor(sf::Color::White);
	if (_randomPlacement)
		createGameButtons[5]->setFillColor(sf::Color::Green);
	else
		createGameButtons[5]->setFillColor(sf::Color::Red);
	createGameButtons[5]->setOutlineThickness(2);
	createGameButtons[5]->setOutlineColor(Color::Black);
	createGameButtons[5]->setFlag(true);

	createGameButtons[6]->setTextSize(numberTextSize);
	createGameButtons[6]->setSize(200, numberTextSize);
	createGameButtons[6]->setText("Start Game");
	createGameButtons[6]->setPosition(screenWidth / 2 - createGameButtons[4]->getGlobalBounds().width / 2, createGameButtons[2]->getPosition().y + 80);
	createGameButtons[6]->setTextColor(sf::Color::White);
	createGameButtons[6]->setFillColor(sf::Color::Black);
	createGameButtons[6]->setOutlineThickness(2);
	createGameButtons[6]->setOutlineColor(Color::Black);
	createGameButtons[6]->setFlag(true);


	joinGameButtons[0]->setTextSize(numberTextSize);
	joinGameButtons[0]->setSize(100, numberTextSize);
	joinGameButtons[0]->setText("Back");
	joinGameButtons[0]->setPosition(0, 0);
	joinGameButtons[0]->setTextColor(sf::Color::White);
	joinGameButtons[0]->setFillColor(sf::Color::Black);
	joinGameButtons[0]->setOutlineThickness(2);
	joinGameButtons[0]->setOutlineColor(Color::Black);
	joinGameButtons[0]->setFlag(true);

	joinGameButtons[1]->setTextSize(numberTextSize);
	joinGameButtons[1]->setSize(200, numberTextSize);
	joinGameButtons[1]->setText("Join Game");
	joinGameButtons[1]->setPosition(screenWidth / 2 - joinGameButtons[1]->getGlobalBounds().width / 2, screenHeight / 2);
	joinGameButtons[1]->setTextColor(sf::Color::White);
	joinGameButtons[1]->setFillColor(sf::Color::Black);
	joinGameButtons[1]->setOutlineThickness(2);
	joinGameButtons[1]->setOutlineColor(Color::Black);
	joinGameButtons[1]->setFlag(true);

	namesOfRulers[0]->setTextSize(numberTextSize);
	namesOfRulers[0]->setSize(250, numberTextSize);
	namesOfRulers[0]->setText("Napoleon");
	namesOfRulers[0]->setPosition(0, screenHeight * 0.2);
	namesOfRulers[0]->setTextColor(sf::Color::White);
	namesOfRulers[0]->setFillColor(sf::Color::Black);
	namesOfRulers[0]->setOutlineThickness(2);
	namesOfRulers[0]->setOutlineColor(Color::Black);
	namesOfRulers[0]->setFlag(true);


	namesOfRulers[1]->setTextSize(numberTextSize);
	namesOfRulers[1]->setSize(250, numberTextSize);
	namesOfRulers[1]->setText("Caesar");
	namesOfRulers[1]->setPosition(namesOfRulers[0]->getPosition().x, screenHeight * 0.2 + 40);
	namesOfRulers[1]->setTextColor(sf::Color::White);
	namesOfRulers[1]->setFillColor(sf::Color::Black);
	namesOfRulers[1]->setOutlineThickness(2);
	namesOfRulers[1]->setOutlineColor(Color::Black);
	namesOfRulers[1]->setFlag(true);

	namesOfRulers[2]->setTextSize(numberTextSize);
	namesOfRulers[2]->setSize(250, numberTextSize);
	namesOfRulers[2]->setText("Mehmet Fatih");
	namesOfRulers[2]->setPosition(namesOfRulers[0]->getPosition().x, screenHeight * 0.2 + 80);
	namesOfRulers[2]->setTextColor(sf::Color::White);
	namesOfRulers[2]->setFillColor(sf::Color::Black);
	namesOfRulers[2]->setOutlineThickness(2);
	namesOfRulers[2]->setOutlineColor(Color::Black);
	namesOfRulers[2]->setFlag(true);

	namesOfRulers[3]->setTextSize(numberTextSize);
	namesOfRulers[3]->setSize(250, numberTextSize);
	namesOfRulers[3]->setText("Genghis Khan");
	namesOfRulers[3]->setPosition(namesOfRulers[0]->getPosition().x, screenHeight * 0.2 + 120);
	namesOfRulers[3]->setTextColor(sf::Color::White);
	namesOfRulers[3]->setFillColor(sf::Color::Black);
	namesOfRulers[3]->setOutlineThickness(2);
	namesOfRulers[3]->setOutlineColor(Color::Black);
	namesOfRulers[3]->setFlag(true);

	namesOfRulers[4]->setTextSize(numberTextSize);
	namesOfRulers[4]->setSize(250, numberTextSize);
	namesOfRulers[4]->setText("Alexander Great");
	namesOfRulers[4]->setPosition(namesOfRulers[0]->getPosition().x, screenHeight * 0.2 + 160);
	namesOfRulers[4]->setTextColor(sf::Color::White);
	namesOfRulers[4]->setFillColor(sf::Color::Black);
	namesOfRulers[4]->setOutlineThickness(2);
	namesOfRulers[4]->setOutlineColor(Color::Black);
	namesOfRulers[4]->setFlag(true);

	namesOfRulers[5]->setTextSize(numberTextSize);
	namesOfRulers[5]->setSize(250, numberTextSize);
	namesOfRulers[5]->setText("Elizabeth I");
	namesOfRulers[5]->setPosition(namesOfRulers[0]->getPosition().x, screenHeight * 0.2 + 200);
	namesOfRulers[5]->setTextColor(sf::Color::White);
	namesOfRulers[5]->setFillColor(sf::Color::Black);
	namesOfRulers[5]->setOutlineThickness(2);
	namesOfRulers[5]->setOutlineColor(Color::Black);
	namesOfRulers[5]->setFlag(true);



	////CreateGame page///////////////////////////////////////////


	cout << "done" << endl;
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

void WindowManager::menuScreen(RenderWindow& window, Event& event) {
	static int x = 0;
	//sf::Texture texture;
	//if (!texture.loadFromFile("assets/background2.png"));


	//// Assign it to a sprite
	//sf::Sprite sprite2;
	//sprite2.setTexture(texture);
	// Draw the textured sprite
	spriteOfBackground.setScale((float)screenWidth / spriteOfBackground.getTexture()->getSize().x, screenHeight / (float)spriteOfBackground.getTexture()->getSize().y);
	window.draw(spriteOfBackground);
	menuEvents(event, 1);


	while (window.pollEvent(event)) {
		cout << "we are checking" << x++ << endl;
		if (event.type == sf::Event::Closed)
			window.close();

		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				menuEvents(event, 0);
			}
		}

	}
}
void WindowManager::menuEvents(sf::Event& e, int i) {
	int id = 0;

	if (i == 0) {
		for (auto it = menuButton.begin(); it != menuButton.end(); it++) {
			if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + ((*it)->getSize().x) / 2 &&
				(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + ((*it)->getSize().y) / 2) {
				buttonClicked(id, e, window);
				break;
			}
			id++;

		}
	}
	if (i == 1) {
		int id = 0;
		for (auto it = menuButton.begin(); it != menuButton.end(); it++) {

			if (IntRect((*it)->getPosition().x, (*it)->getPosition().y, (*it)->getGlobalBounds().width, (*it)->getGlobalBounds().height).contains(mu.getPosition(window))) {
				changeButton(id);
				break;
			}
			id++;

		}

		window.draw(*menuButton[0]);
		window.draw(*menuButton[1]);
		window.draw(*menuButton[2]);
		window.draw(*menuButton[6]);
		window.display();

		for (int i = 0; i < menuButton.size(); i++) {
			menuButton[i]->setColor(Color::White);
		}
		return;




	}
}

void WindowManager::changeButton(int id) {
	menuButton[id]->setColor(Color(200, 200, 200));
}

void WindowManager::createGamePage(RenderWindow& window, Event& event) {
	page = CREATE_GAME_SCREEN;
	window.clear(Color::White);

	window.draw(spriteOfBackground);
	window.draw(backForCreateGame);
	createGameButtons[1]->setText(to_string(numberOfPlayersInCreateGame));

	for (int i = 0; i < createGameButtons.size(); i++) {
		createGameButtons[i]->draw(window);
	}

	window.display();

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				createGamePageEvents(event, 0);
			}
		}
	}
}
void WindowManager::createGamePageEvents(sf::Event& e, int i) {
	int id = 0;
	if (i == 0) {
		for (auto it = createGameButtons.begin(); it != createGameButtons.end(); it++) {
			if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + ((*it)->getSize().x) &&
				(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + ((*it)->getSize().y)) {
				cout << "CreateGame Events BUTTON" << id << endl;
				buttonClicked(id, e, window);
				return;
			}
			id++;
		}
	}
}
void WindowManager::joinGamePage(RenderWindow& window, Event& event) {
	page = JOIN_GAME_SCREEN;
	window.clear(Color::White);

	window.draw(spriteOfBackground);
	window.draw(backForCreateGame);

	for (int i = 0; i < joinGameButtons.size(); i++) {
		joinGameButtons[i]->draw(window);
	}
	for (int i = 0; i < namesOfRulers.size(); i++) {
		namesOfRulers[i]->draw(window);
	}

	window.display();


	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{

				joinGamePageEvents(event, 1);
				joinGamePageEvents(event, 0);
			}
		}
	}
}
void WindowManager::joinGamePageEvents(sf::Event& e, int i) {
	int id = 0;
	if (i == 0) {
		for (auto it = joinGameButtons.begin(); it != joinGameButtons.end(); it++) {
			if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + ((*it)->getSize().x) &&
				(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + ((*it)->getSize().y)) {
				cout << "CreateGame Events BUTTON" << id << endl;
				buttonClicked(id, e, window);
				return;
			}
			id++;
		}
	}
	if (i == 1) {
		for (auto it = namesOfRulers.begin(); it != namesOfRulers.end(); it++) {
			if ((*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + ((*it)->getSize().x) &&
				(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + ((*it)->getSize().y))
			{
				cout << "selectUnit Events BUTTON" << id << endl;
				if (nameSelectedInJoin != (*it)->text.getString()) { 
					nameSelectedInJoin = (*it)->text.getString();
					hoverButtonOfNames(id);
				}
				return;
				
			}
			id++;
		}
	}


}

void WindowManager::hoverButtonOfNames(int id) {
	for (int i = 0; i < namesOfRulers.size(); i++) {
		if (i == id)
			namesOfRulers[i]->setFillColor(Color(0, 100, 0));
		else
			namesOfRulers[i]->setFillColor(Color::Black);
	}
}

void WindowManager::startGameOnOneComputerPage(RenderWindow& window, Event& event) {
	page = START_GAME_ON_ONE_COMPUTER_SCREEN;
	window.clear(Color::White);
	window.draw(spriteOfBackground);
	window.draw(backForCreateGame);
	createGameButtons[1]->setText(to_string(numberOfPlayersInCreateGame));

	for (int i = 0; i < createGameButtons.size(); i++) {
		if(i!=4)
			createGameButtons[i]->draw(window);
	}

	window.display();

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				startGameOnOneComputerPageEvents(event, 0);
			}
		}
	}
}
void WindowManager::startGameOnOneComputerPageEvents(sf::Event& e, int i) {
	int id = 0;
	if (i == 0) {
		for (auto it = createGameButtons.begin(); it != createGameButtons.end(); it++) {
			if (id!=4 && (*it)->getPosition().x < e.mouseButton.x && e.mouseButton.x < (*it)->getPosition().x + ((*it)->getSize().x) &&
				(*it)->getPosition().y < e.mouseButton.y && e.mouseButton.y < (*it)->getPosition().y + ((*it)->getSize().y)) {
				cout << "CreateGame Events BUTTON" << id << endl;
				buttonClicked(id, e, window);
				return;
			}
			id++;
		}
	}
}



//void WindowManager::menuScreen(RenderWindow & window, Event & event) {
//	static int x = 0;
//	while (window.pollEvent(event)) {
//		cout << "we are checking" << x++ << endl;
//		if (event.type == sf::Event::Closed)
//			window.close();
//		else if (event.type == sf::Event::MouseButtonPressed)
//		{
//			if (event.mouseButton.button == sf::Mouse::Left)
//			{
//				checkClickEvents(event);
//			}
//		}
//	}
//
//	window.setView(mainView);
//
//	window.clear(sf::Color::Black);
//
//	window.setView(window.getDefaultView());
//
//	buttons[5]->draw(window);
//	buttons[6]->draw(window);
//	buttons[7]->draw(window);
//	buttons[8]->draw(window);
//
//	window.display();
//}


void WindowManager::multGameComp(RenderWindow & window, Event & event) {
	time = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	time = time / 1200;
	if (time > 20)
		time = 20;
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
				else if (event.mouseButton.y < bottomUpperMargin) {
					resetHighlights();
					resetLines();
					first = NULL;
					second = NULL;
					isProvinceClicked = 0;
					cout << "Not a city" << endl;
					provinceNameTxt.setString("");
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
	lineForProvinces->draw(window, time);

	for (auto it = lines.begin(); it != lines.end(); it++) {
		(*it)->draw(window, time);
	}

	drawAllArmies(window, event);
	drawAllCastles(window, event);

	//for (auto it = castles.begin(); it != castles.end(); it++) {
	//	window.draw(*(*it));
	//}


	window.setView(window.getDefaultView());
	window.draw(lowerPanel);
	battleNotifier->draw(window, time);



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
			if (buttons[i]->getFlag())
				buttons[i]->draw(window);
		}
	}

	for (int i = 0; i < playerStatus.size(); i++) {
		window.draw(*playerStatus[i]);
	}

	int currentPlayer = GM->currentPlayer;
	string currentPlayerName;
	Player * curPlayer = GM->getPlayerByID(currentPlayer, currentPlayerName);

	if (page == 2) {
		topPanel->update(curPlayer->getMoney(), curPlayer->getLeftSoldier(), totalTurn, currentPlayerName);
	}
	else if (page == 1) {
		Player * me = GM->getPlayerByID(userTurn, currentPlayerName);
		topPanel->update(me->getMoney(), me->getLeftSoldier(), totalTurn, currentPlayerName);
	}
	topPanel->draw(window);

	for (int i = 0; i < listOfPhaseNotifiers.size(); i++) {
		if (phase == listOfPhaseNotifiers[i]->phaseId)
			listOfPhaseNotifiers[i]->activate();
		else
			listOfPhaseNotifiers[i]->deactivate();
	}
	for (int i = 0; i < listOfPhaseNotifiers.size(); i++) {
		listOfPhaseNotifiers[i]->draw(window, time);
	}

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

	window.create(sf::VideoMode(screenWidth, screenHeight), "Risk");
	window.setPosition(Vector2i(0, 0));


	while (window.isOpen()) {
		sf::Event event;
		NM->socket.setBlocking(false);
		if (page == 0) {
			menuScreen(window, event);
			if (create_game_clicked) {
				NM->getNamesConnect(&GM);
				provinceNameTxt.setString("Your Turn");
				if (playerCount == 2) {
					create_game_clicked = false;
				}
			}
			else if (join_game_clicked) {
				NM->getAllNames(&GM);
			}
			continue;
		}
		else if (page == 1) {
			multGameComp(window, event);
			if (this->phase == END_TURN) {

				if (userTurn != 0 && GM->getAllProvincesFromHost(&NM)) {
					if (!getInitialState) {
						getInitialState = true;
						if (_randomPlacement)
							continue;
					}
					turn++;
					GM->currentPlayer = turn;
					if (turn == this->GM->getPlayers().size()) {
						turn = 0;
						GM->currentPlayer = turn;
						totalTurn++;
					}

					if (turn == userTurn) {
						provinceNameTxt.setString("Your Turn");
						if (!_randomPlacement && GM->getWorldMap()->ownerCount() != 42)
							phase = INITIAL_PHASE;
						else {
							string dummy;
							GM->getPlayerByID(userTurn, dummy)->setLeftSoldier(GM->getPlayerByID(userTurn, dummy)->getNumberOfProvinces() / 3);
							phase = PLACEMENT_PHASE;
						}
					}
					else {
						provinceNameTxt.setString("Waiting for opponent: " + (GM->getPlayers())[turn]->getName());
					}
					displayPlayerStatus();
				}

				else if (userTurn == 0) {

					if (GM->getAllProvincesFromClient(&NM)) {
						GM->sendAllProvincesFromHost(&NM);
						turn++;
						GM->currentPlayer = turn;

						if (turn == this->GM->getPlayers().size()) {
							turn = 0;
							GM->currentPlayer = turn;
							totalTurn++;
						}

						if (turn == userTurn) {
							provinceNameTxt.setString("Your Turn");
							if (!_randomPlacement && GM->getWorldMap()->ownerCount() != 42)
								phase = INITIAL_PHASE;
							else {
								string dummy;
								GM->getPlayerByID(userTurn, dummy)->setLeftSoldier(GM->getPlayerByID(userTurn, dummy)->getNumberOfProvinces() / 3);
								phase = PLACEMENT_PHASE;
							}

						}
						else {
							provinceNameTxt.setString("Waiting for opponent: " + (GM->getPlayers())[turn]->getName());
						}
						displayPlayerStatus();
					}
				}
			}
			continue;
		}
		else if (page == 2) {
			multGameComp(window, event);
			continue;
		}
		else if (page == CREATE_GAME_SCREEN) {

			createGamePage(window, event);
		}
		else if (page == JOIN_GAME_SCREEN) {
			joinGamePage(window, event);
		}
		else if (page == START_GAME_ON_ONE_COMPUTER_SCREEN)
		{
			startGameOnOneComputerPage(window,event);
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
	if (!buffer.loadFromFile("assets/sounds/effects/click1.wav")) {
		cout << "ERROR LOADING SOUND FILE" << endl;
	}
	sound.setBuffer(buffer);
	sound.play();
	if (page == MENU_SCREEN) {


		if (id == 0) {
			//playerCount = 3; // will be taken from user
			//const string name = "player";
			//for (int i = 0; i < playerCount; i++) {
			//	this->GM->addPlayer(name + to_string(i + 1));
			//}
			//if (_randomPlacement == true) {
			//	GM->randomPlacement();
			//	string dummy;
			//	for (int i = 0; i < playerCount; i++)
			//		GM->getPlayerByID(i, dummy)->setLeftSoldier(GM->getPlayerByID(i, dummy)->getNumberOfProvinces() / 3);
			//	phase = PLACEMENT_PHASE;
			//}
			//else {
			//	phase = INITIAL_PHASE;
			//}
			//cout << "clickked weghwioefng" << endl;
			//turn = 0;
			//page = COMPUTER_GAME_SCREEN;

			page = CREATE_GAME_SCREEN;
			return;
		}
		else if (id == 1) {
			page = JOIN_GAME_SCREEN;
			return;
		}
		else if (id == 5) {
			/*userName = "host";
			NM->createNetwork(&GM, "h", userName);
			create_game_clicked = true;*/
		}

		else if (id == 6) {
			cout << "start game" << endl;
			page = 7;
			//Burak this is for play on one computer
			//button play is pressed and it returns  id6
		}
		else if (id == 7) {
			userName = "client2";
			NM->createNetwork(&GM, "c2", userName);
			join_game_clicked = true;
		}
		else if (id == 8) {
			NM->startGame();
			userTurn = GM->getPlayerTurn(userName);
			if (_randomPlacement) {
				GM->randomPlacement();
				GM->sendAllProvincesFromHost(&NM);
				phase = PLACEMENT_PHASE;
			}
			else {
				phase = INITIAL_PHASE;
			}


			page = 1;
		}
		return;
	}
	else if (page == CREATE_GAME_SCREEN) {

		if (id == 0) {
			if (numberOfPlayersInCreateGame > 1)
				numberOfPlayersInCreateGame--;
		}

		else if (id == 2) {
			if (numberOfPlayersInCreateGame < 6)
				numberOfPlayersInCreateGame++;
		}
		else if (id == 3) {
			page = MENU_SCREEN;
		}
		else if (id == 4 && !create_game_clicked) {
			create_game_clicked = true;
			//Burak  --> 
			userName = "Host";
			NM->createNetwork(&GM, "h", userName);
			create_game_clicked = true;
		}
		else if (id == 5) {
			if (_randomPlacement) {
				_randomPlacement = false;
				createGameButtons[id]->setFillColor(Color(255, 0, 0));
			}
			else {
				_randomPlacement = true;
				createGameButtons[id]->setFillColor(Color(0, 255, 0));
			}

		}
		else if (id == 6) {
			cout << "start game pressed" << endl;
			//Start Game Burak right your code!!!!!!!!!!!!!!
		}

	}
	else if (page == JOIN_GAME_SCREEN) {
		if (id == 0) {
			page = MENU_SCREEN;
		}
		if (id == 1 && !join_game_clicked) {

			NM->createNetwork(&GM, "client", nameSelectedInJoin);
			join_game_clicked = true;
			
		}
	}
	else if (page = START_GAME_ON_ONE_COMPUTER_SCREEN) {
			if (id == 0) {
			if (numberOfPlayersInCreateGame > 1)
				numberOfPlayersInCreateGame--;
		}

		else if (id == 2) {
			if (numberOfPlayersInCreateGame < 6)
				numberOfPlayersInCreateGame++;
		}
		else if (id == 3) {
			page = MENU_SCREEN;
		}
		else if (id == 5) {
			if (_randomPlacement) {
				_randomPlacement = false;
				createGameButtons[id]->setFillColor(Color(255, 0, 0));
			}
			else {
				_randomPlacement = true;
				createGameButtons[id]->setFillColor(Color(0, 255, 0));
			}

		}
		else if (id == 6) {
			cout << "start game on one computer pressed" << endl;
			//Start on one computer Game Burak right your code!!!!!!!!!!!!!! This is for one computer.
		}
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
			if (player->getLeftSoldier() == 0)
				phase = ATTACKING_PHASE;
			buttons[ATTACK_BUTTON]->setText("Attack");
			buttons[NEXT_PHASE_BUTTON]->setText("Fortify Phase");
			lineForProvinces->setVisible(false);
			resetHighlights();

		}
		else if (phase == ATTACKING_PHASE) {
			phase = FORTIFY_PHASE;
			lineForProvinces->setVisible(false);
			buttons[NEXT_PHASE_BUTTON]->setText("Market Phase");
			buttons[ATTACK_BUTTON]->setText("Fortify");
			lineForProvinces->setVisible(false);
			resetHighlights();
		}
		else if (phase == FORTIFY_PHASE) {
			phase = MARKET_PHASE;
			lineForProvinces->setVisible(false);
			buttons[TURN_WHEEL_BUTTON]->setFlag(true);
			buttons[ATTACK_BUTTON]->setFlag(false);
			// ------------------------------ CASTLE ATTACKS BEGINS
			string dum;
			GM->castleAttacks(GM->getPlayerByID(turn, dum));
			// ------------------------------ CASTLE ATTACKS ENDS

			buttons[NEXT_PHASE_BUTTON]->setText("End Turn");
			int soldierPrice = 50;
			soldierAmount = player->getMoney() / soldierPrice;
			buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));

		}
		else if (phase == MARKET_PHASE) {

			if (page == COMPUTER_GAME_SCREEN) {
				string dummy;
				for (int i = 0; i < playerCount; i++)
					GM->getPlayerByID(i, dummy)->setLeftSoldier(GM->getPlayerByID(i, dummy)->getNumberOfProvinces() / 3);
				phase = PLACEMENT_PHASE;
				turn++;
				if (turn == playerCount) {
					turn = 0;
					for (int i = 0; i < playerCount; i++) {
						string dummy;
						int res = 0;
						Player* player0 = GM->getPlayerByID(i, dummy);
						player0->setLeftSoldier(player0->getNumberOfProvinces() / 3 + player0->howManyCont(res));
						player0->setMoney(player0->getMoney() + 200);
						cout << "player" << i + 1 << " has " << res << " Continents";
					}
					totalTurn++;
				}
				GM->currentPlayer = turn;
			}

			if (page == 1) {
				if (userTurn == 0) {
					this->GM->sendAllProvincesFromHost(&NM);
					turn++;
					GM->currentPlayer = turn;
					provinceNameTxt.setString("Waiting for opponent: " + (GM->getPlayers())[turn]->getName());
				}
				else {
					this->GM->sendAllProvincesFromClientToHost(&NM);
				}

				phase = END_TURN;
			}

			buttons[TURN_WHEEL_BUTTON]->setFlag(false);
			buttons[ATTACK_BUTTON]->setFlag(false);
			buttons[NEXT_PHASE_BUTTON]->setFlag(false);

			wheel = false;
			countForWheel = 0;
			takeCastle = false;
			wonSoldier = false;


		}
		else if (phase == END_TURN) {
		}

	}

	else if (id == ATTACK_BUTTON) {
		if (phase == ATTACKING_PHASE) {
			if (isProvinceClicked == 2) {
				if (!buffer.loadFromFile("assets/sounds/attack.wav")) {
					cout << "ERROR LOADING SOUND FILE" << endl;
				}
				sound.setBuffer(buffer);
				sound.play();
				if (GM->attack(player, second->getOwner(), first, second, soldierAmount)) {
					wheel = true;
					phase = POST_ATTACK; //change phase 
					buttons[ATTACK_BUTTON]->setText("Place");
					provinceNameTxt.setString("Enter the number of soldiers you want to place on this city:");
					soldierAmount = first->getNumberOfSoldiers() - 1;
					buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
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
			lineForProvinces->setVisible(false);
			resetHighlights();
		}
		else if (phase == FORTIFY_PHASE) {
			lineForProvinces->setVisible(false);
			soldierAmount = first->getNumberOfSoldiers() - 1;
			buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
			if (GM->fortify(player, first, second, soldierAmount)) {
				displayProvinceInfo(second);
			}
		}
		else if (phase == END_TURN) {}
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

	else if (id == TURN_WHEEL_BUTTON && countForWheel < 1 && phase == MARKET_PHASE && wheel) {
		countForWheel++;
		buttons[TURN_WHEEL_BUTTON]->setFlag(false);
		if (!buffer.loadFromFile("assets/sounds/effects/wheel.wav")) {
			cout << "ERROR LOADING SOUND FILE" << endl;
		}
		sound.setBuffer(buffer);
		sound.play();
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
	}
}

void WindowManager::handleWheel() {
	string temp;
	int index = ((int)(rotateAmount / 45)) % 8;
	if (index == 0) {
		//draw bonus card
		if (GM->getPlayerByID(turn, temp)->getMoney() <= 250) {
			GM->getPlayerByID(turn, temp)->setMoney(0);
			provinceNameTxt.setString("You have lost all your money");
		}
		else {
			GM->getPlayerByID(turn, temp)->setMoney(GM->getPlayerByID(turn, temp)->getMoney() - 250);
			provinceNameTxt.setString("You have lost 250 gold");
		}
	}
	else if (index == 1) {
		//bankrupt
		GM->getPlayerByID(turn, temp)->setMoney(0);
		provinceNameTxt.setString("You have lost all your money");
	}
	else if (index == 2) {
		//250 gold
		GM->getPlayerByID(turn, temp)->setMoney(GM->getPlayerByID(turn, temp)->getMoney() + 250);
		provinceNameTxt.setString("You have earned 250 gold");

	}
	else if (index == 3) {
		//pass
	}
	else if (index == 4) {
		//build castle

		GM->getPlayerByID(turn, temp)->setMoney(GM->getPlayerByID(turn, temp)->getMoney() + 50);
		images[3]->inMove = true;
		takeCastle = true;
		provinceNameTxt.setString("You have a castle to build");
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
		string from = prov->getOwner()->getName();
		prov->setOwner(GM->getPlayerByID(turn, pname));
		cout << pname << "gets " << prov->getName() << endl;
		provinceNameTxt.setString("You got " + prov->getName() + " from " + from);
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
		prov->setOwner(GM->getPlayerByID((turn + 1) % 3, pname));
		provinceNameTxt.setString("You lost " + prov->getName() + " to " + pname);
	}
	else if (index == 7) {
		//take 3 soldiers
		GM->getPlayerByID(turn, temp)->setLeftSoldier(GM->getPlayerByID(turn, temp)->getLeftSoldier() + 3);
		string dum;
		soldierAmount = GM->getPlayerByID(turn, dum)->getLeftSoldier();
		buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
		wonSoldier = true;
		provinceNameTxt.setString("You have 3 soldiers to place");

	}
}

void WindowManager::provinceClicked(int id) {
	Province* city = GM->getWorldMap()->getProvinceByID(id);
	Player* player = GM->getPlayers()[turn];

	if (city == first)
		return;

	if (phase == PLACEMENT_PHASE) {}
	else if (phase == ATTACKING_PHASE) {
		if (isProvinceClicked == 0) {
			if (city->getOwner() == player) {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
				highlightNeighbors(city);
				if (first->getNumberOfSoldiers() > 3)
					soldierAmount = 3;
				else
					soldierAmount = first->getNumberOfSoldiers() - 1;
			}
			else {
				resetHighlights();
				soldierAmount = 0;
			}
			lineForProvinces->setVisible(false);
		}
		else if (isProvinceClicked == 1) {
			if (city->getOwner() != player) {
				if (GM->getWorldMap()->isNeighbor(city, first)) {
					second = city;
					isProvinceClicked = 2;
					resetHighlights();
					highlight(first);
					highlight(second);
					drawLine(first, second);
					provinceNameTxt.setString(first->getName() + " attacks to " + second->getName());
				}
				else {

				}
			}
			else {
				first = city;
				second = NULL;
				isProvinceClicked = 1;
				if (page == 1)
					provinceNameTxt.setString(first->getName());
				resetHighlights();
				highlight(city);
				highlightNeighbors(city);
				lineForProvinces->setVisible(false);
				if (first->getNumberOfSoldiers() > 3)
					soldierAmount = 3;
				else
					soldierAmount = first->getNumberOfSoldiers() - 1;

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
				if (first->getNumberOfSoldiers() > 3)
					soldierAmount = 3;
				else
					soldierAmount = first->getNumberOfSoldiers() - 1;
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
				soldierAmount = 0;
			}
		}
		buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
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
				highlightPossiblePaths(city);
				soldierAmount = first->getNumberOfSoldiers() - 1;
			}
			else {
				resetHighlights();
				soldierAmount = 0;
			}
			lineForProvinces->setVisible(false);
		}
		else if (isProvinceClicked == 1) {
			if (city->getOwner() == player) {
				if (GM->getWorldMap()->hasPath(player, city, first)) {
					second = city;
					isProvinceClicked = 2;
					resetHighlights();
					highlight(first);
					highlight(second);
					//drawLine(first, second);
					drawShortestPath(first, second);
					provinceNameTxt.setString("From " + first->getName() + " to " + second->getName());
				}
				else {

				}
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				if (page == 1)
					provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
				soldierAmount = 0;
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
				highlightPossiblePaths(city);
				lineForProvinces->setVisible(false);
				soldierAmount = first->getNumberOfSoldiers() - 1;
			}
			else {
				first = NULL;
				second = NULL;
				isProvinceClicked = 0;
				provinceNameTxt.setString("");
				resetHighlights();
				lineForProvinces->setVisible(false);
				soldierAmount = 0;
			}
		}

	}
	else if (phase == MARKET_PHASE) {
	}
	buttons[NUMBER_TEXT]->setText(to_string(soldierAmount));
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

void WindowManager::highlightNeighbors(Province* city) {
	vector<Province*> neighbors = GM->getWorldMap()->getNeighbors(city);
	Player* player = GM->getPlayers()[turn];
	for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
		if ((*it)->getOwner() != player) {
			highlight((*it));
		}
	}
}

void WindowManager::highlightPossiblePaths(Province* city) {
	vector<Province*> cities = GM->getWorldMap()->getPossiblePaths(city);
	for (auto it = cities.begin(); it != cities.end(); it++) {
		highlight((*it));
	}
}

void WindowManager::resetHighlights() {
	for (int i = 0; i < listOfArmyBage.size(); i++) {
		listOfArmyBage[i]->setScale(1, 1);
	}
}

void WindowManager::resetLines() {
	lineForProvinces->setVisible(false);
	lines.clear();
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

void WindowManager::drawShortestPath(Province* from, Province* to) {
	lines.clear();
	vector<Province*> test = GM->getWorldMap()->getShortestPath(from, to);

	int size = test.size();
	for (int k = size - 1; k > 0; k--) {

		LineBetweenProvinces* line = new LineBetweenProvinces(lineImg);
		Vector2f firstCoordinates, secondCoordinates;
		for (int i = 0; i < listOfArmyBage.size(); i++) {
			if (listOfArmyBage[i]->nameOfProvince == test[k]->getName())
				firstCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
			if (listOfArmyBage[i]->nameOfProvince == test[k - 1]->getName())
				secondCoordinates = Vector2f(listOfArmyBage[i]->centerCoordinates);
		}

		line->setCoordinates(firstCoordinates, secondCoordinates);
		line->setVisible(true);
		lines.push_back(line);
	}
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
	else if ((event.type == event.MouseButtonReleased&& event.key.code == mouse.Left&& images[id]->inMove && !takeCastle) || (takeCastle && mouse.isButtonPressed(sf::Mouse::Left) && event.key.code == mouse.Left&& images[id]->inMove && id == 3)) {
		sf::Vector2i PixelPos = mouse.getPosition(window);
		sf::Vector2f MousePos = window.mapPixelToCoords(PixelPos, mainView);
		images[id]->inMove = false;
		takeCastle = false;
		int dummy; Province* province; string dum;
		string filename;
		if (id == 0)
			filename = "soldier.wav";
		else if (id == 3) {
			filename = "castle.wav";
		}
		if (!buffer.loadFromFile("assets/sounds/effects/" + filename)) {
			cout << "ERROR LOADING SOUND FILE" << endl;
		}
		sound.setBuffer(buffer);

		images[id]->setPosition(images[id]->getInitialPosition());
		images[id]->setScale(sf::Vector2f(1, 1));
		if (sf::IntRect(0, 0, mapImg.getSize().x, mapImg.getSize().y).contains(sf::Vector2i(MousePos.x, MousePos.y)) && mouse.getPosition(window).y < 500)
		{
			if (id == 0) {
				string provinceName = getProvinceName(window, mouse);
				if (provinceName == "")
					return;
				if (phase == INITIAL_PHASE) {

					if (GM->placeSoldier(turn, provinceName, (buttons[NUMBER_TEXT]->getText()))) {
						if (page == 1) {
							if (userTurn == 0) {
								this->GM->sendAllProvincesFromHost(&NM);
								GM->currentPlayer = turn;
								turn++;
								provinceNameTxt.setString("Waiting for opponent: " + (GM->getPlayers())[turn]->getName());
							}
							else {
								this->GM->sendAllProvincesFromClientToHost(&NM);
							}
							phase = END_TURN;
						}

						if (page == COMPUTER_GAME_SCREEN) {
							if (GM->getWorldMap()->ownerCount() != 42)
								phase = INITIAL_PHASE;
							else
								phase = PLACEMENT_PHASE;
							turn++;
							if (turn == playerCount)
								turn = 0;
						}


						//provinceNameTxt.setString("Player" + to_string(turn + 1) + "'s turn");

					}
				}
				else if (phase == PLACEMENT_PHASE || phase == MARKET_PHASE) {
					if (buttons[NUMBER_TEXT]->getText() == 0) {

					}
					else if (GM->placeSoldier(turn, provinceName, (buttons[NUMBER_TEXT]->getText()))) {
						sound.play();
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
					sound.play();
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
		if (ptr != NULL && (ptr->getCastle())->isBuilt()) {
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
	text.setPosition(this->getPosition().x + this->getSize().x / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + (this->getSize().y / 2) - (text.getCharacterSize() / 2));
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
	text.setPosition(this->getPosition().x + this->getSize().x / 2 - text.getGlobalBounds().width / 2, this->getPosition().y + (this->getSize().y / 2) - (text.getCharacterSize() / 2));
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

void NetworkManager::getNamesConnect(GameManager ** const GM) {
	if (connectionType == "h") {
		string name;

		IpAddress rIP;
		unsigned short port;
		Packet packet;

		if (socket.receive(packet, rIP, port) == Socket::Done) {
			computerID[port] = rIP;
			playerCount++;

			String name;
			packet >> name;
			string display = name;
			
			cout << display << " has joined the room." << endl;
			(*GM)->addPlayer(display);
			playersName += name + ",";
			
		}
		//cout << "Player in the game: " << playerCount << endl;

	}
}

void NetworkManager::getAllNames(GameManager ** const GM) {
	IpAddress tempId;
	unsigned short tempPort;
	Packet packet;

	if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
		String received;
		packet >> received;
		string str = received;
		players = split(str, ',');

		for (int i = 0; i < players.size(); i++) {
			(*GM)->addPlayer(players[i]);
		}
		if (WM != NULL) {
			WM->page = WM->GAME_SCREEN;
			WM->userTurn = WM->GM->getPlayerTurn(WM->userName);
			/*if (WM->_randomPlacement) {
				if (WM->GM->getAllProvincesFromHost(&(WM->NM))) {
					WM->join_game_clicked = false;
					cout << "ALL PROVINCES GOT BEFORE THE game !!!!!!!!" << endl;
				}
			}*/
			WM->phase = WM->END_TURN;
			WM->provinceNameTxt.setString("Waiting for opponent: " + ((*GM)->getPlayers())[0]->getName());
		}
	}
}

void NetworkManager::createNetwork(GameManager ** const GM, string _connectionType, string _name) {
	ip = IpAddress::getLocalAddress();
	string text = " ";

	Packet packet;

	connectionType = _connectionType;

	unsigned short port = 2000;

	if (connectionType == "h") {
		port = 2000;
	}
	else 
		port = Socket::AnyPort;
	/*else if (connectionType == "c2")
		port = 2002;
	else if (connectionType == "c3")
		port = 2003;
	else if (connectionType == "c4")
		port = 2004;
	else if (connectionType == "c5")
		port = 2005;*/

	if (socket.bind(port) != Socket::Done) {
		cout << "Couldnt binded. ";
	}
	else {
		cout << "BINDED !! " << endl;
	}


	if (connectionType == "h") {
		playersName = "";
		playersName += _name + ",";
		(*GM)->addPlayer(_name);
	}
	else {
		//cout << "Enter server ip: ";
		string name;
		name = _name;
		String playerName = name;
		packet << playerName;
		IpAddress sendIP(sIP);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "You have joined the room." << endl;



	}
}

void NetworkManager::startGame() {

	if (connectionType == "h") {
		String sendPlayersName;
		sendPlayersName = "" + playersName;
		Packet packet;
		packet << sendPlayersName;
		string display = sendPlayersName;
		map<unsigned short, IpAddress> ::iterator tempIterator;
		for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
			if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {}
	}
	else {
		return;
	}
}

string NetworkManager::getStringFromClient() {
	if (connectionType == "h") {
		IpAddress tempId;
		unsigned short tempPort;
		Packet packet;
		socket.setBlocking(false);
		if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
			String receivedString;
			packet >> receivedString;
			string stringRec = receivedString;
			return stringRec;
		}
		return "";
	}

}

string NetworkManager::sendStringFromHost(string _sendText) {
	if (connectionType == "h") {
		String sentText = _sendText;
		Packet packet;
		packet << sentText;
		map<unsigned short, IpAddress> ::iterator tempIterator;
		for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
			if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {
				packet >> sentText;
				string  display = sentText;
			} // the socket send or not 
		return "";
	}

	else {

		IpAddress tempId;
		unsigned short tempPort;
		Packet packet;
		socket.setBlocking(false);
		if (socket.receive(packet, tempId, tempPort) == Socket::Done) {			// The socket received or not 
			String receivedString;
			packet >> receivedString;
			string stringRec = receivedString;
			return stringRec;
		}
		return "";
	}

}

string NetworkManager::sendStringFromClientToHost(string _sendText) {
	Packet packet;
	IpAddress sendIP(sIP);

	String sentText = _sendText;
	packet << sentText;

	if (socket.send(packet, sendIP, 2000) == Socket::Done) {
		packet >> sentText;
		string  display = sentText;
		return display;
	} // the socket send or not 
	return "";
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

void LineBetweenProvinces::setCoordinates(Vector2f first, Vector2f second) {
	this->first = first;
	this->second = second;

	lenght = (int)sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
	degree = (float)atan2(second.y - first.y, second.x - first.x) * 180 / 3.14159;

	this->setOrigin(0, img.getSize().y / 2);

	this->setTextureRect(IntRect(step, 0, img.getSize().x, lenght));
	this->setPosition(first);

	this->setRotation(degree);
}

void LineBetweenProvinces::draw(RenderWindow &window, float time) {
	if (visible) {
		step = step - 0.1*time;
		tex.loadFromImage(img);
		tex.setRepeated(true);
		this->setTexture(tex);

		this->setTextureRect(IntRect((int)step, 0, max(0, lenght), img.getSize().y));
		this->setOrigin(0, img.getSize().y / 2);
		window.draw(*this);
	}
}

void LineBetweenProvinces::setVisible(bool flag) {
	this->visible = flag;
}


TopBar::TopBar(Image img, Image coinImg, Image actionImage, Image turnImage, Image playerImage, Font &font) {
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
	coinSprite.setScale(ratio, ratio);

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
	coinField.setSize(Vector2f(100 * ratio, 55 * ratio));
	coinField.setFillColor(Color(0, 0, 0, 30));

	actionField.setSize(Vector2f(100 * ratio, 55 * ratio));
	actionField.setFillColor(Color(0, 0, 0, 30));

	turnField.setSize(Vector2f(100 * ratio, 55 * ratio));
	turnField.setFillColor(Color(0, 0, 0, 30));

	playerField.setSize(Vector2f(180 * ratio, 55 * ratio));
	playerField.setFillColor(Color(0, 0, 0, 30));

	//this->setColor(Color(255, 255, 255, 100));
	this->setPosition(0, 0);
	this->setTextureRect(IntRect(0, 0, panelTexture.getSize().x, 100));
	this->setScale(float(lenght) / panelTexture.getSize().x, 0.65*ratio);



	coinField.setPosition(195 * ratio, 2 * ratio);
	coinSprite.setPosition(coinField.getPosition().x + 5, 5 * ratio);
	coinText.setPosition(coinSprite.getPosition().x + coinSprite.getGlobalBounds().width + 10 * ratio, coinSprite.getPosition().y + coinSprite.getGlobalBounds().height / 2 - coinText.getGlobalBounds().height / 2);

	actionField.setPosition(coinField.getPosition().x + coinField.getGlobalBounds().width + 35, 2 * ratio);
	actionSprite.setPosition(actionField.getPosition().x + 5, 7 * ratio);
	actionText.setPosition(actionSprite.getPosition().x + actionSprite.getGlobalBounds().width + 10 * ratio, actionSprite.getPosition().y + actionSprite.getGlobalBounds().height / 2 - actionText.getGlobalBounds().height / 2);

	turnField.setPosition(actionField.getPosition().x + actionField.getGlobalBounds().width + 35, 2 * ratio);
	turnSprite.setPosition(turnField.getPosition().x + 5, 7 * ratio);
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

PhaseNotifier::PhaseNotifier(int phaseId, Image img, int x, int y, int fx, int fy) {
	this->phaseId = phaseId;
	this->img = img;
	this->tex.loadFromImage(img);
	this->setTexture(this->tex);
	visible = false;
	moveTimer = 0;
	this->setPosition(x - img.getSize().x / 2, y - img.getSize().y / 2);

	this->bigCoordinates = Vector2f(x - img.getSize().x / 2, y - img.getSize().y / 2);
	this->smallCoordinates = Vector2f(fx, fy);
	this->ck.restart();
	this->deactivate();
}


void PhaseNotifier::draw(sf::RenderWindow & window, float time) {
	if (!visible)
		return;
	window.draw(*this);
	moveTimer = this->ck.getElapsedTime().asSeconds() + moveTimer;
	this->ck.restart();

	if (moveTimer > 0.7) {


		if (this->getPosition().x < smallCoordinates.x)
			this->setPosition(this->getPosition().x + 2 * time, this->getPosition().y);

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
		this->setColor(Color(255, 255, 255, 255));
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
	this->setColor(Color(0, 0, 0, 255));
}
void PhaseNotifier::deactivate() {
	if (!visible)
		return;
	moveTimer = 0;
	visible = false;

}

BattleNotifier::BattleNotifier() {

}

BattleNotifier::BattleNotifier(Image img1, Image img2, Image img3, Image back, int x, int y, Font font) {
	imageOfBattle = img1;
	imageOfDeath = img2;
	warriorsImg = img3;
	backPage = back;

	this->font = font;
	visible = false;


	texureOfBattle.loadFromImage(imageOfBattle);
	textureOfDeath.loadFromImage(imageOfDeath);
	textureOfWarriors.loadFromImage(warriorsImg);
	backPageTexture.loadFromImage(backPage);

	this->setTexture(texureOfBattle);
	spriteOfDeath.setTexture(textureOfDeath);
	spriteOfWarriors.setTexture(textureOfWarriors);
	backPageSprite.setTexture(backPageTexture);

	this->setPositionCenter(x, y);
	backPageSprite.setScale(1, 0.8);
	backPageSprite.setPosition(x - backPageSprite.getGlobalBounds().width / 2, y - backPageSprite.getGlobalBounds().height / 2 + 10);

	battleText.setFont(this->font);
	battleText.setCharacterSize(40);
	battleText.setString("Battle");
	Color color = Color(255, 255, 255);
	battleText.setFillColor(color);

	playerName1.setFont(this->font);
	playerName1.setCharacterSize(40);
	playerName1.setStyle(1 << 2);
	playerName1.setFillColor(color);

	playerName2.setFont(this->font);
	playerName2.setCharacterSize(40);
	playerName2.setStyle(1 << 2);
	playerName2.setFillColor(color);

	soldiers1.setFont(this->font);
	soldiers1.setCharacterSize(40);
	soldiers1.setFillColor(color);

	soldiers2.setFont(this->font);
	soldiers2.setCharacterSize(40);
	soldiers2.setFillColor(color);


	soldierLost1.setFont(this->font);
	soldierLost1.setCharacterSize(40);
	soldierLost1.setFillColor(color);

	soldierLost2.setFont(this->font);
	soldierLost2.setCharacterSize(40);
	soldierLost2.setFillColor(color);

}

void BattleNotifier::setPositionCenter(int x, int y) {
	this->setPosition(x - this->getGlobalBounds().width / 2, y - this->getGlobalBounds().height / 2);
	centerCoordinates = Vector2f(x, y);
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

	battleText.setPosition(Vector2f(this->centerCoordinates.x - battleText.getGlobalBounds().width / 2, this->centerCoordinates.y - battleText.getGlobalBounds().height / 2 - this->getGlobalBounds().height - 25));
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
	removeTimer = this->ckk.getElapsedTime().asSeconds() + removeTimer;
	this->ckk.restart();

	if (removeTimer > 1) {
		opacityParam = opacityParam - 0.3*time;
		this->setColor(Color(this->getColor().r, this->getColor().g, this->getColor().b, opacityParam));
		backPageSprite.setColor(Color(backPageSprite.getColor().r, backPageSprite.getColor().g, backPageSprite.getColor().b, (int)opacityParam));

		battleText.setFillColor(Color(battleText.getFillColor().r, battleText.getFillColor().g, battleText.getFillColor().b, (int)opacityParam));
		playerName1.setFillColor(Color(playerName1.getFillColor().r, playerName1.getFillColor().g, playerName1.getFillColor().b, (int)opacityParam));
		playerName2.setFillColor(Color(playerName2.getFillColor().r, playerName2.getFillColor().g, playerName2.getFillColor().b, (int)opacityParam));
		soldiers1.setFillColor(Color(soldiers1.getFillColor().r, soldiers1.getFillColor().g, soldiers1.getFillColor().b, (int)opacityParam));
		soldiers2.setFillColor(Color(soldiers2.getFillColor().r, soldiers2.getFillColor().g, soldiers2.getFillColor().b, (int)opacityParam));
		spriteOfDeath.setColor(Color(spriteOfDeath.getColor().r, spriteOfDeath.getColor().g, spriteOfDeath.getColor().b, (int)opacityParam));

		soldierLost1.setFillColor(Color(soldierLost1.getFillColor().r, soldierLost1.getFillColor().g, soldierLost1.getFillColor().b, (int)opacityParam));
		soldierLost2.setFillColor(Color(soldierLost2.getFillColor().r, soldierLost2.getFillColor().g, soldierLost2.getFillColor().b, (int)opacityParam));
	}

	window.draw(backPageSprite);
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
	backPageSprite.setColor(Color(backPageSprite.getColor().r, backPageSprite.getColor().g, backPageSprite.getColor().b, (int)opacityParam));

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