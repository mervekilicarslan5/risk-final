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
class MiniMap;
class ArmyBage;
class TopBar;



class ArmyBage : public sf::Sprite {
public:
	Image img;
	Texture tex;
	int sizeOfArmy;
	Text text;
	Color color;
	string nameOfProvince;
	Vector2f centerCoordinates;

	ArmyBage();
	ArmyBage(Image img, int x, int y, string nameOfProvince, Font &font);
	void setSizeOfArmy(int size);
	void setBageColor(sf::Color color);
	void draw(sf::RenderWindow & window);

};


class LineBetweenProvinces : public sf::Sprite {
public:
	sf::Image img;
	sf::Texture tex;
	float degree;
	double step;
	int lenght;
	bool visible = false;
	Vector2f first, second;

	LineBetweenProvinces();
	LineBetweenProvinces(Image &img);
	//void setCoordinatesOfProvince1(Vector2f coord);
	//void setCoordinatesOfProvince2(Vector2f coord);
	void setCoordinates(Vector2f first, Vector2f second);
	void draw(RenderWindow & window,float time);
	void setVisible(bool flag);

};

class CastleBage : public sf::Sprite {
public:
	Image img;
	Texture tex;
	bool available;
	Color color;
	string nameOfProvince;
	Vector2f centerCoordinates;

	CastleBage();
	CastleBage(Image img, int x, int y, string nameOfProvince);
	void setBageColor(sf::Color color);
	void draw(sf::RenderWindow & window);

};






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

class MiniMap : public sf::View {
public:
	sf::Texture mapTex;
	sf::Sprite mapSprite;////////////////lol
	sf::RectangleShape miniMapRectangle;
	MiniMap(sf::Texture mapTexture);
	MiniMap();
	void update(sf::View & mainView);
	void draw(sf::RenderWindow & window);


};

class TopBar : public sf::Sprite {
public:
	Image img, coinImg, actionImg, turnImg,playerImg;
	Texture panelTexture, coinTexture, actionTexture, turnTexture,playerTexture;

	Sprite coinSprite, actionSprite, turnSprite,playerSprite;
	Text coinText, actionText, turnText, playerText;
	RectangleShape coinField, actionField, turnField, playerField;

	Color color;
	string nameOfProvince;
	Vector2f centerCoordinates;

	TopBar();
	TopBar(Image img, Image coinImg, Image actionImage, Image turnImage,Image playerImage,Font &font);
	void update(int moneyAmount, int actionAmount, int currentTurn, string player);
	void draw(RenderWindow & window);
	//void setSizeOfArmy(int size);
	//void setBageColor(sf::Color color);
	//void draw(sf::RenderWindow & window);

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
	int ownerCount();
	vector<Province*> getNeighbors(Province* _province);
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
	void sendAllProvincesFromHostString(NetworkManager ** NM);
	void sendAllProvincesClientToHostString(NetworkManager ** NM);
	void sendAllProvincesClientToHost(string _connectionType, NetworkManager ** NM);
	int getPlayerTurn(string _name);
	vector<string> split(std::string strToSplit, char delimeter);
	void destroyNearSoldier(Province* province);
	void castleAttacks(Player* player);
	vector<Player*> getPlayers();

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
	WindowManager * WM;
	NetworkManager(WindowManager * WM);
	void createNetwork(GameManager ** const GM, string _connectionType, string name);
	string sendStringFromHost(string _sendText);
	void sendDataFromHost(GameManager * const GM, int playerID, int _cityID, int count, int _castleLevel);
	void sendDataFromClientToHost(GameManager * const GM, string _connectionType, int _playerID, int _cityID, int _count, int _castleLevel);
	void buildNewtwork();
	vector<string> split(std::string strToSplit, char delimeter);
	void startGame();
	string connectionType;

private:
	IpAddress ip;
	IpAddress sIP;
	map<unsigned short, IpAddress> computerID;
	UdpSocket socket;
	vector<string> players;
	string playersName;
	Packet packet;
};

class WindowManager {
public:

	GameManager * GM;
	NetworkManager * NM;
	double zoom;

	int leftMargin, rightMargin, topMargin, bottomLowerMargin, bottomUpperMargin;
	sf::Image mapImg, hoverImg, roundedSquare,lineImg, coinImg,actionImg,timerImg,topPanelImg, crownImg,castleImg;
	sf::Texture mapTex;
	sf::View mainView;
	sf::Sprite mapSprite;
	sf::RenderWindow window;
	sf::Mouse mouse;
	sf::RectangleShape lowerPanel;
	sf::Text provinceNameTxt, infoText;
	sf::Font font;
	TopBar *topPanel;
	MiniMap miniMap;
	LineBetweenProvinces *lineForProvinces;
	vector<Button*> buttons;
	vector<Text*> playerStatus;
	vector<MyImage*> images;
	vector<MyImage*> castles;
	vector<string> wheelStr;
	vector<ArmyBage*> listOfArmyBage;
	vector<CastleBage*> listOfCastleBage;
	int phase;
	int page = 0;
	int soldierAmount = 1;
	bool turnWheel = false;
	float time; //time to calculate elapsed time
	Clock clock;

	const int MENU_SCREEN = 0;
	const int GAME_SCREEN = 1;
	const int COMPUTER_GAME_SCREEN = 2;

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
	const int MARKET_PHASE = 5;
	const int END_TURN = 6;

	const int host = 5;
	const int c1 = 6;
	const int c2 = 7;
	const int start = 8;
	const int TURN_WHEEL_BUTTON = 9;

	int playerCount = 0;
	int countForWheel = 0;
	bool wheel = false;
	bool takeCastle = false;

	int isProvinceClicked = 0;
	Province* first;
	Province* second;
	Province* currentProvince;

	string userName;
	int userTurn;
	int turn = 0;

	bool _randomPlacement = true;
	bool castle = false;
	float rotateAmount = 22.5;
	float rotateStep = 2.5;
	int rotateRandom = 250;


	int counter = 0;

	WindowManager();
	~WindowManager();
	void createWindow();
	void menuScreen(RenderWindow & window, Event & e);
	void multGameLan(RenderWindow & window, Event & event);
	void multGameComp(RenderWindow & window, Event & event);
	string getProvinceByColor(int color);
	int getPixelColor(int x, int y);
	bool insideTheWindow(Vector2i pos);
	string getProvinceName(sf::RenderWindow & window, sf::Mouse & m);
	void provinceClicked(int id);
	void checkClickEvents(sf::Event & e);
	void buttonClicked(int id, sf::Event &event, sf::RenderWindow & window);
	void imageClicked(int id);
	void handleWheel();
	void dragObject(sf::RenderWindow & window, sf::Event & event, int id);
	void displayProvinceInfo(Province * province);
	void displayPlayerStatus();
	void drawAllArmies(RenderWindow & window, Event & e);
	void drawAllCastles(RenderWindow & window, Event & e);
};

class Button : public sf::RectangleShape {
public:
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
public:
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
