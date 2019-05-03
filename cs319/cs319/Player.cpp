#include "Player.h"

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

//void Player::captureProvince(WorldMap* worldMap, Province * _province)
//{
//
//	provinces.push_back(worldMap->findIndex(_province));
//	_province->setOwner(this);
//}
//
//bool Player::placeSoldier(WorldMap * worldMap, int amount, Province * _province)
//{
//	int index = worldMap->findIndex(_province);
//	if (!hasProvince(worldMap, _province))
//		return false;
//	_province->setNumberOfSoldiers(_province->getNumberOfSoldiers() + amount);
//	return true;
//}
//
//bool Player::hasProvince(WorldMap * worldMap, Province * _province)
//{
//	int index = worldMap->findIndex(_province);
//	if (provinces.end() == find(provinces.begin(), provinces.end(), index))
//		return false;
//	return true;
//}
//

