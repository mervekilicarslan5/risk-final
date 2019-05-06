
#include "Top.h"


#include <iostream>
#include <string>
using namespace std;

int main() {


	GameManager* GM = new GameManager();
	WindowManager* WM = new WindowManager();

	GM->addPlayer("rumeysa");
	GM->addPlayer("serdar");
	GM->addPlayer("geyOsman");

	WM->createWindow();

	//GM->startGame();

	//system("PAUSE");
	return 0;
}