//
// Created by dpr on 29/5/24.
//

#ifndef LIBBITCOIN_DATABASE_STORE_HPP
#define LIBBITCOIN_DATABASE_STORE_HPP

// implementation of store with the help of sqlite3 bindings will be done here!
#include "bitcoin/database/error.hpp"
#include "bitcoin/database-sqlite/wrapper/db.hpp"

namespace libbitcoin {
namespace database {

// template <typename Storage,if_base_of<Storage,storage>=true>
class store {
    // just a basic constructor!
    store();

    code create() NOEXCEPT;

    code open(sqlite::db::database& db);


}




}
}
#endif //LIBBITCOIN_DATABASE_STORE_HPP

#define TEMPLATE template <typename Storage, if_base_of<storage, Storage> If>
#define CLASS store<Storage, If>