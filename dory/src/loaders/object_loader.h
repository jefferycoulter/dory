#ifndef DORY_OBJECT_LOADER_INCL
#define DORY_OBJECT_LOADER_INCL

#include "renderer/model.h"
#include <tiny_obj_loader.h>
#include <string>

namespace DORY
{
    class ObjectLoader
    {
        public:
            static void Load(Model::Mesh& dmesh, const std::string& path);
    }; // class ObjectLoader
} // namespace DORY

#endif // DORY_OBJECT_LOADER_INCL