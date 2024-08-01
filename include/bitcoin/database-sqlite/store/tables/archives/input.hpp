// #ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_INPUT_HPP
// #define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_INPUT_HPP

// #include "include/bitcoin/database-sqlite/store/table.hpp"
// #include "include/bitcoin/database-sqlite/store/schema.hpp"
// #include "include/bitcoin/database-sqlite/wrapper/db.hpp"
// #include <bitcoin/system.hpp>

// namespace libbitcoin {
// namespace database {
// namespace table {

// struct input : public table<schema::input> {

//     inline bool create_table(db::database& db)
//     {
//         return db.exec(schema::input::create_table) == SQLITE_OK;
//     }

//     inline bool insert(db::database& db, const system::chain::input& input, const system::hash_digest& tx_hash, uint32_t index)
//     {
//         const char* sql = R"(
//             INSERT OR REPLACE INTO inputs
//             (tx_hash, index, previous_output_hash, previous_output_index, script, sequence)
//             VALUES (?, ?, ?, ?, ?, ?)
//         )";

//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1,encode_hash(tx_hash));
//         stmt.bind(2,index);
//         stmt.bind(3,encode_hash(input.point().hash()));
//         stmt.bind(4,input.point().index());
//         stmt.bind(5,encode_script(input.script()));
//         stmt.bind(6,input.sequence());

//         return stmt.finalize() == SQLITE_DONE;
//     }
    
//     inline system::chain::input::cptr get(db::database& db,const system::hash_digest& tx_hash,uint32_t index) 
//     {
//         const char* sql = R"(
//             SELECT * FROM inputs WHERE tx_hash = ? && INDEX = ?"
//             )";
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1, encode_hash(tx_hash));
//         stmt.bind(2, index);

//         if (stmt.step() != SQLITE_OK) return nullptr;

//         return extract_output(stmt.get());
//     }

//     inline system::chain::inputs_ptr get_for_trasactions(db::database& db,const system::hash_digest& hash)
//     {
//         const char* sql = "SELECT * FROM inputs WHERE tx_hash = ? ORDER BY index";

//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return nullptr;

//         stmt.bind(1,hash);

//         auto inputs = std::make_shared<system::chain::input_cptrs>();
//         while (stmt.step() == SQLITE_ROW) 
//         {
//             inputs->push_back(extract_inputs(stmt));
//         }

//         return inputs;
//     }

// private:
//     inline std::string encode_hash(const system::hash_digest& hash)
//     {
//         return system::encode_base16(hash);
//     }

//     inline system::hash_digest decode_hash(const std::string& str) 
//     {
//         system::hash_digest hash;
//         system::decode_base16(hash,str);
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
//         return system::chain::script::factory_from_data(script_data, false);
//     }

//     inline std::string encode_witness(const system::chain::witness& witness) 
//     {
//         return system::encode_base16(witness.to_string());
//     }

//     inline system::chain::witness decode_witness(const std::string& witness_str)
//     {
//         return system::chain::witness::factory_from_string(witness_str);
//     }

//     inline system::chain::input::cptr extract_input(sqlite3_stmt* stmt)
//     {
//         system::chain::point point;
//         point.set_hash(decode_hash(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
//         point.set_index(sqlite3_column_int(stmt, 3));

//         auto script = decode_script(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
//         auto witness = decode_witness(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
//         auto sequence = sqlite3_column_int(stmt, 6);

//         return std::make_shared<system::chain::input>(
//             std::make_shared<system::chain::point>(point),
//             std::make_shared<system::chain::script>(script),
//             std::make_shared<system::chain::witness>(witness),
//             sequence
//         );
//     }

// };
// }
// }
// }

// #endif