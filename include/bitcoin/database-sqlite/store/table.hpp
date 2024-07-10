#ifndef LIBBITCOIN_DATABASE_SQLITE_TABLE_HPP
#define LIBBTITCOIN_DATABASE_SQLITE_TABLE_HPP

#include "database-sqlite/wrapper/db.hpp"
#include "database-sqlite/wrapper/command.hpp"
#include "database-sqlite/wrapper/db_query.hpp"
#include <bitcoin/system.hpp>
#include <string>
#include <optional>


namespace libbitcoin {
    namespace database {
        
        template <typename Record>
        class table {
            public:
                table(db::database& db,std::string& table_name): db_(db), table_name(table_name) {};

                // table destructor
                virtual ~table() = default;

                // create a new table
                virtual bool create() = 0;

                bool drop() {
                    const std::string sql = "DROP TABLE IF EXISTS" + this->table_name;
                    return db_.exec(sql.c_str()) == SQLITE_OK;
                }

                bool insert(const Record& record) {
                    db::command command(db_, get_insert_sql().c_str());
                    bind_record(command,record);
                    return command.execute() == SQLITE_OK;
                }

                // most probably we won't need this!
                bool update(const Record& record) {
                    db::command command(db_,get_update_sql().c_str());
                    bind_record(command,record);
                    return command.execute() == SQLITE_OK:
                }

                // // remove a record! typename is used as the key_type belongs to Record template argument!
                // // key_type is dependent of Record type-> therefore we use typename Recordd::key_type
                // bool remove(const typename Record::key_type& key) {
                //     db::command command(db_,get_delete_sql().c_str());
                //     bind_record(command,key);
                //     return command.execute() == SQLITE_OK;
                // }

                // std::optional -> Record or null return type
                // use auto while instantiating this type!
                // not sure if this would work. will have to test it!

                std::optional<Record> get(const typename Record::key_type& key) {
                    db::query query(db_,get_select_sql().c_str());
                    bind_key(query,key);
                    db::query::query_iterator query_iterator(query);

                    while (query_iterator != SQLITE_ROW) {
                        query_iterator++;
                    }
                    return query_iterator;
                    
                }

            protected:
                db::database& db_;
                std::string table_name;

                // methods that will be implemented in the derived classes!
                virtual std::string get_create_sql() const = 0;
                virtual std::string get_insert_sql() const = 0;
                virtual std::string get_update_sql() const = 0;
                virtual std::string get_select_sql() const = 0;
                // virtual std::string get_delete_sql() const = 0;


                virtual void bind_record(db::command& cmd,const Record& record) const = 0;
                virtual void bind_key(db::command& cmd,const typename Record::key_type& key) const = 0;
                virtual void bind_key(db::query& query, const typename Record::key_type& key) const = 0;
                virtual Record extract_record(db::query& query) const = 0;
        };
    }
}

#endif