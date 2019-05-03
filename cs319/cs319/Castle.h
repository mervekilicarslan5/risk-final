#pragma once

#ifndef CASTLE_H
#define CASTLE_H

class Castle
{
public:
	Castle();
	Castle(int _price);
	~Castle();
	int getPrice();
	void setPrice(int _price);
	int getLevel();
	void setLevel(int _level);
	void upgradeLevel();
private:
	int price;
	int level;
};

#endif