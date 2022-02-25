#include "Renderer.h"

SparEngine::Renderer::Renderer(SparEngine::Window& _rWindow) : m_rWindow { _rWindow }
{
	auto req_ext = m_rWindow.getRequiredExtensions();

	auto inst_builder = vkb::InstanceBuilder()
				.enable_validation_layers()
				.use_default_debug_messenger()
				.require_api_version(VK_API_VERSION_1_0);

	for (auto ext : req_ext)
		inst_builder.enable_extension(ext);
	
	auto inst_builder_ret = inst_builder.build();

	assert(inst_builder_ret && "failed to create vulkan instance");

	m_Instance = inst_builder_ret.value();

	m_rWindow.getSurface(m_Instance.instance, &m_Surface);

	auto phys_dev_builder = vkb::PhysicalDeviceSelector(m_Instance)
					.set_surface(m_Surface)
					.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);

	auto phys_dev_builder_ret = phys_dev_builder.select();
	
	assert(phys_dev_builder_ret && "failed to select vulkan physical device");

	m_PhysicalDevice = phys_dev_builder_ret.value();

	auto device_builder = vkb::DeviceBuilder(m_PhysicalDevice);

	auto device_builder_ret = device_builder.build();

	assert(device_builder_ret && "failed to create vulkan physical device");

	m_Device = device_builder_ret.value();

	auto swapchain_builder = vkb::SwapchainBuilder(m_Device);
	auto swap_builder_ret = swapchain_builder.build();
	
	assert(swap_builder_ret && "failed to create swapchain");

	m_Swapchain = swap_builder_ret.value();
}

SparEngine::Renderer::~Renderer()
{
	vkb::destroy_swapchain(m_Swapchain);
	vkb::destroy_device(m_Device);
	vkb::destroy_surface(m_Instance, m_Surface);
	vkb::destroy_instance(m_Instance);
}
