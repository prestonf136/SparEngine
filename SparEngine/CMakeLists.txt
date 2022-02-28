set (CMAKE_CXX_STANDARD 11)

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory("thirdparty/SDL")

find_package(Vulkan REQUIRED FATAL_ERROR)

add_library(
	SparEngine_Window
	"Window/Window.h"
	"Window/Window.cpp"
)

add_library(
	SparEngine_Renderer
	"Renderer/Renderer.h"
	"Renderer/Renderer.cpp"
)

add_library(
	SparEngine_RenderObject
	"RenderObject/RenderObject.h"
	"RenderObject/RenderObject.cpp"
)

target_include_directories (SparEngine_Window PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(SparEngine_Window PUBLIC SDL2main SDL2-static Vulkan::Vulkan)

target_include_directories (SparEngine_RenderObject PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(SparEngine_RenderObject PUBLIC Vulkan::Vulkan)


target_include_directories (SparEngine_Renderer PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(SparEngine_Renderer PUBLIC SparEngine_Window Vulkan::Vulkan SparEngine_RenderObject)

add_library(
	SparEngine
	"SparEngine.h"
	"SparEngine.cpp"
)
add_library(SparEngine::SparEngine ALIAS SparEngine)

target_link_libraries(SparEngine PUBLIC SparEngine_Window SparEngine_Renderer)