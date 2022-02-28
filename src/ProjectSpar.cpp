// ProjectSpar.cpp : Defines the entry point for the application.
//

#include "ProjectSpar.h"

int main()
{
	auto gamewin = SparEngine::Window(640, 480, "My brand new app!");
	SparEngine::Renderer renderer{ &gamewin, true };
	auto render_object = renderer.createRenderObject();
	gamewin.loop();

	return 0;
}
