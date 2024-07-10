#ifndef LIBBITCOIN_DATABASE_SQLITE_STRUCTURES_HPP
#define LIBBITCOIN_DATABASE_SQLITE_STRUCTURES_HPP

#include <bitcoin/system.hpp>
#include <cstdint>
#include <vector>
#include <string>

namespace libbitcoin
{
    namespace database
    {
        // might be of use!
        using Hash = std::string;
        using Script = std::string;
        using Witness = std::string;

        namespace schema
        {       
            // static so that variables can be accessed directly without instantiating the object.
            // the members belong to the class itself and not the object!
            // constexpr indicates that the values shall be computed at compile time.
            // this makes sure that the values exist in read only memory!
            // together they make the member a global constant which can be accessed from anywhere!
            // this is important as these values will be used for compile-time arguments, such as 
            // template arguments or array sizes!
            // R defines a raw string literal in C++;
            // sql often contains queries that contain / or "", that's why we've used R (raw string literal) keyword.

            struct header
            {       // is_candidate INTEGER,
                    // is_confirmed INTEGER
                static constexpr auto table_name = "headers";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS headers (
                    hash TEXT PRIMARY KEY,
                    version INTEGER NOT NULL,
                    previous_block_hash TEXT NOT NULL,
                    merkle_root TEXT NOT NULL,
                    timestamp INTEGER NOT NULL,
                    bits INTEGER NOT NULL,
                    nonce INTEGER NOT NULL,
                    height INTEGER UNIQUE NOT NULL,
                    flags INTEGER NOT NULL,
                    mtp INTEGER NOT NULL
                    )
                )";
            };

            // we use single table for both input and output
            // which are differntiated by it's type!
            // this approach simplifies the queries which need to look for both input 
            // and output
            struct puts 
            {
                static constexpr auto table_name = "puts";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS puts (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    transaction_hash TEXT NOT NULL,
                    put_fk TEXT NOT NULL,
                    is_input INTEGER NOT NULL,
                    FOREIGN KEY (transaction_hash) REFERENCES transactions(hash)
                    )
                )";
            };

            struct transaction {
                static constexpr auto table_name = "transactions";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS transactions (
                        hash TEXT PRIMARY KEY,
                        coinbase TEXT,
                        size INTEGER NOT NULL,
                        weight INTEGER NOT NULL,
                        locktime INTEGER NOT NULL,
                        version INTEGER NOT NULL,
                        input_count INTEGER NOT NULL,
                        input_fk TEXT,
                        output_count INTEGER NOT NULL,
                        output_fk TEXT
                    )
                
                )";
            };
            
            struct txs {
                static constexpr auto table_name = "txs";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS txs (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        header_hash TEXT NOT NULL,
                        transaction_hash TEXT NOT NULL,
                        FOREIGN KEY (transaction_hash) REFERENCES transactions(hash),
                        FOREIGN KEY (header_hash) REFERENCES header(hash)
                    )
                )";
            };

            struct input {
                static constexpr auto table_name = "inputs";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS inputs (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        transaction_hash TEXT NOT NULL,
                        index INTEGER NOT NULL,
                        point_fk TEXT NOT NULL,
                        sequence INTEGER NOT NULL,
                        script TEXT NOT NULL,
                        witness TEXT,
                        FOREIGN KEY (transaction_hash) REFERENCES transactions(hash),
                        FOREIGN KEY (point_fk) REFERENCES points(hash)
                    )
                )";
            };

            struct output {
                static constexpr auto table_name = "outputs";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS outputs (
                        id INTEGER PRIMARY KEY AUTOINCREMENT,
                        transaction_hash TEXT NOT NULL,
                        index INTEGER NOT NULL,
                        value INTEGER NOT NULL,
                        script TEXT NOT NULL,
                        FOREIGN KEY (transaction_hash) REFERENCES transactions(hash)
                    )
                )";
            };

            struct point {
                static constexpr auto table_name = "points";
                static constexpr auto create_table = R"(
                    CREATE TABLE IF NOT EXISTS points (
                        hash TEXT PRIMARY KEY
                    )
                )";
            };
        };
    };
}

#endif