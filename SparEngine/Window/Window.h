#pragma once

#include <SDL.h>
#undef main

#include <SDL_vulkan.h>
#include <string>
#include <cassert>
#include <vector>

namespace SparEngine {
	class Window {
	private:
		SDL_Window* m_pWindow;
		int32_t m_iWidth, m_iHeight;
		std::string m_sWindowName;
	public:
		Window(int32_t _iWidth, int32_t _iHeight, std::string sWindowName);
		void loop();
		void getSurface(VkInstance pInstance, VkSurfaceKHR *pSurface);
		std::string getName();
		std::vector<const char*> getRequiredExtensions();
		~Window();
	};
}