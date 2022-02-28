#include "Window.h"

SparEngine::Window::Window(int32_t _iWidth, int32_t _iHeight, std::string sWindowName) : m_iWidth{ _iWidth }, m_iHeight{ _iHeight }
{
	m_sWindowName = sWindowName;
	SDL_Init(SDL_INIT_VIDEO);
	m_pWindow = SDL_CreateWindow(sWindowName.c_str(), 
								 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								 m_iWidth, m_iHeight,
								 SDL_WINDOW_VULKAN);

	assert(m_pWindow != nullptr && "Failed to create SDL window!");
}

void SparEngine::Window::loop()
{
	while (true)
	{
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break;
		}
	}
}

void SparEngine::Window::getSurface(VkInstance pInstance, VkSurfaceKHR* pSurface)
{
	assert(SDL_Vulkan_CreateSurface(m_pWindow, pInstance, pSurface) && "Failed cto create VKSurface");
}

std::string SparEngine::Window::getName()
{
	return m_sWindowName;
}

std::vector<const char*> SparEngine::Window::getRequiredExtensions()
{
	uint32_t count;
	assert(SDL_Vulkan_GetInstanceExtensions(m_pWindow, &count, nullptr) && "failed to query extensions");

	std::vector<const char*> extensions;
	extensions.resize(count);

	assert(SDL_Vulkan_GetInstanceExtensions(m_pWindow, &count, extensions.data()));
	return extensions;
}

SparEngine::Window::~Window()
{
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}
