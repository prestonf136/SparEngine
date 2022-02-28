#pragma once

#include <vulkan/vulkan.hpp>
#include <thread>
#include <memory>

namespace SparEngine {
	class RenderObject {
	private:
		vk::UniqueInstance *m_Instance;
		vk::PhysicalDevice *m_PhysicalDevice;
		vk::UniqueDevice *m_Device;
		vk::UniqueCommandPool *m_CommandPool;
		vk::SurfaceKHR* m_Surface;
		vk::CommandBuffer m_CommandBuffer;
		
		std::unique_ptr<std::thread> m_pRenderThread;
	public:
		RenderObject(vk::UniqueInstance* _Instance,
					 vk::PhysicalDevice* _PhysicalDevice,
					 vk::UniqueDevice* _Device,
					 vk::SurfaceKHR* _Surface,
					 vk::UniqueCommandPool* _CommandPool);
		void Create();
		~RenderObject();
	};
}