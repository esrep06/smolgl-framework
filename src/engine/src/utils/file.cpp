#include "engine/utils/file.hpp"
#include "cpp-utilz/logger/logger.hpp"

namespace sm 
{
    uint8_t file_util::set_working_dir(std::string path)
    {
        uint8_t res = 0;
#if defined(__unix__) || defined(__APPLE)
        res = chdir(path.c_str());

        if (res != 0) 
            utilz::logger::log(std::format("Failed to set working directory to: {}", path), utilz::logger::ERROR);

#elif defined(_WIN32) 
        res = SetCurrentDirectory(path);

        if (res == 0) 
            utilz::logger::log(std::format("Failed to set working directory to: {}", path), utilz::logger::ERROR);
#endif

        return 0;
    }
}

