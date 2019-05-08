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
	leftSoldier = 10;
	name = "";
	id = -1;
	battlesLost = 0;
	battlesWon = 0;
	money = 200;
}

Player::Player(string _name, int _id)
{
	leftSoldier = 10;
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
	if (! (this->hasProvince(worldMap, _province))) {
		if (_province->getOwner() != NULL) {
			_province->getOwner()->loseProvince(worldMap, _province);
		}
		provinces.push_back(worldMap->findIndex(_province));
		_province->setOwner(this);
	}
	
}

void Player::loseProvince(WorldMap* worldMap, Province* _province) {
	vector <int> :: iterator index = find(provinces.begin(), provinces.end(), worldMap->findIndex(_province));
	if (index != provinces.end())
		provinces.erase(index);
}

bool Player::placeSoldier(WorldMap * worldMap, int amount, Province * _province)
{
	if (leftSoldier < amount)
		return false;
	if (!hasProvince(worldMap, _province))
		return false;
	_province->setNumberOfSoldiers(_province->getNumberOfSoldiers() + amount);
	worldMap->showProvinceStatus(_province);
	leftSoldier--;
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
	if(p->getOwner() == NULL)
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
	while (!file.eof()) {
		file >> cityName >> color;
		createProvince(cityName, to_string(color));
		colorLookUpTable.insert(pair<int, string> (color, cityName));
		cout << cityName << " " << color << " loaded. " << endl;
	}
	file.close();
	cout << "efsfsef" << endl;
	createNeighbor("ankara", "konya");
	createNeighbor("ankara", "eskisehir");
	createNeighbor("konya", "antalya");
	createNeighbor("istanbul", "edirne");
	createNeighbor("istanbul", "kocaeli");
	createNeighbor("eskisehir", "kocaeli");
	createNeighbor("eskisehir", "istanbul");
	
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
			cout << endl << "============		"<< players[1]->getName() <<" TURN		===============  " << endl;
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


void GameManager::sendAllProvincesFromHost(NetworkManager ** NM) {
	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
		Province * pro = worldMap->getProvinceByID(i);
		(*NM)->sendDataFromHost(this, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(), pro->getCastle()->getLevel());
	}
	showWorldStatus();
}

void GameManager::sendAllProvincesClientToHost (string _connectionType, NetworkManager ** NM) {
	for (int i = 0; i < worldMap->getNumberOfProvinces(); i++) {
		Province * pro = worldMap->getProvinceByID(i);
		(*NM)->sendDataFromClientToHost(this, _connectionType, pro->getOwner()->getId(), i, pro->getNumberOfSoldiers(),pro->getCastle()->getLevel());
	}
		
}

void NetworkManager::createNetwork(GameManager ** const GM) {
	ip = IpAddress::getLocalAddress();
	string text = " ";
	int playerCount = 0;
	Packet packet;

	cout << "(h) for server, (c) for client: ";
	cin >> connectionType;

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
		string playersName = "";
		cout << "Enter your name(HOST): ";
		cin >> name;
		playersName += name + ",";
		(*GM)->addPlayer(name);
		do {
			IpAddress rIP;
			unsigned short port;
			;
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
				if (port == 2002) {
					cout << display << " has joined the room." << endl;
					(*GM)->addPlayer(display);
					playersName += name + ",";
				}
				//if (port == 2003)
				//	cout << "Client3 has joined the room." << endl;
				//if (port == 2004)
				//	cout << "Client4 has joined the room." << endl;
				//if (port == 2005)
				//	cout << "Client5 has joined the room." << endl;
			}
			cout << "Player in the game (except host): " << playerCount << endl;
		} while (playerCount != 2);

		string startgame = "no";
		do {
			cout << "enter 's' to create the game: ";
			cin >> startgame;
		} while (startgame != "s");

		String sendPlayersName;
		sendPlayersName = "" + playersName;
		Packet packet;
		packet << sendPlayersName;
		//string display = sendPlayersName;
		//cout << display << endl;
		map<unsigned short, IpAddress> ::iterator tempIterator;
		for (tempIterator = computerID.begin(); tempIterator != computerID.end(); tempIterator++)
			if (socket.send(packet, tempIterator->second, tempIterator->first) == Socket::Done) {}


	}

	else if (connectionType == "c1" || connectionType == "c2") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		string name;
		cout << "Enter your name: ";
		cin >> name;
		String playerName = name;
		packet << playerName;
		sIP = "139.179.210.187";
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

			for (int i = 0; i < 3; i++) {
				(*GM)->addPlayer(players[i]);
			}
		}
	}
}

