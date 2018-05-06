#include "Game.h"

//extern "C"
//{
int main() {

	auto app = new Game();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}
//}