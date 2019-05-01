#pragma once

using namespace std;
#include <vector>

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
	void setName(string _name);
	void setId(int _id);

private:
	string name;
	int money;
	int id;
	int battlesWon;
	int battlesLost;
	vector<int> provinces;
	vector<int> bonusCards;
};

