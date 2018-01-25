/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
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
#include <bitcoin/database/databases/spend_database.hpp>

#include <cstddef>
#include <tuple>
#include <utility>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/database/memory/memory.hpp>

// Record format (v3):
// ----------------------------------------------------------------------------
// [ point-hash:32 - const]
// [ point-index:2 - const]

namespace libbitcoin {
namespace database {

using namespace bc::chain;

// Spends use a hash table index, O(1).
// The spend database keys off of output point and has input point value.
spend_database::spend_database(const path& filename, size_t buckets,
    size_t expansion)
  : hash_table_file_(filename, expansion),
    hash_table_(hash_table_file_, buckets,
        std::tuple_size</*point*/ short_hash>::value)
{
}

spend_database::~spend_database()
{
    close();
}

// Startup and shutdown.
// ----------------------------------------------------------------------------

bool spend_database::create()
{
    if (!hash_table_file_.open())
        return false;

    // No need to call open after create.
    return
        hash_table_.create();
}

bool spend_database::open()
{
    return
        hash_table_file_.open() &&
        hash_table_.start();
}

void spend_database::commit()
{
    hash_table_.commit();
}

bool spend_database::flush() const
{
    return hash_table_file_.flush();
}

bool spend_database::close()
{
    return hash_table_file_.close();
}

// Queries.
// ----------------------------------------------------------------------------

input_point spend_database::get(const output_point& outpoint) const
{
    input_point spend;
    ////auto element = hash_table_.find(outpoint);

    ////if (!element)
    ////    return spend;

    ////const auto reader = [&](byte_deserializer& deserial)
    ////{
    ////    spend.from_data(deserial, false);
    ////};

    ////element.read(reader);
    return spend;
}

// Store.
// ----------------------------------------------------------------------------

void spend_database::store(const chain::output_point& outpoint,
    const chain::input_point& spend)
{
    ////const auto writer = [&](byte_serializer& serial)
    ////{
    ////    spend.to_data(serial, false);
    ////};

    // Write the new spend.
    auto front = hash_table_.allocator();
    ////front.create(outpoint, writer);
    hash_table_.link(front);
}

// Update.
// ----------------------------------------------------------------------------

bool spend_database::unlink(const output_point& outpoint)
{
    return hash_table_.unlink({});
}

} // namespace database
} // namespace libbitcoin
