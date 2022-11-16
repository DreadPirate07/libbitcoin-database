/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_POINT_HPP
#define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_POINT_HPP

#include <bitcoin/system.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/primitives/primitives.hpp>
#include <bitcoin/database/tables/schema.hpp>

namespace libbitcoin {
namespace database {
namespace point {

/// Point records are empty, providing only a sk<->fk compression mapping.
/// Each record is 32+4=36 bytes, enabling 4 byte point.hash storage.

struct record
{
    /// Sizes.
    static constexpr size_t pk = schema::tx;
    static constexpr size_t sk = schema::hash;
    static constexpr size_t minsize = zero;
    static constexpr size_t minrow = pk + sk + minsize;
    static constexpr size_t size = minsize;
    static_assert(minsize == 0u);
    static_assert(minrow == 36u);

    static constexpr linkage<pk> count() NOEXCEPT { return 1; }

    /// Fields (none).

    /// Serialializers (nops).

    inline bool from_data(const reader& source) NOEXCEPT
    {
        // debug warning if source non-const, but get_position is non-const.
        ////BC_ASSERT(source.get_position() == minrow);
        return source;
    }

    inline bool to_data(const finalizer& sink) const NOEXCEPT
    {
        // debug warning if sink non-const, but get_position is non-const.
        ////BC_ASSERT(sink.get_position() == minrow);
        return sink;
    }

    inline bool operator==(const record&) const NOEXCEPT
    {
        return true;
    }
};

/// Get search key only.
struct record_sk
{
    static constexpr size_t size = record::size;

    inline bool from_data(reader& source) NOEXCEPT
    {
        source.rewind_bytes(record::sk);
        sk = source.read_hash();
        return source;
    }

    hash_digest sk{};
};

/// point::table
class BCD_API table
  : public hash_map<record>
{
public:
    using hash_map<record>::hashmap;
};

} // namespace point
} // namespace database
} // namespace libbitcoin

#endif