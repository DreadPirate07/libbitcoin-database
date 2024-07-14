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
#include "wrapper/sqlite3pp.h"

namespace libbitcoin {
namespace database {
namespace table {
    // using hash_digest = libbitcoin::system::data_array<32UL>;

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

            stmt.bind(1, encode_hash(header.hash()).c_str(),db::copy_semantic::nocopy);
            stmt.bind(2, header.version());
            stmt.bind(3, encode_hash(header.previous_block_hash()).c_str(),db::copy_semantic::nocopy);
            stmt.bind(4, encode_hash(header.merkle_root()).c_str(),db::copy_semantic::nocopy);
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
            return extract_header(stmt);

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

        // We're trying to create a function that can be used to extract header data from a SQLite database
        // and create a system::chain::header object, which is part of the libbitcoin system.
        inline system::chain::header::cptr extract_header(db::statement& stmt) {
            const char* sql = "SELECT * FROM headers WHERE has = ?";

            if (stmt.step() != SQLITE_ROW)
            {
                return nullptr;
            }


            const auto version = static_cast<uint32_t>(stmt.column_uint32(1));
            const auto previous_block_hash = decode_hash(stmt.column_text(2));
            const auto merkle_root = decode_hash(stmt.column_text(3));
            const auto timestamp = static_cast<uint32_t>(stmt.column_uint32(4));
            const auto bits = static_cast<uint32_t>(stmt.column_uint32(5));
            const auto nonce = static_cast<uint32_t>(stmt.column_uint32(5));

            auto header = std::make_shared<system::chain::header>(
                version,
                previous_block_hash,
                merkle_root,
                timestamp,
                bits,
                nonce
            );

            // Set the hash separately as it's not part of the constructor
            header->set_hash(decode_hash(stmt.column_text(0)));

            return header;

        }
    };
}
}
}

#endif