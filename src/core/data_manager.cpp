/*
    SQLite3 Wrapper.
    (C) 2016 SiLeader.
*/

#include <string>
#include <vector>

#include <core/detail/sqlite_wrapper.hpp>

namespace  {
    std::string create_db_name(const std::string& path)
    {
        return path+"/web_package.db";
    }

    std::vector<std::string> db_cols(bool is_select)
    {
        std::vector<std::string> ret;
        ret.push_back("version");
        ret.push_back("name");
        ret.push_back("path");
        ret.push_back("dtype");
        ret.push_back("id");
        if(is_select) {

        }
    }
} /* noname */

namespace core {
    data_manager::data_manager(const std::string& path)
        : m_sql(create_db_name(path), "package")
    {

    }

    PackageInfo data_manager::get_package_info(const std::string& pkg_name)
    {
        auto packages=m_sql.select(db_cols(true), "name="+pkg_name);

        PackageInfo pi;
        pi.version=packages[0]["version"];
        pi.name=packages[0]["name"];
        pi.contains_path=packages[0]["path"];
        pi.id=packages[0]["id"];
        pi.display_type=(packages[0]["dtype"]=="inline" ? DisplayType::Inline : DisplayType::FullPage);

        return pi;
    }

} /* core */
