/*
    SQLite3 Wrapper.
    (C) 2016 SiLeader.
*/

#include <string>

#include <sqlite3.h>

#include <core/detail/sqlite_wrapper.hpp>

namespace core {
    namespace detail {
        namespace  {
            inline std::string create_key_val(const std::vector<std::string>& strs)
            {
                std::string ret;
                for(auto&& str : strs) {
                    ret+=str+", ";
                }
                return std::string(ret.begin(), ret.end()-2);
            }

            inline void text_bind(sqlite3_stmt *stmt, const std::string& key, const std::string& val)
            {
                sqlite3_bind_text(stmt,
                    sqlite3_bind_parameter_index(stmt, key.c_str()),
                    val.c_str(), val.size(), SQLITE_TRANSIENT);
            }
        } /* noname */

        sqlite_wrapper::sqlite_wrapper(const std::string& fname, const std::string& table_name)
            : m_file_name(fname), m_table_name(table_name), m_db(nullptr)
        {
            auto err=sqlite3_open(m_file_name.c_str(), &m_db);
            if(err!=SQLITE_OK) {
                m_db=nullptr;
                return;
            }

            const std::string SELECT_QUERY="SELECT $cols FROM $table_name $where",
                INSERT_QUERY="INSERT INTO $table_name ($keys) VALUES ($values)";

            sqlite3_prepare_v2(m_db, SELECT_QUERY.c_str(), SELECT_QUERY.size(), &m_select_stmt, nullptr);
            sqlite3_prepare_v2(m_db, INSERT_QUERY.c_str(), INSERT_QUERY.size(), &m_insert_stmt, nullptr);
            return;
        }

        sqlite_wrapper::~sqlite_wrapper()
        {
            sqlite3_finalize(m_select_stmt);
            sqlite3_finalize(m_insert_stmt);
            auto err=sqlite3_close(m_db);
        }

        bool sqlite_wrapper::exec(const std::string& query)
        {
            auto err=sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, nullptr);
            return (err==SQLITE_OK ? true : false);
        }

        void sqlite_wrapper::change_table(const std::string& table_name)
        {
            m_table_name=table_name;
        }

        bool sqlite_wrapper::create_table(const std::string& table_name, bool if_not_exists)
        {
            return this->exec(("CREATE TABLE "+(if_not_exists ? "IF NOT EXISTS " : "")+table_name).cstr());
        }

        bool sqlite_wrapper::drop_table(const std::string& table_name, bool if_exists)
        {
            return this->exec((("DROP TABLE ")+(if_exists ? "IF EXISTS " : "")+table_name).cstr());
        }

        void sqlite_wrapper::insert(KeyValue kvs)
        {
            std::string keys, values;
            for(auto&& kv : kvs) {
                keys+=kv.first+", ";
                values+=kv.second+", ";
            }
            keys=std::string(keys.begin(), keys.end()-2);
            values=std::string(values.begin(), values.end()-2);

            text_bind(m_insert_stmt, "$table_name", m_table_name);

            text_bind(m_insert_stmt, "$keys", keys);

            text_bind(m_insert_stmt, "$values", values);

            while(SQLITE_DONE != sqlite3_step(m_insert_stmt));

            sqlite3_reset(m_insert_stmt);
        }

        std::vector<KeyValue> sqlite_wrapper::select(const std::vector<std::string>& keys, const std::string& where)
        {
            text_bind(m_select_stmt, "$col", create_key_val(keys));
            text_bind(m_select_stmt, "$table_name", m_table_name);
            text_bind(m_select_stmt, "$where", where.size()<=0 ? "WHERE "+where : "");

            KeyValue kvs;
            std::vector<KeyValue> kvv;
            while(SQLITE_ROW == (err = sqlite3_step(m_select_stmt)) ){
                for(std::size_t i=0; i<keys.size(); ++i) {
                    kvs[keys[i]]=sqlite3_column_text(m_select_stmt, i);
                }
                kvv.push_back(kvs);
            }

            sqlite3_reset(m_select_stmt);

            return kvv;
        }
    } /* detail */
} /* core */
