// ProjectSpar.cpp : Defines the entry point for the application.
//

#include "ProjectSpar.h"

int main()
{
	auto gamewin = SparEngine::Window(640, 480, "My brand new app!");
	auto renderer = SparEngine::Renderer(gamewin);
	gamewin.loop();

	return 0;
}
