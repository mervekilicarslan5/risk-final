
#include "Top.h"


#include <iostream>
#include <string>

using namespace std;



int main() {
	WindowManager* WM = new WindowManager();
	//GameManager* GM = new GameManager();
	//NetworkManager* NM = new NetworkManager(WM);
	//WM = new WindowManager(GM,NM);

	//GM->addPlayer("ahmet");
	//GM->addPlayer("serdar");
  
    WM->GM->loadProvinces();

	//GM->randomPlacement();
  
	//NM->createNetwork(&GM);

	//GM->startGame(&NM);

    WM->createWindow();

	//system("PAUSE");
	return 0;
}