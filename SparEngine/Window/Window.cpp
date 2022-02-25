#include "Window.h"
#include "Window.h"
#include "Window.h"

SparEngine::Window::Window(int32_t _iWidth, int32_t _iHeight, std::string sWindowName) : m_iWidth{ _iWidth }, m_iHeight{ _iHeight }
{
	SDL_Init(SDL_INIT_VIDEO);
	m_pWindow = SDL_CreateWindow(sWindowName.c_str(), 
								 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								 m_iWidth, m_iHeight,
								 SDL_WINDOW_VULKAN);

	assert(m_pWindow != nullptr && "Failed to create SDL window!");
}

void SparEngine::Window::getSurface(VkInstance pInstance, VkSurfaceKHR* pSurface)
{
	assert(SDL_Vulkan_CreateSurface(m_pWindow, pInstance, pSurface) && "Failed cto create VKSurface");
}

SparEngine::Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}
