#pragma once

#include "../Window/Window.h"
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

namespace SparEngine {
	class Renderer {
	private:
		SparEngine::Window& m_rWindow;
		vkb::Instance m_Instance;
		vkb::PhysicalDevice m_PhysicalDevice;
		vkb::Device m_Device;
		vkb::Swapchain m_Swapchain;
		
		VkSurfaceKHR m_Surface;
	public:
		Renderer(SparEngine::Window& _rWindow);
		~Renderer();
	};
}