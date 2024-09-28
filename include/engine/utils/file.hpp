#pragma once 

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#include <iostream>
#include <inttypes.h>
#include <format>
#include "cpp-utilz/logger/logger.hpp"

namespace sm
{
    class file_util
    {
        public:
            static uint8_t set_working_dir(std::string path);
    };
}

