// #ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_POINT_HPP
// #define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_POINT_HPP

// #include "include/bitcoin/database-sqlite/store/table.hpp"
// #include "include/bitcoin/database-sqlite/store/schema.hpp"
// #include "include/bitcoin/database-sqlite/wrapper/db.hpp"
// #include <bitcoin/system.hpp>

// namespace libbitcoin {
// namespace database {
// namespace table {

// struct point : public table<schema::point> {

//     inline bool create_table(db::database& db)
//     {
//         return db.exec(schema::point::create_table) == SQLITE_OK;
//     }

//     inline bool insert(db::database& db, const system::chain::point& point)
//     {
//         const char* sql = R"(
//             INSERT OR REPLACE INTO points
//             (hash, index)
//             VALUES (?, ?)
//         )";

//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, encode_hash(point.hash()));
//         stmt.bind(2, point.index());

//         return stmt.finalize() == SQLITE_DONE;
//     }

//     inline system::chain::point::cptr get(db::database& db, const system::hash_digest& hash, uint32_t index)
//     {
//         const char* sql = "SELECT * FROM points WHERE hash = ? AND index = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1, encode_hash(hash));
//         stmt.bind(2, index);

//         if (stmt.step() != SQLITE_ROW) return nullptr;

//         return extract_point(stmt.get());
//     }

//     inline bool exists(db::database& db, const system::hash_digest& hash, uint32_t index)
//     {
//         const char* sql = "SELECT 1 FROM points WHERE hash = ? AND index = ? LIMIT 1";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, encode_hash(hash));
//         stmt.bind(2, index);

//         return stmt.step() == SQLITE_ROW;
//     }

//     inline bool remove(db::database& db, const system::hash_digest& hash, uint32_t index)
//     {
//         const char* sql = "DELETE FROM points WHERE hash = ? AND index = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, encode_hash(hash));
//         stmt.bind(2, index);

//         return stmt.finalize() == SQLITE_DONE;
//     }

// private:
//     inline std::string encode_hash(const system::hash_digest& hash)
//     {
//         return system::encode_base16(hash);
//     }

//     inline system::hash_digest decode_hash(const std::string& hash_str)
//     {
//         system::hash_digest hash;
//         system::decode_base16(hash, hash_str);
//         return hash;
//     }

//     inline system::chain::point::cptr extract_point(sqlite3_stmt* stmt)
//     {
//         auto hash = decode_hash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
//         auto index = sqlite3_column_int(stmt, 1);

//         return std::make_shared<system::chain::point>(hash, index);
//     }
// };

// }
// }
// }

// #endif // LIBBITCOIN_DATABASE_TABLES_ARCHIVES_POINT_HPP