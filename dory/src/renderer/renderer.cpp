#include "renderer/renderer.h"
#include <array>

namespace DORY
{
    Renderer::Renderer(Window& window, Device& device)
        : m_window{window}, m_device{device}
    {
        RecreateSwapChain();
        CreateCommandBuffers();
    }
    
    Renderer::~Renderer()
    {
        FreeCommandBuffers();
    }

    void Renderer::CreateCommandBuffers()
    {
        m_command_buffer.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        // initialize the command buffer info struct
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = m_device.GetCommandPool();
        alloc_info.commandBufferCount = static_cast<uint32_t>(m_command_buffer.size());

        // allocate memory for the command buffers
        if (vkAllocateCommandBuffers(m_device.GetDevice(), &alloc_info, m_command_buffer.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers!");
        }
    }

    void Renderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(m_device.GetDevice(), m_device.GetCommandPool(), static_cast<uint32_t>(m_command_buffer.size()), m_command_buffer.data());
        m_command_buffer.clear();
    }

    void Renderer::RecreateSwapChain()
    {
        auto extent = m_window.GetExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = m_window.GetExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(m_device.GetDevice());

        // if no swap chain, then create a new one
        if (m_swap_chain == nullptr)
        {
            m_swap_chain = std::make_unique<SwapChain>(m_device, extent);
        }
        else // there is an existing swap chain
        {
            // save the old swap chain
            std::shared_ptr<SwapChain> old_swap_chain = std::move(m_swap_chain);
            // make a new one with new extent
            m_swap_chain = std::make_unique<SwapChain>(m_device, extent, old_swap_chain);
            // check that the formats are the same
            if (!old_swap_chain->CompareFormats(*m_swap_chain.get()))
            {
                throw std::runtime_error("Swap chain image or depth format changed!");
            }
        }

        //CreatePipeline();
    }

    VkCommandBuffer Renderer::BeginFrame()
    {
        DASSERT_MSG(!m_frame_in_progress, "Can't begin frame when frame is in progress.");

        auto result = m_swap_chain->AcquireNextImage(&m_current_image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapChain();
            return nullptr; // frame not started successfully
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Failed to acquire the next swap chain image!");
        }

        m_frame_in_progress = true;

        auto command_buffer = GetCurrentCommandBuffer();
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        return command_buffer;
    }

    void Renderer::EndFrame()
    {
        DASSERT_MSG(m_frame_in_progress, "Can't end frame when a frame is in not progress.");

        auto command_buffer = GetCurrentCommandBuffer();
        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer!");
        }

        auto result = m_swap_chain->SubmitCommandBuffers(&command_buffer, &m_current_image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.WindowResized())
        {
            m_window.ResetWindowResizedFlag();
            RecreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit command buffer!");
        }

        m_frame_in_progress = false;
        m_current_frame_index = (m_current_frame_index + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::BeginSwapChainRenderPass(VkCommandBuffer command_buffer)
    {
        DASSERT_MSG(m_frame_in_progress, "Can't begin render pass when a frame is in not progress.");
        DASSERT_MSG(command_buffer == GetCurrentCommandBuffer(), "Can't begin render pass with invalid command buffer.");

        // first get a render pass created by the swap chain
        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_swap_chain->GetRenderPass();
        render_pass_info.framebuffer = m_swap_chain->GetFrameBuffer(m_current_image_index); // specify which frame buffer the render pass is writing
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = m_swap_chain->GetSwapChainExtent();

        // specify initial value for frame buffer attachments to be cleared to
        // index 0 is for color, index 1 is for depth. see ::SwapChain::CreateRenderPass() implementation
        std::array<VkClearValue, 2> clear_values{};
        clear_values[0].color = {0.7f, 0.5f, 0.3f, 1.0f};
        clear_values[1].depthStencil = {1.0f, 0};
        render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
        render_pass_info.pClearValues = clear_values.data();

        // record the above actions to the command buffer, begin render pass and bind the graphics pipeline
        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        // specify the viewport and scissor rectangles
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swap_chain->GetSwapChainExtent().width);
        viewport.height = static_cast<float>(m_swap_chain->GetSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = m_swap_chain->GetSwapChainExtent();
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    }

    void Renderer::EndSwapChainRenderPass(VkCommandBuffer command_buffer)
    {
        DASSERT_MSG(m_frame_in_progress, "Can't end render pass when a frame is in not progress.");
        DASSERT_MSG(command_buffer == GetCurrentCommandBuffer(), "Can't end render pass with invalid command buffer.");

        vkCmdEndRenderPass(command_buffer);
    }
} // namespace DORY