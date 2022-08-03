#ifndef DORY_POINT_LIGHT_SYSTEM_INCL
#define DORY_POINT_LIGHT_SYSTEM_INCL

#include "core/core.h"
#include "renderer/camera.h"
#include "renderer/device.h"
#include "renderer/frame_info.h"
#include "renderer/object.h"
#include "renderer/pipeline.h"
#include "renderer/swapchain.h"
#include "utils/nocopy.h"

#include <memory>
#include <vector>

namespace DORY
{
    /**
     * @brief class representing a renderer system. this describes the graphics pipeline that an application
     * will use to render its point lights.
     */
    class PointLightSystem : public NoCopy
    {
        public:
            /**
             * @brief construct a new renderer system on a given device with a given render pass.
             */
            PointLightSystem(Device& device, VkRenderPass render_pass, VkDescriptorSetLayout descriptor_set_layout);

            /**
             * @brief destroy the renderer system
             */
            ~PointLightSystem();

            /**
             * @brief render the application's point lights
             * @param command_buffer the command buffer to render on
             */
            void Render(FrameInfo frame_info);

        private: // methods    
            /**
             * @brief initialize the layout for the graphcis pipeline that the renderer will use
             */
            void CreatePipelineLayout(VkDescriptorSetLayout descriptor_set_layout);

            /**
             * @brief initialize the graphics pipeline that the renderer will use
             */
            void CreatePipeline(VkRenderPass render_pass);
            
        private: // members
            Device& m_device; // the device that the renderer will use
            std::unique_ptr<Pipeline> m_pipeline; // the renderer's graphics pipeline
            VkPipelineLayout m_pipeline_layout; // the layout/specs for the renderer's graphics pipeline
    }; // class PointLightSystem
} // namespace DORY

#endif // DORY_POINT_LIGHT_SYSTEM_INCL