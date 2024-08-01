
#ifndef LIBBITCOIN_DATABASE_TRANSACTION_HPP
#define LIBBITCOIN_DATABASE_TRANSACTION_HPP

#include "db.hpp"
namespace libbitcoin {
namespace sqlite {
namespace database {
namespace db {
    class transaction : noncopyable
    {
    public:
        explicit transaction(database& db, bool fcommit = false, bool freserve = false);
        ~transaction();

        int commit();
        int rollback();

    private:
        database* db_;
        bool fcommit_;
    };
}
}
}
}
#endif