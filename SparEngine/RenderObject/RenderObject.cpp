#include "RenderObject.h"
#include <iostream>
#include <chrono>
#include <functional>

SparEngine::RenderObject::RenderObject(
	vk::UniqueInstance* _Instance,
	vk::PhysicalDevice* _PhysicalDevice,
	vk::UniqueDevice* _Device,
	vk::SurfaceKHR* _Surface,
	vk::UniqueCommandPool* _CommandPool) : m_Instance{ _Instance }, m_PhysicalDevice{ _PhysicalDevice }, m_Device{ _Device }, m_Surface{ _Surface }, m_CommandPool{ _CommandPool }
{
	m_pRenderThread = std::make_unique<std::thread>(&RenderObject::Create, this);
}

void SparEngine::RenderObject::Create()
{
	m_CommandBuffer = m_Device->get()
		.allocateCommandBuffers(vk::CommandBufferAllocateInfo(m_CommandPool->get(), vk::CommandBufferLevel::ePrimary, 1)).front();
}

SparEngine::RenderObject::~RenderObject()
{
	m_pRenderThread->join();
}
