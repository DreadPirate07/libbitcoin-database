//
// Created by dpr on 29/5/24.
//

#ifndef LIBBITCOIN_DATABASE_QUERY_HPP
#define LIBBITCOIN_DATABASE_QUERY_HPP
// implementation of queries will be written in another subdir called impl as done in above memory mapped store

#include <utility>
#include <bitcoin/system.hpp>
#include <bitcoin/database/association.hpp>
#include <bitcoin/database/associations.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/error.hpp>
#include <bitcoin/database/primitives/primitives.hpp>
#include <bitcoin/database/tables/context.hpp>
#include <bitcoin/database/tables/tables.hpp>


namespace libbitcoin {
namespace database {

/// Database type aliases.
using height_link = table::height::link;
using header_link = table::header::link;
using output_link = table::output::link;
using input_link  = table::input::link;
using point_link  = table::point::link;
using spend_link  = table::spend::link;
using txs_link    = table::txs::link;
using tx_link     = table::transaction::link;
using filter_link = table::neutrino::link;

using header_links = std_vector<header_link::integer>;
using tx_links     = std_vector<tx_link::integer>;
using input_links  = std_vector<input_link::integer>;
using output_links = std_vector<output_link::integer>;
using spend_links  = std_vector<spend_link::integer>;

template<typename Store>
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
    Store& store_;
    bool minimize_;
};
}
}
#define TEMPLATE template <typename Store>
#define CLASS query<Store>


#include <bitcoin/database-sqlite/impl/query/archive.ipp>

#endif //LIBBITCOIN_DATABASE_QUERY_HPP
