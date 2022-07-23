#ifndef DORY_RENDERER_SYSTEM_INCL
#define DORY_RENDERER_SYSTEM_INCL

#include "core/core.h"
#include "math/push.h"
#include "renderer/camera.h"
#include "renderer/device.h"
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
     * will use to render its objects.
     */
    class RendererSystem : public NoCopy
    {
        public:
            /**
             * @brief construct a new renderer system on a given device with a given render pass.
             */
            RendererSystem(Device& device, VkRenderPass render_pass);

            /**
             * @brief destroy the renderer system
             */
            ~RendererSystem();

            /**
             * @brief render the application's objects
             * @param command_buffer the
             */
            void RenderObjects(VkCommandBuffer command_buffer, std::vector<Object>& objects, const Camera& camera);

        private: // methods
            /**
             * @brief initialize the layout for the graphcis pipeline that the renderer will use
             */
            void CreatePipelineLayout();

            /**
             * @brief initialize the graphics pipeline that the renderer will use
             */
            void CreatePipeline(VkRenderPass render_pass);
            
        private: // members
            Device& m_device; // the device that the renderer will use
            std::unique_ptr<Pipeline> m_pipeline; // the renderer's graphics pipeline
            VkPipelineLayout m_pipeline_layout; // the layout/specs for the renderer's graphics pipeline

    }; // class RendererSystem

} // namespace DORY

#endif // DORY_RENDERER_SYSTEM_INCL