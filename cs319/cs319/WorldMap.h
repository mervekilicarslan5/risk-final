#pragma once

#include "Province.h"
using namespace std;
#include <vector>
class WorldMap
{
public:
	WorldMap();
	~WorldMap();
	void addProvince(Province* _province);
	void addEdge(int first, int second);
	int findIndex(Province* _province);
	void addNeighbor(Province* first, Province* second);
private:
	int numberOfProvinces;
	vector<Province*> provinceList;
	vector< vector<int> > map;
};

