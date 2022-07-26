#include "renderer/swapchain.h"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <stdexcept>

#include <array>
#include <limits>
#include <set>
#include <algorithm>

namespace DORY
{
    SwapChain::SwapChain(Device &device_ref, VkExtent2D extent)
        : m_device{device_ref}, m_window_extent{extent} 
    {
        Init();
    }

    SwapChain::SwapChain(Device &device_ref, VkExtent2D extent, std::shared_ptr<SwapChain> old_swap_chain)
        : m_device{device_ref}, m_window_extent{extent}, m_old_swap_chain{old_swap_chain} 
    {
        Init();
        m_old_swap_chain = nullptr; // old swap chain is only used in initialization, so we don't need it anymore
    }

    void SwapChain::Init()
    {
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFramebuffers();
        CreateSyncObjects();
    }

    SwapChain::~SwapChain()
    {
        // clean up image views
        for (auto image_view : m_swap_chain_image_views)
        {
            vkDestroyImageView(m_device.GetDevice(), image_view, nullptr);
        }
        m_swap_chain_image_views.clear();

        // clean up swap chain
        if (m_swap_chain != nullptr)
        {
            vkDestroySwapchainKHR(m_device.GetDevice(), m_swap_chain, nullptr);
            m_swap_chain = nullptr;
        }

        // clean up depth resources
        for (size_t i = 0; i < m_depth_images.size(); i++)
        {
            vkDestroyImageView(m_device.GetDevice(), m_depth_image_views[i], nullptr);
            vkDestroyImage(m_device.GetDevice(), m_depth_images[i], nullptr);
            vkFreeMemory(m_device.GetDevice(), m_depth_image_memories[i], nullptr);
        }

        // clean up frame buffers
        for (auto framebuffer : m_swap_chain_framebuffers)
        {
            vkDestroyFramebuffer(m_device.GetDevice(), framebuffer, nullptr);
        }

        // clean up render pass
        vkDestroyRenderPass(m_device.GetDevice(), m_render_pass, nullptr);

        // clean up synchronization objects
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroySemaphore(m_device.GetDevice(), m_render_finished_semaphores[i], nullptr);
            vkDestroySemaphore(m_device.GetDevice(), m_image_available_semaphores[i], nullptr);
            vkDestroyFence(m_device.GetDevice(), m_in_flight_fences[i], nullptr);
        }
    }

    VkResult SwapChain::AcquireNextImage(uint32_t *image_index)
    {
        vkWaitForFences(m_device.GetDevice(), 1, &m_in_flight_fences[m_current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        VkResult result = vkAcquireNextImageKHR(m_device.GetDevice(),
                                                m_swap_chain,
                                                std::numeric_limits<uint64_t>::max(),
                                                m_image_available_semaphores[m_current_frame],  // must be a not signaled semaphore
                                                VK_NULL_HANDLE,
                                                image_index);

        return result;
    }

    VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *image_index)
    {
        if (m_images_in_flight[*image_index] != VK_NULL_HANDLE)
        {
            vkWaitForFences(m_device.GetDevice(), 1, &m_images_in_flight[*image_index], VK_TRUE, UINT64_MAX);
        }
        m_images_in_flight[*image_index] = m_in_flight_fences[m_current_frame];

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[] = {m_image_available_semaphores[m_current_frame]};
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = buffers;

        VkSemaphore signal_semaphores[] = {m_render_finished_semaphores[m_current_frame]};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        vkResetFences(m_device.GetDevice(), 1, &m_in_flight_fences[m_current_frame]);
        if (vkQueueSubmit(m_device.GetGraphicsQueue(), 1, &submit_info, m_in_flight_fences[m_current_frame]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = signal_semaphores;

        VkSwapchainKHR swap_chains[] = {m_swap_chain};
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swap_chains;
        present_info.pImageIndices = image_index;

        auto result = vkQueuePresentKHR(m_device.GetPresentQueue(), &present_info);

        m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }      

    void SwapChain::CreateSwapChain()
    {
        // get supported swap chain details and specify our desired properties
        SwapChainSupportDetails swap_chain_support = m_device.GetSwapChainSupport();

        VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(swap_chain_support._formats);
        VkPresentModeKHR present_mode = ChooseSwapPresentMode(swap_chain_support._present_modes);
        VkExtent2D extent = ChooseSwapExtent(swap_chain_support._capabilities);

        // implementation of swap chain provides a minimum number of images necessary to function with minImageCount.
        // it is recommended to add at least one more image to the image count to prevent latency.
        uint32_t image_count = swap_chain_support._capabilities.minImageCount + 1;
        // also make sure this doesn't exceed the maxImageCount
        if (swap_chain_support._capabilities.maxImageCount > 0 && image_count > swap_chain_support._capabilities.maxImageCount)
        {
            image_count = swap_chain_support._capabilities.maxImageCount;
        }

        // fill in the swap chain info struct
        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_device.GetSurface();
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = m_device.FindPhysicalQueueFamilies();
        uint32_t queue_family_indices[] = {indices._graphics_family, indices._present_family};
        if (indices._graphics_family != indices._present_family)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        } 
        else 
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;      // optional
            create_info.pQueueFamilyIndices = nullptr;  // optional
        }

        create_info.preTransform = swap_chain_support._capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;

        // if there is no old swap chain, return null, otherwise use the old swap chain
        create_info.oldSwapchain = m_old_swap_chain == nullptr ? VK_NULL_HANDLE : m_old_swap_chain->m_swap_chain;

        // create the swap chain
        if (vkCreateSwapchainKHR(m_device.GetDevice(), &create_info, nullptr, &m_swap_chain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swap chain!");
        }

        // we only specified a minimum number of images in the swap chain, so the implementation is
        // allowed to create a swap chain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        vkGetSwapchainImagesKHR(m_device.GetDevice(), m_swap_chain, &image_count, nullptr);
        m_swap_chain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_device.GetDevice(), m_swap_chain, &image_count, m_swap_chain_images.data());

        m_swap_chain_image_format = surface_format.format;
        m_swap_chain_extent = extent;
    }

    void SwapChain::CreateImageViews()
    {
        m_swap_chain_image_views.resize(m_swap_chain_images.size());
        for (size_t i = 0; i < m_swap_chain_images.size(); i++)
        {
            VkImageViewCreateInfo view_info{};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = m_swap_chain_images[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = m_swap_chain_image_format;
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_device.GetDevice(), &view_info, nullptr, &m_swap_chain_image_views[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create texture image view!");
            }
        }
    }

    void SwapChain::CreateRenderPass()
    {
        VkAttachmentDescription depth_attachment{};
        depth_attachment.format = FindDepthFormat();
        depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depth_attachment_ref{};
        depth_attachment_ref.attachment = 1;
        depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription color_attachment{};
        color_attachment.format = GetSwapChainImageFormat();
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;
        subpass.pDepthStencilAttachment = &depth_attachment_ref;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstSubpass = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = {color_attachment, depth_attachment};
        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        if (vkCreateRenderPass(m_device.GetDevice(), &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass!");
        }
    }

    void SwapChain::CreateFramebuffers()
    {
        m_swap_chain_framebuffers.resize(GetImageCount());
        for (size_t i = 0; i < GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments = {m_swap_chain_image_views[i], m_depth_image_views[i]};

            VkExtent2D swap_chain_extent = GetSwapChainExtent();
            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = m_render_pass;
            framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebuffer_info.pAttachments = attachments.data();
            framebuffer_info.width = swap_chain_extent.width;
            framebuffer_info.height = swap_chain_extent.height;
            framebuffer_info.layers = 1;

            if (vkCreateFramebuffer(m_device.GetDevice(), &framebuffer_info, nullptr, &m_swap_chain_framebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }
    }

    void SwapChain::CreateDepthResources()
    {
        VkFormat depth_format = FindDepthFormat();
        m_swap_chain_depth_format = depth_format;
        VkExtent2D swap_chain_extent = GetSwapChainExtent();

        m_depth_images.resize(GetImageCount());
        m_depth_image_memories.resize(GetImageCount());
        m_depth_image_views.resize(GetImageCount());

        for (size_t i = 0; i < m_depth_images.size(); i++)
        {
            VkImageCreateInfo image_info{};
            image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            image_info.imageType = VK_IMAGE_TYPE_2D;
            image_info.extent.width = swap_chain_extent.width;
            image_info.extent.height = swap_chain_extent.height;
            image_info.extent.depth = 1;
            image_info.mipLevels = 1;
            image_info.arrayLayers = 1;
            image_info.format = depth_format;
            image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
            image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            image_info.samples = VK_SAMPLE_COUNT_1_BIT;
            image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            image_info.flags = 0;

            m_device.CreateImageWithInfo(image_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_images[i], m_depth_image_memories[i]);

            VkImageViewCreateInfo view_info{};
            view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            view_info.image = m_depth_images[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = depth_format;
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_device.GetDevice(), &view_info, nullptr, &m_depth_image_views[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create texture image view!");
            }
        }
    }

    void SwapChain::CreateSyncObjects()
    {
        m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
        m_images_in_flight.resize(GetImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_device.GetDevice(), &semaphore_info, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device.GetDevice(), &semaphore_info, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device.GetDevice(), &fence_info, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create synchronization objects for a frame!");
            }
        }
    }

    VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &available_formats)
    {
        for (const auto &available_format : available_formats)
        {
            if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return available_format;
            }
        }

        return available_formats[0];
    }

    VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &available_present_modes)
    {
        for (const auto &available_present_mode : available_present_modes)
        {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                std::cout << "Present mode: Mailbox" << "\n";
                return available_present_mode;
            }
        }

        // for (const auto &available_present_mode : available_present_modes)
        // {
        //   if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
        //     std::cout << "Present mode: Immediate" << "\n";
        //     return available_present_mode;
        //   }
        // }

        std::cout << "Present mode: V-Sync" << "\n";
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actual_extent = m_window_extent;
            actual_extent.width = std::max( capabilities.minImageExtent.width,
                                            std::min(capabilities.maxImageExtent.width, 
                                            actual_extent.width));
            actual_extent.height = std::max(capabilities.minImageExtent.height,
                                            std::min(capabilities.maxImageExtent.height, 
                                            actual_extent.height));

            return actual_extent;
        }
    }

    VkFormat SwapChain::FindDepthFormat()
    {
        return m_device.FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                            VK_IMAGE_TILING_OPTIMAL,
                                            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
} // namespace DORY