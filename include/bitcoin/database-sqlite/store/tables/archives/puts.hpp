// #ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_PUTS_HPP
// #define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_PUTS_HPP

// #include "include/bitcoin/database-sqlite/store/table.hpp"
// #include "include/bitcoin/database-sqlite/store/schema.hpp"
// #include "include/bitcoin/database-sqlite/wrapper/db.hpp"
// #include <bitcoin/system.hpp>

// namespace libbitcoin {
// namespace database {
// namespace table {

// struct puts : public table<schema::puts> {

//     using spend = linkage<schema::spend_>;
//     using out = linkage<schema::put>;
//     using spend_links = std_vector<spend::integer>;
//     using output_links = std_vector<out::integer>;

//     inline bool create_table(db::database& db)
//     {
//         return db.exec(schema::puts::create_table) == SQLITE_OK;
//     }

//     inline bool insert(db::database& db, const spend_links& spend_fks, const output_links& out_fks)
//     {
//         const char* sql = R"(
//             INSERT INTO puts (spend_fks, out_fks)
//             VALUES (?, ?)
//         )";

//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, encode_links(spend_fks));
//         stmt.bind(2, encode_links(out_fks));

//         return stmt.finalize() == SQLITE_DONE;
//     }

//     inline bool get(db::database& db, linkage<schema::puts>::integer link, spend_links& spend_fks, output_links& out_fks)
//     {
//         const char* sql = "SELECT spend_fks, out_fks FROM puts WHERE rowid = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return false;

//         stmt.bind(1, link);

//         if (stmt.step() != SQLITE_ROW) return false;

//         spend_fks = decode_links<spend::integer>(reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0)));
//         out_fks = decode_links<out::integer>(reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1)));

//         return true;
//     }

//     inline spend_links get_spends(db::database& db, linkage<schema::puts>::integer link)
//     {
//         const char* sql = "SELECT spend_fks FROM puts WHERE rowid = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return {};

//         stmt.bind(1, link);

//         if (stmt.step() != SQLITE_ROW) return {};

//         return decode_links<spend::integer>(reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0)));
//     }

//     inline output_links get_outs(db::database& db, linkage<schema::puts>::integer link)
//     {
//         const char* sql = "SELECT out_fks FROM puts WHERE rowid = ?";
        
//         db::statement stmt(db);
//         auto ec = stmt.prepare(sql);
//         if (!ec) return {};

//         stmt.bind(1, link);

//         if (stmt.step() != SQLITE_ROW) return {};

//         return decode_links<out::integer>(reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0)));
//     }

//     inline spend::integer get_spend_at(db::database& db, linkage<schema::puts>::integer link, size_t index)
//     {
//         auto spends = get_spends(db, link);
//         return index < spends.size() ? spends[index] : spend::integer{};
//     }

//     inline out::integer get_output_at(db::database& db, linkage<schema::puts>::integer link, size_t index)
//     {
//         auto outputs = get_outs(db, link);
//         return index < outputs.size() ? outputs[index] : out::integer{};
//     }

// private:
//     template<typename T>
//     inline std::string encode_links(const std_vector<T>& links)
//     {
//         std::stringstream ss;
//         for (const auto& link : links)
//         {
//             ss << link << ",";
//         }
//         return ss.str();
//     }

//     template<typename T>
//     inline std_vector<T> decode_links(const std::string& encoded)
//     {
//         std_vector<T> links;
//         std::stringstream ss(encoded);
//         std::string item;
//         while (std::getline(ss, item, ','))
//         {
//             if (!item.empty())
//             {
//                 links.push_back(static_cast<T>(std::stoull(item)));
//             }
//         }
//         return links;
//     }
// };

// }
// } 
// }

// #endif // LIBBITCOIN_DATABASE_TABLES_ARCHIVES_PUTS_HPP