
#include "Top.h"


#include <iostream>
#include <string>

using namespace std;



int main() {

	GameManager* GM = new GameManager();
	NetworkManager* NM = new NetworkManager();
	WindowManager* WM = new WindowManager(GM);
  
  //GM->loadProvinces();
  
	NM->createNetwork(&GM);

	GM->startGame(&NM);

  WM->createWindow();


	//system("PAUSE");
	return 0;
}