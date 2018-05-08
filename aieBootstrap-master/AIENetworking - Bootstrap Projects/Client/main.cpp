/**
	main.cpp
	Purpose: Calling the application on run, setting the window size
			 and deleting the app afterwards.

	@author Nathan Nette
*/
#include "Client.h"

/**
	Main calls the Client application and once the application is closed,
	it deletes the application to deallcate the memory.
*/
int main()
{
	// Create new Client instance;
	auto app = new Client();

	// Run the Client instance at 1280 by 720.
	app->run("AIE", 1280, 720, false);

	// Delete the application to deallocate the memory.
	delete app;

	// Int functions need to return a value.
	return 0;
}
