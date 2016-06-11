/*
    arg processor.
    (C) 2016 SiLeader.
*/

#pragma once

#include <string>
#include <vector>

namespace base {
    inline std::vector<std::string> format_params(int argc, char **argv)
    {
        std::vector<std::string> params;
        for(std::size_t i=0; i<argc; ++i) {
            params.push_back(argv[i]);
        }
        return params;
    }
} /* base */
