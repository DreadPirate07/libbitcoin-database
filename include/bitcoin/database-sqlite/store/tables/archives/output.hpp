// #ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_OUTPUT_HPP
// #define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_OUTPUT_HPP

// #include "include/bitcoin/database-sqlite/store/table.hpp"
// #include "include/bitcoin/database-sqlite/store/schema.hpp"
// #include "include/bitcoin/database-sqlite/wrapper/db.hpp"
// #include <bitcoin/system.hpp>

// namespace libbitcoin {
// namespace database {
// namespace table {

// struct output : public table<schema::output> {

//     inline bool create_table(db::database& db)
//     {
//         return db.exec(schema::output::create_table) == SQLITE_OK;
//     }

//     inline bool insert(db::database& db, const system::chain::output& output, const system::hash_digest& tx_hash, uint32_t index)
//     {
//         const char* sql = R"(
//             INSERT OR REPLACE INTO outputs
//             (tx_hash, index, value, script, is_spent)
//             VALUES (?, ?, ?, ?, ?)
//         )";

//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1, encode_hash(tx_hash));
//         stmt.bind(2, index);
//         stmt.bind(3, output.value());
//         stmt.bind(4, encode_script(output.script()));
//         stmt.bind(5, 0);  // Initially set as unspent

//         return stmt->exec() == SQLITE_DONE;
//     }

//     inline system::chain::output::cptr get(db::database& db, const system::hash_digest& tx_hash, uint32_t index)
//     {
//         const char* sql = "SELECT * FROM outputs WHERE tx_hash = ? AND index = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1, encode_hash(tx_hash));
//         stmt.bind(2, index);

//         if (stmt.step() != SQLITE_ROW) return nullptr;

//         return extract_output(stmt.get());
//     }

//     inline system::chain::outputs_ptr get_for_transaction(db::database& db, const system::hash_digest& tx_hash)
//     {
//         const char* sql = "SELECT * FROM outputs WHERE tx_hash = ? ORDER BY index";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1, encode_hash(tx_hash));

//         auto outputs = std::make_shared<system::chain::output_cptrs>();
//         while (stmt.step() == SQLITE_ROW) {
//             outputs->push_back(extract_output(stmt.get()));
//         }

//         return outputs;
//     }

//     inline bool set_spent(db::database& db, const system::hash_digest& tx_hash, uint32_t index, bool spent)
//     {
//         const char* sql = "UPDATE outputs SET is_spent = ? WHERE tx_hash = ? AND index = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, spent ? 1 : 0);
//         stmt.bind(2, encode_hash(tx_hash));
//         stmt.bind(3, index);

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

//     inline std::string encode_script(const system::chain::script& script)
//     {
//         return system::encode_base16(script.to_data(false));
//     }

//     inline system::chain::script decode_script(const std::string& script_str)
//     {
//         system::data_chunk script_data;
//         system::decode_base16(script_data, script_str);
//         return system::chain::script::from_data(script_data, false);
//     }

//     inline system::chain::output::cptr extract_output(sqlite3_stmt* stmt)
//     {
//         auto value = sqlite3_column_int64(stmt, 2);
//         auto script = decode_script(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
//         bool is_spent = sqlite3_column_int(stmt, 4) != 0;

//         auto output = std::make_shared<system::chain::output>(value, script);
//         // Note: is_spent is not part of the output object in libbitcoin, but we store it for database purposes

//         return output;
//     }
// };

// }
// }
// }

// #endif // LIBBITCOIN_DATABASE_TABLES_ARCHIVES_OUTPUT_HPP