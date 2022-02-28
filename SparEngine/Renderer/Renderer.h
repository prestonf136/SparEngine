#pragma once

#include "../Window/Window.h"
#include "../RenderObject/RenderObject.h"
#include <vulkan/vulkan.hpp>

namespace SparEngine {
	class Renderer {
	private:
		vk::UniqueInstance m_Instance;
		vk::UniqueDebugUtilsMessengerEXT m_DebugUtilsMessenger;
		vk::PhysicalDevice m_PhysicalDevice;
		vk::UniqueDevice m_Device;
		vk::UniqueCommandPool m_CommandPool;
		vk::SurfaceKHR m_Surface;
	public:
		Renderer(SparEngine::Window* _pWindow, bool _bDebug);
		std::shared_ptr<RenderObject> createRenderObject();
		~Renderer();
	};
}