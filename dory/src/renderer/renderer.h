#ifndef DORY_RENDERER_INCL
#define DORY_RENDERER_INCL

#include "core/core.h"
#include "platform/window.h"
#include "renderer/device.h"
#include "renderer/swapchain.h"
#include "utils/nocopy.h"



namespace DORY
{
    /**
     * @brief class representing the renderer
     */
    class Renderer : NoCopy
    {
        public:
            /**
             * @brief construct a new Renderer object
             */
            Renderer(Window& window, Device& device);

            /**
             * @brief destroy the Renderer object
             */
            ~Renderer();

            /**
             * @brief get the render pass
             * @return VkRenderPass 
             */
            VkRenderPass GetSwapChainRenderPass() const { return m_swap_chain->GetRenderPass(); }

            /**
             * @brief check if a frame is currently being rendered
             * @return true 
             * @return false 
             */
            bool IsRunning() const { return m_frame_in_progress; }

            /**
             * @brief get the command buffer of the current image being rendered
             * @return VkCommandBuffer 
             */
            VkCommandBuffer GetCurrentCommandBuffer() const
            { 
                DASSERT_MSG(m_frame_in_progress, "Can't get current command buffer when frame is in progress");
                return m_command_buffer[m_current_image_index]; 
            }

            /**
             * @brief begin the current frame and command buffer for the current image
             * @return VkCommandBuffer 
             */
            VkCommandBuffer BeginFrame();

            /**
             * @brief submit the current command buffer for the current image to the graphics queue
             */
            void EndFrame();

            /**
             * @brief begin the render pass for the current frame
             * @param command_buffer 
             */
            void BeginSwapChainRenderPass(VkCommandBuffer command_buffer);

            /**
             * @brief end the render pass for the current frame
             * @param command_buffer 
             */
            void EndSwapChainRenderPass(VkCommandBuffer command_buffer);

        private: // methods
            /**
             * @brief create a Command Buffer object(s).  In Vulkan, the command buffer pre-records commands
             * to be sent and executed on the GPU rather than, for example in OpenGL, using a while loop
             * and executing the commands in the loop. 
             */
            void CreateCommandBuffers();

            /**
             * @brief if the swap chain is recreated with a different size, this function frees the command buffers
             * of the previous swap chain so that new command buffers can be created with the new swap chain size.
             */
            void FreeCommandBuffers();

            /**
             * @brief recreate the swap chain if the window is resized
             */
            void RecreateSwapChain();

        private: // members
            Window& m_window; // the window to render to
            Device& m_device; // the device to render with
            std::unique_ptr<SwapChain> m_swap_chain; // the renderer's swap chain
            std::vector<VkCommandBuffer> m_command_buffer; // renderer's command buffers
            uint32_t m_current_image_index; // index of the current image in the swap chain
            bool m_frame_in_progress = false; // check whether a frame is in progress

    }; // class Renderer

} // namespace DORY

#endif // DORY_RENDERER_INCL