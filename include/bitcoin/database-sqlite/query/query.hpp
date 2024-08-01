//
// Created by dpr on 29/5/24.
//

#ifndef LIBBITCOIN_DATABASE_QUERY_HPP
#define LIBBITCOIN_DATABASE_QUERY_HPP
// implementation of queries will be written in another subdir called impl as done in above memory mapped store

#include "bitcoin/database/query.hpp"
#include "bitcoin/database/tables/schema.hpp"
#include <utility>
#include <bitcoin/system.hpp>
#include <bitcoin/database/association.hpp>
#include <bitcoin/database/associations.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/error.hpp>
#include <bitcoin/database/primitives/primitives.hpp>
#include <bitcoin/database/tables/context.hpp>
#include <bitcoin/database/tables/tables.hpp>
#include <bitcoin/database-sqlite/impl/query/archive.ipp>
#include "bitcoin/database-sqlite/store/store.hpp"

namespace libbitcoin {

    /// Database type aliases.
using height_link = database::table::height::link;
using header_link = database::table::header::link;
using output_link = database::table::output::link;
using input_link  = database::table::input::link;
using point_link  = database::table::point::link;
using spend_link  = database::table::spend::link;
using txs_link    = database::table::txs::link;
using tx_link     = database::table::transaction::link;
using filter_link = database::table::neutrino::link;

using header_links = std_vector<header_link::integer>;
using tx_links     = std_vector<tx_link::integer>;
using input_links  = std_vector<input_link::integer>;
using output_links = std_vector<output_link::integer>;
using spend_links  = std_vector<spend_link::integer>;
using hash_digest = database::hash_digest;
using header_link = header_link;

namespace sqlite {
namespace database {
// template<typename Store>
class query 
{
public:
    inline bool is_header(const hash_digest& key) const NOEXCEPT;
    inline bool is_block(const hash_digest& key) const NOEXCEPT;
    inline bool is_tx(const hash_digest& key) const NOEXCEPT;
    inline bool is_coinbase(const tx_link& link) const NOEXCEPT;
    inline bool is_associated(const header_link& link) const NOEXCEPT;
    inline bool is_milestone(const header_link& link) const NOEXCEPT;
private:
    //Store& store_;
    store Store_;
    bool minimize_;
};
}
}
}
// #define TEMPLATE template <typename Store>
// #define CLASS query<Store>

#endif //LIBBITCOIN_DATABASE_QUERY_HPP
