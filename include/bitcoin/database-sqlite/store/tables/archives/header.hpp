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

#include "include/bitcoin/database-sqlite/store/table.hpp"
#include "include/bitcoin/database-sqlite/store/schema.hpp"
#include "include/bitcoin/database-sqlite/wrapper/statement.hpp"
#include "include/bitcoin/database-sqlite/wrapper/db.hpp"
#include <bitcoin/system.hpp>

namespace libbitcoin {
namespace database {
namespace table {
    using hash_digest = libbitcoin::system::data_array<32UL>;


    struct header : public table<schema::header> {

        inline bool create_table(db::database& db)
        {
            return db.exec(schema::header::create_table) == SQLITE_OK;
        }

        inline bool insert(db::database& db, const system::chain::header& header,uint32_t height, uint32_t flags,uint32_t mtp)
        {
            const char* sql = R"(
                INSERT OR REPLACE INTO headers
                (hash, version, previous_block_hash,merkle_root,timestamp,bits,nonce,height,flags,mtp)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            )";
            
            db::statement stmt(db);
            auto ec = stmt.prepare(sql);
            if (!ec) return false;

            stmt.bind(1, encode_hash(header.hash()));
            stmt.bind(2, header.version());
            stmt.bind(3, encode_hash(header.previous_block_hash()));
            stmt.bind(4, encode_hash(header.merkle_root()));
            stmt.bind(5, header.timestamp());
            stmt.bind(6, header.bits());
            stmt.bind(7, header.nonce());
            stmt.bind(8, height);
            stmt.bind(9, flags);
            stmt.bind(10, mtp);

            return stmt.finalize() == SQLITE_OK;
        }

        inline system::chain::header::cptr get_by_height(db::database& db, uint32_t height)
        {
            const char* sql = "SELECT * FROM headers WHERE hash = ?";
            db::statement stmt(db);
            auto ec = stmt.prepare(sql);

            stmt.bind(1,height);

            if (stmt.step() != SQLITE_ROW)
            {
                return nullptr;
            }
            return extract_header(stmt.get());

        }
    private:
        inline std::string encode_hash(const system::hash_digest& hash_digest)
        {
            return system::encode_base16(hash_digest);
        }

        inline system::hash_digest decode_hash(const std::string& str)
        {
            system::hash_digest hash;
            system::decode_base16(hash,str);
            return hash;
        }

        inline system::chain::header::cptr extract_header(db::statement& stmt) {
            auto header = std::shared_ptr<system::chain::header>();

            header->set_hash(decode_hash(reinterpret_cast<const char*>(sqlite3_column_text(stmt,0))));
            header->version(sqlite3_column_int(stmt,1));
            header->previous_block_hash(sqlite3_column_int(stmt,1));
            header->merkle_root(decode_hash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
            header->set_timestamp(sqlite3_column_int(stmt, 4));
            header->set_bits(sqlite3_column_int(stmt, 5));
            header->set_nonce(sqlite3_column_int(stmt, 6));

            return header;

        }
    };
}
}
}

#endif

