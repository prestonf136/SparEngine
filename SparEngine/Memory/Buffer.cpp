#include "Buffer.h"

SparEngine::Buffer::Buffer(VmaAllocator* _allocator, VkBufferCreateInfo _bufferInfo)
{
	m_Allocator = _allocator;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

	vmaCreateBuffer(*m_Allocator, &_bufferInfo, &allocInfo, &m_Buffer, &m_Allocation, nullptr);
}

VkBuffer& SparEngine::Buffer::get()
{
	return m_Buffer;
}

SparEngine::Buffer::~Buffer()
{
	vmaDestroyBuffer(*m_Allocator, m_Buffer, m_Allocation);
}
