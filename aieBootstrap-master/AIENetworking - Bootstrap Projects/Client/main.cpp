#include "GameApp.h"

//extern "C"
//{
	int main() {

		auto app = new GameApp();
		app->run("AIE", 1280, 720, false);
		delete app;

		return 0;
	}
//}