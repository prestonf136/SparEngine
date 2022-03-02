#include "RenderThread.h"
#include <iostream>
#include <chrono>
#include <functional>

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

SparEngine::RenderThread::RenderThread(RenderThreadData _Data, int32_t _iWidth, int32_t _iHeight) : m_Data{ _Data }
{
    m_Data.m_iWidth = _iWidth;
    m_Data.m_iHeight = _iHeight;
	m_pRenderThread = std::make_unique<std::thread>(&RenderThread::Create, this);
}

void SparEngine::RenderThread::CreateSwapchainAndAssociated() {
    std::vector<vk::SurfaceFormatKHR> formats = m_Data.m_PhysicalDevice->getSurfaceFormatsKHR(m_Data.m_Surface->get());
    assert(!formats.empty());
    vk::Format format = (formats[0].format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats[0].format;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_Data.m_PhysicalDevice->getSurfaceCapabilitiesKHR(m_Data.m_Surface->get());
    vk::Extent2D               swapchainExtent;
    if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
    {
        swapchainExtent.width = clamp(
            static_cast<uint32_t>(m_Data.m_iWidth),
            surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);

        swapchainExtent.height = clamp(
            static_cast<uint32_t>(m_Data.m_iHeight),
            surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    }
    else
    {
        swapchainExtent = surfaceCapabilities.currentExtent;
    }

    vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

    vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
        ? vk::SurfaceTransformFlagBitsKHR::eIdentity
        : surfaceCapabilities.currentTransform;

    vk::CompositeAlphaFlagBitsKHR compositeAlpha =
        (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
        : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
        : (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
        : vk::CompositeAlphaFlagBitsKHR::eOpaque;

    vk::SwapchainCreateInfoKHR swapChainCreateInfo(vk::SwapchainCreateFlagsKHR(),
        m_Data.m_Surface->get(),
        surfaceCapabilities.minImageCount,
        format,
        vk::ColorSpaceKHR::eSrgbNonlinear,
        swapchainExtent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        {},
        preTransform,
        compositeAlpha,
        swapchainPresentMode,
        true,
        nullptr);

    uint32_t queueFamilyIndices[2] = { m_Data.m_GraphicsQueuFamilyIndex, m_Data.m_PresentQueuFamilyIndex };
    if (m_Data.m_GraphicsQueuFamilyIndex != m_Data.m_PresentQueuFamilyIndex)
    {
        swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    auto _SwapChain = &m_Data.m_Device->get().createSwapchainKHRUnique(swapChainCreateInfo);
    std::vector<vk::Image> swapChainImages = m_Data.m_Device->get().getSwapchainImagesKHR(_SwapChain->get());

    std::vector<vk::UniqueImageView> imageViews;
    imageViews.reserve(swapChainImages.size());
    vk::ImageViewCreateInfo imageViewCreateInfo({}, {}, vk::ImageViewType::e2D, format, {}, { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
    for (auto image : swapChainImages)
    {
        imageViewCreateInfo.image = image;
        imageViews.push_back(m_Data.m_Device->get().createImageViewUnique(imageViewCreateInfo));
    }
}

void SparEngine::RenderThread::Create()
{
    CreateSwapchainAndAssociated();
}

SparEngine::RenderThread::~RenderThread()
{
	m_pRenderThread->join();
}
