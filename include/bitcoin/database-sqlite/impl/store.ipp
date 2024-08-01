#ifndef LIBBITCOIN_DATABASE_STORE_IPP
#define LIBBITCOIN_DATABASE_STORE_IPP


#include "bitcoin/database-sqlite/store/schema.hpp"
#include "bitcoin/database-sqlite/store/store.hpp"
#include "bitcoin/database-sqlite/wrapper/db.hpp"
#include "bitcoin/database-sqlite/wrapper/db_error.hpp"
#include "bitcoin/database-sqlite/wrapper/noncopyable.hpp"
#include <bitcoin/system/error/error_t.hpp>


namespace libbitcoin {
namespace sqlite {
namespace database {

store::store() = default;
store::~store() = default;

system::code store::create(const std::string& db_path) NOEXCEPT
{
    try
    {
        db_ = std::make_unique<db::database>(db_path.c_str());

        header_t = std::make_unique<table::header>(*db_);
        if (!header_t->create_table())
            return system::error::error_t::not_found;

        return system::error::success;
    }
    catch (const std::exception&)
    {
        return system::error::error_t::not_found;
    }
}

system::code store::open(const std::string& db_path) NOEXCEPT
{
    try
    {
        header_t = std::make_unique<table::header>(*db_);

        // Initialize other table objects as needed

        return system::error::error_t::not_found;
    }
    catch (const std::exception&)
    {
        return system::error::error_t::not_found;
    }
}

system::code store::close()
{
    try
    {
        if (db_)
        {
            db_->disconnect();
            db_.reset();
        }
        header_t.reset();
        // Reset other table objects

        return system::error::success;
    }
    catch (const std::exception&)
    {
        return system::error::error_t::not_found;
    }
}

// table::header& store::headers()
// {
//     return *header_t;
// }


}
}
}

#endif LIBBITCOIN_DATABASE_STORE_IPP