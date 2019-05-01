#pragma once

using namespace std;
#include <string>
#include <iostream>
#include "Castle.h"
#include "Player.h"

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

