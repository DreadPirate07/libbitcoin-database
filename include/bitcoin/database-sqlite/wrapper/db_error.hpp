#ifndef LIBBITCOIN_DATABASE_ERROR_HPP
#define LIBBITCOIN_DATABASE_ERROR_HPP


#include "db.hpp"
#include <stdexcept>

namespace libbitcoin {
namespace sqlite {
namespace database {
namespace db {
    class db_error : public std::runtime_error
    {
    public:
        explicit db_error(char const* msg);
        explicit db_error(database& db);
    };
}
}
}
}
#endif //LIBBITCOIN_DATABASE_ERROR_HPP
