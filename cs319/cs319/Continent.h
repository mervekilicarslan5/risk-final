#pragma once


#include <vector>
#include "Player.h"

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

