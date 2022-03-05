#include <vulkan/vulkan.h>
#include "../thirdparty/VMA/vk_mem_alloc.h"

namespace SparEngine {
	class Buffer {
	private:
		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;
		VmaAllocator *m_Allocator;
	public:
		Buffer(VmaAllocator* _allocator, VkBufferCreateInfo _bufferInfo);
		VkBuffer& get();
		~Buffer();
	};
}