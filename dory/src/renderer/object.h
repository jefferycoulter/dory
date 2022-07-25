#ifndef DORY_OBJECT_INCL
#define DORY_OBJECT_INCL

#include "renderer/model.h" // includes glm
#include "math/transforms.h"
#include "utils/nocopy.h"

#include <memory>

namespace DORY
{   
    /**
     * @brief basic class to represent a graphical object with properties (a game entity).  will probably
     * switch to something like entt for this in the future, but this is a simple way to get things working.
     */
    class Object //: public NoCopy
    {
        public:
            /**
             * @brief essentially the constructor. used to create an object with a unique id
             * @return Object 
             */
            static Object CreateObject()
            {
                static unsigned int current_id = 0;
                return Object{current_id++};
            }

            /**
             * @brief get the unqiue id of an object
             * @return unsigned int 
             */
            uint32_t GetObjectId() const { return m_id; }

            // for some reason inheriting from NoCopy doesn't work here, so delete the copy constructor and assignment operator
            Object(Object const&) = delete;
            Object& operator=(Object const&) = delete;
            // default move constructor and assignment operator
            Object(Object&&) = default;
            Object& operator=(Object&&) = default;

            std::shared_ptr<Model> m_model{};
            glm::vec3 m_color{};
            TransformObject transform{};

        private: // methods
            // private constructor to ensure objects created with CreateObject() have unqiue ids
            Object(unsigned int id)
                : m_id{id}
            {
            };

        private: // members
            uint32_t m_id; // object id
    }; // class Object
} // namespace DORY

#endif // DORY_OBJECT_INCL