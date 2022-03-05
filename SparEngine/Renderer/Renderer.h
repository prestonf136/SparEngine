#pragma once

#include "../Window/Window.h"
#include "../RenderThread/RenderThread.h"
#include <vulkan/vulkan.h>

#define SPAR_PATCH_NUM VK_MAKE_API_VERSION(0, 0, 0, 1)

namespace SparEngine {
	class Renderer{
	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice;
		VkSurfaceKHR m_Surface;
		VkDevice m_Device;
		VkCommandPool m_CommandPool;

		bool m_Debug;
	public:
		Renderer(Window& _win, bool _debug);
		~Renderer();
	};
}