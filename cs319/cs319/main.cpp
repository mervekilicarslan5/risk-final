
#include "Top.h"


#include <iostream>
#include <string>

using namespace std;



int main() {
	WindowManager* WM = new WindowManager();

	WM->GM->loadProvinces();

	WM->createWindow();


	return 0;
}
