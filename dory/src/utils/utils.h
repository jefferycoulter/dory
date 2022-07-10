#ifndef DORY_UTILS_INCL
#define DORY_UTILS_INCL

#include <string>
#include <vector>
#include <fstream>

namespace DORY
{
    namespace Utils
    {
        /**
         * @brief read and return file content from a given file path.  it returns a character vector
         * rather than a C-style character array because, when creating a Vulkan shader module, the
         * required type for the shader source code input is uint32_t and casting a character array
         * to uint32_t results in an invalid cast.
         * 
         * @param file_path path to file
         * @return std::vector<char>
         */
        std::vector<char> ReadFile(const std::string& file_path);
    } // namespace Utils 

} // namespace DORY

#endif // DORY_UTILS_INCL