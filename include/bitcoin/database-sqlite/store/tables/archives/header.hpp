/**
 * Copyright (c) 2011-2023 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_HEADER_HPP
#define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_HEADER_HPP

#include <bitcoin/system.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/primitives/primitives.hpp>
#include <bitcoin/database/tables/context.hpp>
#include <bitcoin/database/tables/schema.hpp>
#include <bitcoin/database-sqlite/wrapper/db.hpp>          // for db::database
#include <bitcoin/database-sqlite/wrapper/transaction.hpp> // for db::transaction
#include <bitcoin/database-sqlite/wrapper/command.hpp>     // for db::command
#include <bitcoin/database-sqlite/wrapper/db_query.hpp>    // for db::queries
#include <bitcoin/database-sqlite/wrapper/statement.hpp>   // for db::statement



 
    // CREATE TABLE IF NOT EXISTS header (
    //     pk INTEGER PRIMARY KEY,
    //     sk_hash BLOB,
    //     flags INTEGER,
    //     height INTEGER,
    //     mtp INTEGER,
    //     header_fk INTEGER,
    //     version INTEGER,
    //     time INTEGER,
    //     bits INTEGER,
    //     nonce INTEGER,
    //     merkle_root BLOB,
    //     FOREIGN KEY (header_fk) REFERENCES header (pk)
    // )

    
// /**
//  * The `header` class represents the header table in the database.
//  * It provides methods for creating, modifying, and querying the header table.
//  */
// class header
// {
// public:
//     /**
//      * Create the header table.
//      * @param database The database object.
//      * @param transaction The transaction object.
//      * @return A status code indicating success or failure.
//      */
//     static int create(db::database& database, db::transaction& transaction);

//     /**
//      * Insert a row into the header table.
//      * @param database The database object.
//      * @param transaction The transaction object.
//      * @param row The row to insert.
//      * @return A status code indicating success or failure.
//      */
//     static int insert(db::database& database, db::transaction& transaction, const db::table::archives::row& row);

//     /**
//      * Update a row in the header table.
//      * @param database The database object.
//      * @param transaction The transaction object.
//      * @param row The row to update.
//      * @return A status code indicating success or failure.
//      */
//     static int update(db::database& database, db::transaction& transaction, const db::table::archives::row& row);

//     /**
//      * Delete a row from the header table.
//      * @param database The database object.
//      * @param transaction The transaction object.
//      * @param hash The hash of the row to delete.
//      * @return A status code indicating success or failure.
//      */
//     static int remove(db::database& database, db::transaction& transaction, const hash_digest& hash);

//     /**
//      * Find a row in the header table by hash.
//      * @param

namespace libbitcoin
{
    namespace database
    {
        namespace table
        {
            struct header
            {
                struct record {
                    inline bool from_data(reader& source) NOEXCEPT
                    {

                    }
                    inline bool to_data(finalizer& sink) const NOEXCEPT {

                    }
                    inline bool operator==(const record& other) const NOEXCEPT {

                    }
                    // members data types 
                    // will be writing these in the type that will be stored in sql!
                    // context ctx{};
                    // bool milestone{};
                    // link::integer parent_fk{};
                    // uint32_t version{};
                    // uint32_t timestamp{};
                    // uint32_t bits{};
                    // uint32_t nonce{};
                    // hash_digest merkle_root{};
                };
                

            };
            // i think record put refernce would not be needed!
            // at last since we have to return the data in the query
            // so the query can be processed intermediately in the ipps!
            // similarly record put refernce might also be not needed!

            // returns timestamp!
            struct get_version {
                inline bool from_data(reader& source) NOEXCEPT {

                }
            };

            // returns bits! ()
            struct get_bits {
                inline bool from_data(reader& source) NOEXCEPT {

                }
                unit32_t bits{}; // put bits!
            };

            // get parent fk returns the header fk which is the pk of block before
            struct get_parent_fk {
                inline bool from_data(reader& source) {

                }
                uint32_t parent_fk{};
            };

            struct get_flags {
                inline bool from_data(reader& source) NOEXCEPT {
                    return source;
                }
                short_int flag; // 2 bytes
            };
            
            // returns the height!
            struct get_height {
                inline bool from_data(reader& source) NOEXCEPT {
                    height = store_db::store.get_height(); // modify the height member of source
                    return source; // return source;
                }
                uint32_t height{};
            };

            struct get_mtp {
                inline bool from_data(reader& source) NOEXCEPT {
                    return source;
                }
                uint32_t mtp{};
            };

            struct get_milestone {
                inline bool from_data(reader& source) NOEXCEPT {
                    return source;
                }
                bool milestone{};
            };
            

            // get check context - a bit ambiguos about it's understanding!
            // and also if context too needs an implementation!


        };
    };
}; // namespace

#endif

