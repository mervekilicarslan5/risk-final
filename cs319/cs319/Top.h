#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <map>
#include <sstream>

using namespace std;
using namespace sf;

class Die;
class Castle;
class Province;
class Continent;
class Player;
class WorldMap;
class NetworkManager;
class WindowManager;
class Button;
class MyImage;


class Die
{
public:
	Die();
	Die(int _numberOfFaces);
	~Die();
	int roll();

private:
	int numberOfFaces;
};

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
	bool isBuilt();
	void build();
	void destroy();
private:
	int price;
	int level;
	bool built;
};

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
	void incBattlesWon();
	void incBattlesLost();
	void setName(string _name);
	void setId(int _id);
	void captureProvince(WorldMap* worldMap, Province* _province);
	void loseProvince(WorldMap* worldMap, Province* _province);
	bool placeSoldier(WorldMap* worldMap, int amount, Province* _province);
	bool hasProvince(WorldMap * worldMap, Province* _province);
	int buildCastle(Province* province);
	int getNumberOfProvinces();
	void setLeftSoldier(int n);
	int getLeftSoldier();
private:
	string name;
	int money;
	int id;
	int battlesWon;
	int battlesLost;
	vector<int> provinces;
	vector<int> bonusCards;
	int leftSoldier;
};

class WorldMap
{
public:
	WorldMap();
	~WorldMap();
	void addProvince(Province* _province);
	void addNeighbor(Province* first, Province* second);
	void printNeighbors(Province* _province);
	bool isNeighbor(Province* first, Province* second);
	bool hasPath(Player* player, Province* from, Province* to);
	int findIndex(Province* _province);
	void getProvinceByName(string name, int & index, Province* & ptr);
	void showWorldStatus();
	void showProvinceStatus(Province* p);
	int getNumberOfProvinces();
	Province* getProvinceByID(int id);
private:
	int numberOfProvinces;
	vector< Province* > provinceList;
	vector< vector<int> > map;
	void addEdge(int first, int second);
};

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

class GameManager
{
public:

	GameManager();
	~GameManager();

	WorldMap* getWorldMap();
	void createProvince(string name, string color);
	void createNeighbor(string first, string second);
	void addPlayer(string _name);
	Player* getPlayerByID(int id, string & name);
	Player* getPlayerByName(string name, int & id);
	bool buildCastle(Player* player, Province* province);
	bool buildCastle(int playerID, string provinceName);
	bool buildCastle(string playerName, string provinceName);
	bool placeSoldier(Player* player, string provinceName, int amount);
	bool placeSoldier(int playerID, string provinceName, int amount);
	bool placeSoldier(string playerName, string provinceName, int amount);
	bool fortify(Player* player, Province* from, Province * to, int amount);
	bool fortify(int playerID, string from, string to, int amount);
	bool fortify(string name, string from, string to, int amount);
	bool attack(Player* attacker, Player* defender, Province* from, Province * to, int amount);
	bool attack(string attacker, string defender, string from, string to, int amount);
	void showWorldStatus();
	void showProvinceStatus(string name);
	vector<int> rollDice(int attacker, int defender);
	void startPlacement();
	void startTurn(int id);
	void loadProvinces();
	void startGame(NetworkManager ** NM);
	void startPlacementPhase(int id);
	void startAttackingPhase(int id);
	void startMarket(int id);
	void startFortifyPhase(int id);
	void randomPlacement();
	void sendAllProvincesFromHost(NetworkManager ** NM);
	void sendAllProvincesClientToHost(string _connectionType, NetworkManager ** NM);

	map<int, string> colorLookUpTable;
	int currentPlayer;

private:
	vector<Player*> players;
	WorldMap* worldMap;
	Die* die;
	bool gameOn;
};


class NetworkManager {
public:
	void createNetwork(GameManager ** const GM, string _connectionType);
	void sendDataFromHost(GameManager * const GM, int playerID, int _cityID, int count, int _castleLevel);
	void sendDataFromClientToHost(GameManager * const GM, string _connectionType, int _playerID, int _cityID, int _count, int _castleLevel);
	void buildNewtwork();
	vector<string> split(std::string strToSplit, char delimeter);
	string connectionType;


private:
	IpAddress ip;
	IpAddress sIP;
	map<unsigned short, IpAddress> computerID;
	UdpSocket socket;
	vector<string> players;

	Packet packet;
};

class WindowManager {
public: 

	GameManager* GM;

	double zoom;
	int screenWidth;
	int screenHeight;
	int leftMargin, rightMargin, topMargin, bottomLowerMargin, bottomUpperMargin;
	sf::Image mapImg, hoverImg;
	sf::Texture mapTex;
	sf::View mainView;
	sf::Sprite mapSprite;
	sf::RenderWindow window;
	sf::Mouse mouse;
	sf::RectangleShape lowerPanel;
	sf::Text provinceNameTxt, infoText;
	sf::Font font;
	vector<Button*> buttons;
	vector<MyImage*> images;
	int phase;
	int soldierAmount = 1;

	const int NEXT_PHASE_BUTTON = 0;
	const int ATTACK_BUTTON = 1;
	const int DEC_BUTTON = 2;
	const int INC_BUTTON = 3;
	const int NUMBER_TEXT = 4;

	const int INITIAL_PHASE = 0;
	const int PLACEMENT_PHASE = 1;
	const int ATTACKING_PHASE = 2;
	const int POST_ATTACK = 3;
	const int FORTIFY_PHASE = 4;
	const int END_TURN = 5;

	int isProvinceClicked = 0;
	Province* first;
	Province* second;
	Province* currentProvince;
	
	WindowManager();
	WindowManager(GameManager* GM);
	~WindowManager();
	void createWindow();
	string getProvinceByColor(int color);
	int getPixelColor(int x, int y);
	string getProvinceName(sf::RenderWindow & window, sf::Mouse & m);
	void provinceClicked(int id);
	void checkClickEvents(sf::Event & e);
	void buttonClicked(int id);
	void imageClicked(int id);
	void dragObject(sf::RenderWindow & window, sf::Event & event, int id);
	void displayProvinceInfo(Province * province);
};

class Button : public sf::RectangleShape {
public :
	sf::Text text;
	int id;

	Button();
	Button(sf::Font & font);
	~Button();

	void setText(string text);
	void draw(sf::RenderWindow & window);
	void setPosition(float x, float y);
	void setTextColor(sf::Color color);
	void setTextSize(int size);
	void setSize(int width, int height);
};

class MyImage : public sf::Sprite {
public :
	sf::Image img;
	sf::Texture tex;
	bool inMove;
	sf::Vector2f initialPosition;
	
	MyImage();
	MyImage(string fileName);
	~MyImage();
	sf::Vector2f getSize();
	void setInitialPosition(float x, float y);
	sf::Vector2f getInitialPosition();
};