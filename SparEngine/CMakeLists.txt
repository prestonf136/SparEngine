set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory("thirdparty/SDL")

find_package(Vulkan REQUIRED FATAL_ERROR)

add_library(
	SparEngine_Window
	"Window/Window.h"
	"Window/Window.cpp"
)

target_include_directories (SparEngine_Window PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(SparEngine_Window PUBLIC SDL2main SDL2-static ${VULKAN_LIB_LIST})

add_library(
	SparEngine
	"SparEngine.h"
	"SparEngine.cpp"
)

target_link_libraries(SparEngine PUBLIC SparEngine_Window)