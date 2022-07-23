#ifndef DORY_SWAP_CHAIN_INCL
#define DORY_SWAP_CHAIN_INCL

#include "renderer/device.h"
#include "utils/nocopy.h"

#include <vulkan/vulkan.h>

#include <memory>
#include <string>
#include <vector>

namespace DORY
{
    /**
     * @brief this class encapsulates the swap chain and its associated images and buffers.  it creates the
     * frame buffers and render pass for the swap chain.
     */
    class SwapChain : public NoCopy
    {
        public:
            // limit at most 2 command buffers sent to device graphics queue at once
            static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

            /**
             * @brief create a new swap chain on a given device with a given window size
             * @param device_ref device to create the swap chain on
             * @param window_extent size of window
             */
            SwapChain(Device &device_ref, VkExtent2D window_extent);

            /**
             * @brief recreate the swap chain with a new window size
             * 
             * @param device_ref device to create the swap chain on
             * @param window_extent size of window
             * @param old_swap_chain the old swap chain to destroy
             */
            SwapChain(Device &device_ref, VkExtent2D window_extent, std::shared_ptr<SwapChain> old_swap_chain);

            ~SwapChain();

            /**
             * @brief get frame buffer at a specified index in the swap chain
             * @param index index of frame buffer
             * @return VkFramebuffer 
             */
            VkFramebuffer GetFrameBuffer(int index) { return m_swap_chain_framebuffers[index]; }

            /**
             * @brief get the render pass
             * @return VkRenderPass 
             */
            VkRenderPass GetRenderPass() { return m_render_pass; }

            /**
             * @brief get the image view at a specified index in the swap chain
             * @return int 
             */
            VkImageView GetImageView(int index) { return m_swap_chain_image_views[index]; }

            /**
             * @brief get the number of images available in the swap chain. typically two or three
             * depending on whether there is support for double or triple buffering.
             */
            size_t GetImageCount() { return m_swap_chain_images.size(); }

            /**
             * @brief get swap chain image format
             * @return VkFormat 
             */
            VkFormat GetSwapChainImageFormat() { return m_swap_chain_image_format; }

            /**
             * @brief get swap chain image extent
             * @return VkExtent2D 
             */
            VkExtent2D GetSwapChainExtent() { return m_swap_chain_extent; }

            /**
             * @brief get the width of the swap chain extent
             * @return uint32_t 
             */
            uint32_t width() { return m_swap_chain_extent.width; }

            /**
             * @brief get the height of the swap chain extent
             * @return uint32_t 
             */
            uint32_t height() { return m_swap_chain_extent.height; }

            /**
             * @brief get the aspect ratio of the swap chain extent
             * @return float 
             */
            float ExtentAspectRatio(){ return static_cast<float>(m_swap_chain_extent.width) / static_cast<float>(m_swap_chain_extent.height);}

            /**
             * @brief find supported depth format for the device
             * @return VkFormat 
             */
            VkFormat FindDepthFormat();

            /**
             * @brief when the swap chain is recreated, the image and depth formats could potentially change.
             * this checks whether the swap chain image format and depth format are compatible with the current 
             * @param swap_chain 
             * @return true 
             * @return false 
             */
            bool CompareFormats(const SwapChain& swap_chain) const
            {
                return m_swap_chain_image_format == swap_chain.m_swap_chain_image_format &&
                       m_swap_chain_depth_format == swap_chain.m_swap_chain_depth_format;
            }

            /**
             * @brief get the index of next image in the swap chain.  this is the image that will be 
             * rendered to next.
             * @param image_index the index of the image to get
             * @return VkResult 
             */
            VkResult AcquireNextImage(uint32_t *image_index);

            /**
             * @brief submit the command buffer for the selected image to the device graphics queue. 
             * @param buffers the command buffer to submit
             * @param image_index the index of the image to submit
             * @return VkResult 
             */
            VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *image_index);

        private: // constructor functions

            /**
             * @brief the constructor for the swap chain.  this creates the swap chain but, it's given it's
             * own function because the SwapChain class has two constructors, one for the initial creation
             * and another for the recreation of the swap chain when the window size changes.
             */
            void Init();

            /**
             * @brief create a swap chain using the desired properties stored in a SwapChainSupportDetails struct.
             */
            void CreateSwapChain();

            /**
             * @brief create image views for each image in the swap chain. the image view describes
             * how the image is to be accessed. https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Image_views
             */
            void CreateImageViews();

            /**
             * @brief create depth resources for the swap chain.  this is used for depth testing.
             */
            void CreateDepthResources();

            /**
             * @brief create a render pass for the swap chain. the render pass describes the 
             * attachments and subpasses to be used for rendering the swap chain images.
             */
            void CreateRenderPass();

            /**
             * @brief create a framebuffer for each image in the swap chain.  this is used for rendering by
             * binding a VkImageView with an attachment, e.g. color, depth, etc.
             */
            void CreateFramebuffers();

            /**
             * @brief create synchronization objects for the swap chain
             */
            void CreateSyncObjects();

        private: // swap chain support details functions
            /**
             * @brief choose "optimal" surface format for swap chain.  first looks for 8-bit BGRA format with
             * SRGB color space.  if not available, then it takes the first available format.
             * @param available_formats list (vector) of available formats
             * @return VkSurfaceFormatKHR the chosen format
             * 
             */
            VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &available_formats);
            
            /**
             * @brief select the presentation mode for the swap buffers.  currently looks for "mailbox" mode
             * but if this is unavailable, then it will default to FIFO
             * @param available_present_modes list (vector) of available presentation modes
             * @return VkPresentModeKHR the chosen presentation mode
             */
            VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &available_present_modes);

            /**
             * @brief select the extent, or resolution, of the swap chain images (typically, this is the
             * resolution of the window in pixels). see https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Swap_chain
             * @param capabilities surface capabilities
             * @return VkExtent2D 
             */
            VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        private: // members
            VkFormat m_swap_chain_image_format;
            VkFormat m_swap_chain_depth_format;
            VkExtent2D m_swap_chain_extent;

            std::vector<VkFramebuffer> m_swap_chain_framebuffers;
            VkRenderPass m_render_pass;

            std::vector<VkImage> m_depth_images;
            std::vector<VkDeviceMemory> m_depth_image_memories;
            std::vector<VkImageView> m_depth_image_views;
            std::vector<VkImage> m_swap_chain_images;
            std::vector<VkImageView> m_swap_chain_image_views;

            Device &m_device;
            VkExtent2D m_window_extent;

            VkSwapchainKHR m_swap_chain;
            std::shared_ptr<SwapChain> m_old_swap_chain;

            std::vector<VkSemaphore> m_image_available_semaphores; // used to signal that an image is available for rendering
            std::vector<VkSemaphore> m_render_finished_semaphores; // used to signal that an image has been rendered
            std::vector<VkFence> m_in_flight_fences;
            std::vector<VkFence> m_images_in_flight;
            size_t m_current_frame = 0;
    }; // class SwapChain
} // namespace DORY

#endif // DORY_SWAP_CHAIN_INCL