// ProjectSpar.cpp : Defines the entry point for the application.
//

#include "ProjectSpar.h"
#include <filesystem>

int main()
{
	auto win = SparEngine::Window(640, 480, "Hello Spar Engine!");
	auto renderer = SparEngine::Renderer(win, true);

	win.loop();

	return 0;
}
