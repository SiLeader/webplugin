/*
    SQLite3 Wrapper.
    (C) 2016 SiLeader.
*/

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include <sqlite3.h>

namespace core {
    namespace detail {
        template<class T> std::vector<std::string> get_key(const std::unordered_map<std::string, T>& kvs)
        {
            std::vector<std::string> keys;
            for(const auto&& kv : kvs) {
                keys.push_back(kvs.first);
            }
            return keys;
        }

        class sqlite_wrapper
        {
        public:
            using KeyValue=std::unordered_map<std::string, std::string>;
        private:
            std::string m_file_name, m_table_name;
            sqlite3 *m_db;
            sqlite3_stmt *m_select_stmt, *m_insert_stmt;
        public:
            sqlite_wrapper(const std::string& fname, const std::string& table_name);
            ~sqlite_wrapper();

            bool exec(const std::string& query);

            void change_table(const std::string& table_name);

            bool create_table(const std::string& table_name, bool if_not_exists=true);
            bool drop_table(const std::string& table_name, bool if_exists=true);

            void insert(KeyValue kvs);

            std::vector<KeyValue> select(const std::vector<std::string>& keys, const std::string& where="");
        };
    } /* detail */
} /* core */
