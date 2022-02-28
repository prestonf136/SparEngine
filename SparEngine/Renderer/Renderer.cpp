#include "Renderer.h"
#include <iostream>

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger)
{
    return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const* pAllocator)
{
    return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* /*pUserData*/)
{
    std::ostringstream message;

    message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
        << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
    message << "\t"
        << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
    message << "\t"
        << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
    message << "\t"
        << "message         = <" << pCallbackData->pMessage << ">\n";
    if (0 < pCallbackData->queueLabelCount)
    {
        message << "\t"
            << "Queue Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
        {
            message << "\t\t"
                << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
        }
    }
    if (0 < pCallbackData->cmdBufLabelCount)
    {
        message << "\t"
            << "CommandBuffer Labels:\n";
        for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
        {
            message << "\t\t"
                << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
        }
    }
    if (0 < pCallbackData->objectCount)
    {
        message << "\t"
            << "Objects:\n";
        for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
        {
            message << "\t\t"
                << "Object " << i << "\n";
            message << "\t\t\t"
                << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
            message << "\t\t\t"
                << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
            if (pCallbackData->pObjects[i].pObjectName)
            {
                message << "\t\t\t"
                    << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
            }
        }
    }

    std::cout << message.str() << std::endl;
    return false;
}

SparEngine::Renderer::Renderer(SparEngine::Window* _pWindow, bool _bDebug)
{
    std::vector<const char*> validationLayers;
    if (_bDebug)
        validationLayers.push_back("VK_LAYER_KHRONOS_validation");

    std::vector<const char*> extNames = _pWindow->getRequiredExtensions();

    if (_bDebug) {
        std::vector<vk::LayerProperties> layer_props = vk::enumerateInstanceLayerProperties();
        std::vector<vk::ExtensionProperties> ext_props = vk::enumerateInstanceExtensionProperties();

        auto extpropertyIterator = std::find_if(
            ext_props.begin(), ext_props.end(), [](vk::ExtensionProperties const& ep) { return strcmp(ep.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; });

        assert(extpropertyIterator != ext_props.end());

        auto layerpropertyIterator = std::find_if(
            layer_props.begin(), layer_props.end(), [](vk::LayerProperties const& ep) { return strcmp(ep.layerName, "VK_LAYER_KHRONOS_validation") == 0; });

        assert(layerpropertyIterator != layer_props.end());

        extNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    vk::ApplicationInfo applicationInfo {};
    applicationInfo.setPApplicationName(_pWindow->getName().c_str());
    applicationInfo.setPEngineName("Spar Engine");
    applicationInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
    applicationInfo.setEngineVersion(VK_MAKE_VERSION(1,0,0));
    applicationInfo.setApiVersion(VK_API_VERSION_1_2);
  
    vk::InstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.setPApplicationInfo(&applicationInfo);
    instanceCreateInfo.setEnabledExtensionCount(extNames.size());
    instanceCreateInfo.setPEnabledExtensionNames(extNames);
    instanceCreateInfo.setEnabledLayerCount(validationLayers.size());
    instanceCreateInfo.setPEnabledLayerNames(validationLayers);

    m_Instance = vk::createInstanceUnique(instanceCreateInfo);

    if (_bDebug) {
        pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(m_Instance.get().getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        assert(pfnVkCreateDebugUtilsMessengerEXT);

        pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(m_Instance.get().getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        assert(pfnVkDestroyDebugUtilsMessengerEXT);

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
        auto debugUtilsCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &debugMessageFunc);

        m_DebugUtilsMessenger = m_Instance.get().createDebugUtilsMessengerEXTUnique(debugUtilsCreateInfo);
    }

    auto instance = static_cast<VkInstance>(m_Instance.get());
    VkSurfaceKHR surface;

    _pWindow->getSurface(instance, &surface);
    m_Surface = vk::SurfaceKHR{ surface };

    // Will have to use some form of algorithm to determine the best GPU to select, this will do for now.
    // Or allow the user to set it through a config file, that is left to the game engine interface.
    m_PhysicalDevice = m_Instance.get().enumeratePhysicalDevices().front();

    auto queueFamilyProperties = m_PhysicalDevice.getQueueFamilyProperties();
    auto graphicsQueueIterator = std::find_if(queueFamilyProperties.begin(),
        queueFamilyProperties.end(),
        [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; });

    size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), graphicsQueueIterator);
    assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());

    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority);
    m_Device = m_PhysicalDevice.createDeviceUnique(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), deviceQueueCreateInfo));
}

std::shared_ptr<SparEngine::RenderObject> SparEngine::Renderer::createRenderObject()
{
    // We have no problem passing in raw pointers to the renderObjet as we know for a fact it won't outlive the renderer.

    auto render_obj = std::make_shared<SparEngine::RenderObject>(
        &m_Instance,
        &m_PhysicalDevice,
        &m_Device,
        &m_Surface,
        &m_CommandPool);
    return render_obj;
}

SparEngine::Renderer::~Renderer() {
}
