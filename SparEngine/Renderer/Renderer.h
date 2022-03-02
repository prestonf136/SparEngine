#pragma once

#include "../Window/Window.h"
#include "../RenderThread/RenderThread.h"
#include <vulkan/vulkan.hpp>

namespace SparEngine {
	class Renderer {
	private:
		vk::UniqueInstance m_Instance;
		vk::UniqueDebugUtilsMessengerEXT m_DebugUtilsMessenger;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::UniqueDevice m_Device;
		vk::UniqueCommandPool m_CommandPool;
		vk::UniqueSurfaceKHR m_Surface;
		vk::Queue m_GraphicsQueue;
		vk::Queue m_PresentQueue;

		RenderThreadData m_Data;
	public:
		Renderer(SparEngine::Window* _pWindow, bool _bDebug);
		std::shared_ptr<RenderThread> createRenderObject(int32_t _iWidth, int32_t _iHeight);
		~Renderer();
	};
}