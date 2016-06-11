/*
    Data Manager.
    (C) 2016 SiLeader.
*/

#pragma once

#include <string>

#include <core/detail/sqlite_wrapper.hpp>

namespace core {
    class data_manager
    {
    public:
        class DisplayType {
            Inline, FullPage,
        };
        struct PackageInfo {
            std::string contains_path;
            std::string name;
            std::string version;
            std::string id;
            DisplayType display_type;
        };
    private:
        core::detail::sqlite_wrapper m_sql;
    public:
        data_manager(const std::string& path);

        PackageInfo get_package_info(const std::string& pkg_name);
    };
} /* core */
