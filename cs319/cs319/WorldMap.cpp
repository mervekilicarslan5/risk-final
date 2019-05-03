//#include "WorldMap.h"
//
//WorldMap::WorldMap()
//{
//	/*numberOfProvinces = 0;*/
//}
//
//WorldMap::~WorldMap()
//{
//	//provinceList.clear();
//	//map.clear(); // Test it 
//}
//
////void WorldMap::addProvince(Province * _province)
////{
////	provinceList.push_back(_province);
////	vector<int> empty;
////	map.push_back(empty);
////	numberOfProvinces++;
////}
////
////void WorldMap::addEdge(int first, int second)
////{
////	map[first].push_back(second);
////	map[second].push_back(first);
////}
////
////int WorldMap::findIndex(Province * _province)
////{	
////	int count = 0;
////	for (auto it = provinceList.begin(); it != provinceList.end(); it++) {
////		if (*it == _province) {
////			return count;
////		}
////		count++;
////	}
////	return -1;
////}
////
////void WorldMap::addNeighbor(Province * first, Province * second)
////{
////	addEdge(findIndex(first), findIndex(second));
////}
////
////void WorldMap::printNeighbors(Province * _province)
////{
////	int index = findIndex(_province);
////	for (auto it = map[index].begin(); it != map[index].end(); it++) {
////		cout << provinceList[(*it)]->getName() << endl;
////	}
////}
////
////bool WorldMap::isNeighbor(Province * first, Province * second)
////{
////	int index = findIndex(first);
////	for (auto it = map[index].begin(); it != map[index].end(); it++) {
////		if (provinceList[*it] == second)
////			return true;
////	}
////	return false;
////}
////
////bool WorldMap::hasPath(Player * player, Province * from, Province * to)
////{
////	bool mark[100];
////	for (int i = 0; i < 100; i++) {
////		mark[i] = false;
////	}
////	int first = findIndex(from);
////	int second = findIndex(to);
////	queue<int> q;
////	q.push(first);
////	mark[first] = true;
////
////	while (!q.empty()) {
////		int cur = q.front();
////		q.pop();
////
////		if (cur == second)
////			return true;
////
////		for (auto it = map[cur].begin(); it != map[cur].end(); it++) {
////			if (mark[*it] || provinceList[*it]->getOwner() != player)
////				continue;
////			q.push(*it);
////			mark[*it] = true;
////		}
////
////	}
////
////	return false;
////
////}
//
