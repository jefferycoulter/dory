#ifndef DORY_CAMERA_INCL
#define DORY_CAMERA_INCL

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace DORY
{
    class Camera
    {
        public:
            /**
             * @brief set the projection matrix to be the orthographic projection
             * @param left left bound of the orthographic projection
             * @param right right bound of the orthographic projection
             * @param bottom bottom bound of the orthographic projection
             * @param top top bound of the orthographic projection
             * @param near near bound of the orthographic projection
             * @param far far bound of the orthographic projection
             */
            void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);

            /**
             * @brief set the projection matrix to be the perspective projection
             * 
             * @param fov vertical field of view of the camera in radians
             * @param aspect aspect ratio of the window
             * @param near near bound of the perspective projection
             * @param far far bound of the perspective projection
             */
            void SetPerspectiveProjection(float fov, float aspect, float near, float far);

            /**
             * @brief initialize the view matrix with the camera position and the camera direction
             * @param position position of the camera
             * @param direction direction the camera is facing
             * @param up the up direction
             */
            void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));

            /**
             * @brief initialize the view matrix with the camera position and a target (some point in space)
             * that will always be in the center of the camera view
             * @param position position of the camera
             * @param target the point in space that the camera will look at
             * @param up the up direction
             */
            void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));

            /**
             * @brief initialize the view matrix using euler angles
             * @param position position of the camera
             * @param rotation angles the camera is rotated around each axis in radians
             */
            void SetViewZYX(glm::vec3 position, glm::vec3 rotation);

            /**
             * @brief get the projection matrix
             * @return const glm::mat4& 
             */
            const glm::mat4& GetProjection() const { return m_projection_matrix; }

            /**
             * @brief get the view matrix
             * @return const glm::mat4& 
             */
            const glm::mat4& GetView() const { return m_view_matrix; }

        private:
            glm::mat4 m_projection_matrix{1.0f}; // projection transformation matrix
            glm::mat4 m_view_matrix{1.0f}; // camera transformation matrix
    }; // class Camera
} // namespace DORY

#endif // DORY_CAMERA_INCL