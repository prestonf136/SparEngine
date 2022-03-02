#pragma once

#include <vulkan/vulkan.hpp>
#include <thread>
#include <memory>

namespace SparEngine {
	struct RenderThreadData {
		vk::UniqueInstance* m_Instance;
		vk::PhysicalDevice* m_PhysicalDevice;
		vk::UniqueDevice* m_Device;
		vk::UniqueCommandPool* m_CommandPool;
		vk::UniqueSurfaceKHR* m_Surface;
		vk::CommandBuffer m_CommandBuffer;
		vk::Queue* m_GraphicsQueue;
		vk::Queue* m_PresentQueue;
		uint32_t m_GraphicsQueuFamilyIndex;
		uint32_t m_PresentQueuFamilyIndex;
		int32_t m_iWidth, m_iHeight;

		vk::UniqueSwapchainKHR* m_SwapChain;
	};
	class RenderThread {
	private:
		RenderThreadData m_Data;
		std::unique_ptr<std::thread> m_pRenderThread;
	public:
		RenderThread(RenderThreadData m_Data, int32_t _iWidth, int32_t _iHeight);
		void CreateSwapchainAndAssociated();
		void Create();
		~RenderThread();
	};
}