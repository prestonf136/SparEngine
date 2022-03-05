#include "Renderer.h"
#include <iostream>

#define VK_CHECK(eval)													\
	{																	 \
		VkResult res = eval;											  \
		if(res != VK_SUCCESS)  {										   \
			std::cout << "Error Encounterd, Of Type: " << res << std::endl; \
			abort();														 \
		}																	  \
	} 

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
									  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
									  const VkAllocationCallbacks* pAllocator,
									  VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
								   VkDebugUtilsMessengerEXT debugMessenger,
								   const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

SparEngine::Renderer::Renderer(Window& _win, bool _debug) : m_Debug{ _debug } {
	VkApplicationInfo AppInfo;
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.apiVersion = VK_API_VERSION_1_0;
	AppInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
	AppInfo.pApplicationName = _win.getName().c_str();
	AppInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
	AppInfo.pEngineName = "Spar Engine";

	VkInstanceCreateInfo InstanceCreateInfo{};
	InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceCreateInfo.pApplicationInfo = &AppInfo;

	auto exts = _win.getRequiredExtensions();
	exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	InstanceCreateInfo.enabledExtensionCount = exts.size();
	InstanceCreateInfo.ppEnabledExtensionNames = exts.data();
	
	VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCreateInfo{};
	m_DebugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	m_DebugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	m_DebugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	m_DebugMessengerCreateInfo.pfnUserCallback = debugCallback;


	std::vector<const char*> validationLayers;
	if (_debug) {
		validationLayers.push_back("VK_LAYER_KHRONOS_validation");
		InstanceCreateInfo.pNext = &m_DebugMessengerCreateInfo;
	};

	InstanceCreateInfo.enabledLayerCount = validationLayers.size();
	InstanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

	VK_CHECK(vkCreateInstance(&InstanceCreateInfo, nullptr, &m_Instance));
	if(m_Debug)
		VK_CHECK(CreateDebugUtilsMessengerEXT(m_Instance, &m_DebugMessengerCreateInfo, nullptr, &m_DebugMessenger));

	_win.getSurface(m_Instance, &m_Surface);

	uint32_t gpuCount = 0;
	VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));

	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, physicalDevices.data()));

	assert(gpuCount > 0);

	bool systemHasDiscrete = false;
	VkPhysicalDevice selectedDev;
	for (auto device : physicalDevices) {
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(device, &props);

		if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			systemHasDiscrete = true;
			selectedDev = device;
		}
	}

	if (!systemHasDiscrete)
		selectedDev = physicalDevices[0];

	m_PhysicalDevice = selectedDev;

	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> familyProps(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, familyProps.data());

	uint32_t graphicsQueueFamilyIndex;
	
	bool queuFamilyFound = false;
	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		if (familyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsQueueFamilyIndex = i;
			queuFamilyFound = true;
			break;
		}
	}

	float queuePriorities =  0.0;
	VkDeviceQueueCreateInfo QueueCreateInfo;
	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.pNext = nullptr;
	QueueCreateInfo.queueCount = 1;
	QueueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
	QueueCreateInfo.pQueuePriorities = &queuePriorities;

	VkDeviceCreateInfo DeviceCreateInfo;
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.pNext = nullptr;
	DeviceCreateInfo.queueCreateInfoCount = 1;
	DeviceCreateInfo.pQueueCreateInfos = &QueueCreateInfo;
	DeviceCreateInfo.enabledExtensionCount = 0;
	DeviceCreateInfo.ppEnabledExtensionNames = nullptr;
	DeviceCreateInfo.enabledLayerCount = 0;
	DeviceCreateInfo.ppEnabledLayerNames = nullptr;
	DeviceCreateInfo.pEnabledFeatures = nullptr;

	VK_CHECK(vkCreateDevice(m_PhysicalDevice, &DeviceCreateInfo, nullptr, &m_Device));

	VkCommandPoolCreateInfo CmdPoolCreateInfo;
	CmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CmdPoolCreateInfo.pNext = nullptr;
	CmdPoolCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
	CmdPoolCreateInfo.flags = 0;
	
	VK_CHECK(vkCreateCommandPool(m_Device, &CmdPoolCreateInfo, nullptr, &m_CommandPool));
};

SparEngine::Renderer::~Renderer() {
	if (m_Debug)
		DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
	vkDestroyInstance(m_Instance, nullptr);
}