WindowManager::WindowManager() {

}

WindowManager::WindowManager(GameManager* GM)
{
	this->GM = GM;
	zoom = 1.0;

	phase = INITAL_PHASE;

	screenWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	screenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;

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

	provinceNameTxt.setFont(font);
	provinceNameTxt.setCharacterSize(50);
	provinceNameTxt.setFillColor(sf::Color(0, 0, 0));
	provinceNameTxt.setPosition(600, bottomUpperMargin);

	mapSprite.setPosition(0, 0);
	mainView.setCenter(mapSprite.getLocalBounds().width / 2, mapSprite.getLocalBounds().height / 2);

	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));
	buttons.push_back(new Button(font));

	buttons[0]->setPosition(400, bottomUpperMargin);
	buttons[0]->setSize(100, 40);
	buttons[0]->setText("Next Phase");
	buttons[0]->setTextSize(20);
	buttons[0]->setTextColor(sf::Color::White);
	buttons[0]->setFillColor(sf::Color::Blue);

	buttons[1]->setSize(100, 100);
	buttons[1]->setText("Attack");
	buttons[1]->setPosition(400, screenHeight-buttons[1]->getSize().y);
	buttons[1]->setTextSize(20);
	buttons[1]->setTextColor(sf::Color::White);
	buttons[1]->setFillColor(sf::Color::Red);

	images.push_back(new MyImage("soldier.png"));
	images[0]->setInitialPosition(lowerPanel.getPosition().x + 30, lowerPanel.getPosition().y + lowerPanel.getSize().y / 2 - images[0]->getTextureRect().height / 2);
	images[0]->setPosition(images[0]->getInitialPosition());
}

WindowManager::~WindowManager() {

}

void WindowManager::createWindow() {

	window.setKeyRepeatEnabled(false);
	window.create(sf::VideoMode(screenWidth, screenHeight), "Risk");


	int counter = 0;

	while (window.isOpen()) {
		sf::Event event;
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
				else if (zoom < 1 && event.mouseWheelScroll.delta < 0) {
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
						provinceClicked(index);
					}
				}
			}
		}
		float speed = 3;
		if (counter > 10) {
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

		window.clear(sf::Color::Black);
		window.draw(mapSprite);

		window.setView(window.getDefaultView());
		window.draw(lowerPanel);
		dragObject(window, event, 0);
		window.draw(*images[0]);

		window.draw(provinceNameTxt);

		buttons[0]->draw(window);
		buttons[1]->draw(window);


		window.display();
	}

}

string WindowManager::getProvinceByColor(int color) {

}

int WindowManager::getPixelColor(int x, int y) {
	cout << "Mouse : " << x << ", " << y << endl;
	return (int)mapImg.getPixel(x, y).toInteger();
}

