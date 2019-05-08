
#include "Top.h"


#include <iostream>
#include <string>
using namespace std;

int main() {


	GameManager* GM = new GameManager();

	for (auto it = GM->colorLookUpTable.begin(); it != GM->colorLookUpTable.end(); it++) {
		cout << (*it).second << endl;
	}

	WindowManager* WM = new WindowManager(GM);

	GM->loadProvinces();

	GM->addPlayer("rumeysa");
	GM->addPlayer("serdar");
	GM->addPlayer("geyOsman");

	WM->createWindow();

	//GM->startGame();

	//system("PAUSE");
	return 0;
}