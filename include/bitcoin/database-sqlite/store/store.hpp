//
// Created by dpr on 29/5/24.
//

#ifndef LIBBITCOIN_DATABASE_STORE_HPP
#define LIBBITCOIN_DATABASE_STORE_HPP

// implementation of store with the help of sqlite3 bindings will be done here!
#include "bitcoin/database-sqlite/store/tables/archives/header.hpp"
#include "bitcoin/database/error.hpp"
#include "bitcoin/database-sqlite/wrapper/db.hpp"
#include <cstddef>
#include <memory>

// using header_db = libbitcoin::sqlite::database::table::header;


namespace libbitcoin {
namespace sqlite {
namespace database {

// template <typename Storage,if_base_of<Storage,storage>=true>
class store {
public:
    // just a basic constructor!
    store();
    ~store();

    system::code create(const std::string& db_path) NOEXCEPT;

    system::code open(const std::string& db_path) NOEXCEPT;

    system::code close();

private:
    std::unique_ptr<db::database> db_;
    std::unique_ptr<table::header> header_t;
};
}
}
}

#endif //LIBBITCOIN_DATABASE_STORE_HPP

#define TEMPLATE template <typename Storage, if_base_of<storage, Storage> If>
#define CLASS store<Storage, If>