string WindowManager::getProvinceName(sf::RenderWindow & window, sf::Mouse & m) {
	sf::Vector2i PixelPos = m.getPosition(window);
	sf::Vector2f MousePos = window.mapPixelToCoords(PixelPos, mainView);
	if (MousePos.x <= mapImg.getSize().x && MousePos.y <= mapImg.getSize().y) {
		int colorInInt = (int)mapImg.getPixel(MousePos.x, MousePos.y).toInteger();
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
			buttonClicked(id);
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

void WindowManager::buttonClicked(int id) {
	string dummy;
	Player* player = GM->getPlayerByID(GM->currentPlayer, dummy);
	if (id == 0) {
		if (phase == INITAL_PHASE) {
			player->setLeftSoldier(player->getNumberOfProvinces() / 3);
			phase = PLACEMENT_PHASE;
		}
		else if (phase == PLACEMENT_PHASE) {
			phase = ATTACKING_PHASE;
		}
		cout << "Phase: " << phase << endl;
	}
	if (phase == ATTACKING_PHASE && id == ATTACK_BUTTON) {
		if (isProvinceClicked == 2) {
			if (GM->attack(player, second->getOwner(), first, second, 1)) {
				phase = POST_ATTACK; //change phase 
			}
		}
	}
	if (phase == POST_ATTACK && id == ATTACK_BUTTON && second->getNumberOfSoldiers() == 0) {
		//player->captureProvince(GM->getWorldMap(),second);
		buttons[ATTACK_BUTTON]->setText("Place");
		provinceNameTxt.setString("Enter the number of soldiers you want to place on this city:");
		GM->fortify(player, first, second, 1);
		phase = FORTIFY_PHASE;
	}
	if (phase == ATTACKING_PHASE && id == ATTACK_BUTTON) {
	}
}

void WindowManager::imageClicked(int id) {
	if (id == 0) {
		cout << "Image clicked" << endl;
	}
}

void WindowManager::provinceClicked(int id) {
	if (phase == ATTACKING_PHASE) {
		if (isProvinceClicked == 0) {
			first = GM->getWorldMap()->getProvinceByID(id);
			second = NULL;
			isProvinceClicked = 1;
			provinceNameTxt.setString(first->getName());
		}
		else if (isProvinceClicked == 1) {
			second = GM->getWorldMap()->getProvinceByID(id);
			isProvinceClicked = 2;
			provinceNameTxt.setString(first->getName() + " attacks to " +second->getName());
		}
		else if (isProvinceClicked == 2) {
			second = NULL;
			first = GM->getWorldMap()->getProvinceByID(id);
			isProvinceClicked = 1;
			provinceNameTxt.setString(first->getName());
		}
	}
}

void WindowManager::dragObject(sf::RenderWindow & window, sf::Event & event, int id) {
	if (id == 0 && !(phase == PLACEMENT_PHASE || phase == INITAL_PHASE))
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
	else if (event.type == event.MouseButtonReleased&& event.key.code == mouse.Left&& images[id]->inMove) {
		sf::Vector2i PixelPos = mouse.getPosition(window);
		sf::Vector2f MousePos = window.mapPixelToCoords(PixelPos, mainView);
		images[id]->inMove = false;
		images[id]->setPosition(images[id]->getInitialPosition());
		images[id]->setScale(sf::Vector2f(1, 1));
		if (sf::IntRect(0, 0, mapImg.getSize().x, mapImg.getSize().y).contains(sf::Vector2i(MousePos.x, MousePos.y)) && mouse.getPosition(window).y < 500)
		{
			if (id == 0) {
				string provinceName = getProvinceName(window, mouse);
				cout << provinceName << endl;
				if (phase == PLACEMENT_PHASE) {
					if (GM->placeSoldier(GM->currentPlayer, provinceName, 1)) {
						int dummy; Province* province;
						GM->getWorldMap()->getProvinceByName(provinceName, dummy, province);
						provinceNameTxt.setString(provinceName + "\nSoldier number: " + to_string(province->getNumberOfSoldiers()));
					}
					else {
						provinceNameTxt.setString("Senin deðil amk");
					}			
				}
			}
		}
	}
}

Button::Button() {
	
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
	text.setPosition(x, y);
}

void Button::setSize(int width, int height) {
	sf::RectangleShape::setSize(sf::Vector2f(width, height));
}

void Button::setTextColor(sf::Color color) {
	text.setFillColor(color);
}

void Button::setTextSize(int size) {
	text.setCharacterSize(size);
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

void NetworkManager::sendDataFromHost ( GameManager * const GM, int _playerID, int _cityID, int _count, int _castleLevel) {

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
	string connectionType ;
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
		} while (startGame != "start" );
		
	}

	// clients are the other player who joined the room
	else if (connectionType == "c1") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		sIp = "139.179.210.187";
		IpAddress sendIP(sIp);
		if (socket.send(packet, sendIP, 2000) == Socket::Done)
			cout << "Client1 has joined the room." << endl;
		cout << sIp << endl;
	}

	else if (connectionType == "c2") {
		//cout << "Enter server ip: ";
		//cin >> sIp;
		sIp = "139.179.210.187";
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

vector<string> NetworkManager ::split( string strToSplit, char delimeter){
	stringstream ss (strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

sf::Vector2f MyImage::getInitialPosition() {
	return initialPosition;
}