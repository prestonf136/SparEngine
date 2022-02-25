#pragma once

#include <SDL.h>
#undef main

#include <SDL_vulkan.h>
#include <string>
#include <cassert>

namespace SparEngine {
	class Window {
	private:
		SDL_Window* m_pWindow;
		int32_t m_iWidth, m_iHeight;
	public:
		Window(int32_t _iWidth, int32_t _iHeight, std::string sWindowName);
		void getSurface(VkInstance pInstance, VkSurfaceKHR *pSurface);
		~Window();
	};
}