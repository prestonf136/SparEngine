add_library(
	SparEngine_Window
	"Window/Window.h"
	"Window/Window.cpp"
)

add_library(
	SparEngine
	"SparEngine.h"
)

target_link_libraries(SparEngine PUBLIC SparEngine_Window)
add_library(SparEngine::SparEngine ALIAS SparEngine)