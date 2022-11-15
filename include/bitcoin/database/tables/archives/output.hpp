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
#ifndef LIBBITCOIN_DATABASE_TABLES_ARCHIVES_OUTPUT_HPP
#define LIBBITCOIN_DATABASE_TABLES_ARCHIVES_OUTPUT_HPP

#include <bitcoin/system.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/primitives/primitives.hpp>
#include <bitcoin/database/tables/schema.hpp>

namespace libbitcoin {
namespace database {
namespace output {

BC_PUSH_WARNING(NO_METHOD_HIDING)

// Output is a blob (set of non-searchable slabs).
// Output can be obtained by fk navigation (eg from tx/index).

struct slab
{
    // Sizes.
    static constexpr size_t pk = schema::put;
    ////static constexpr size_t sk = zero;
    static constexpr size_t minsize =
        schema::tx +
        1u + // variable_size (average 1)
        5u + // variable_size (average 5)
        1u;  // variable_size (average 1)
    static constexpr size_t minrow = minsize;
    static constexpr size_t size = minsize;
    static_assert(minsize == 11u);
    static_assert(minrow == 11u);

    linkage<pk> count() const NOEXCEPT
    {
        using out = typename linkage<pk>::integer;
        return system::possible_narrow_cast<out>(
            schema::tx +
            variable_size(index) +
            variable_size(value) +
            script.serialized_size(true));
    }

    // Fields.
    uint32_t parent_fk; // parent fk *not* a required query.
    uint32_t index;     // own (parent-relative) index not a required query.
    uint64_t value;
    system::chain::script script;
    bool valid{ false };

    // Serialializers.

    inline slab from_data(reader& source) NOEXCEPT
    {
        parent_fk = source.read_4_bytes_little_endian();
        index     = system::narrow_cast<uint32_t>(source.read_variable());
        value     = source.read_variable();
        script    = system::chain::script(source, true);
        BC_ASSERT(source.get_position() == count());
        valid = source;
        return *this;
    }

    inline bool to_data(finalizer& sink) const NOEXCEPT
    {
        sink.write_4_bytes_little_endian(parent_fk);
        sink.write_variable(index); // limited to uint32_t (cast on read)
        sink.write_variable(value);
        script.to_data(sink, true);
        BC_ASSERT(sink.get_position() == count());
        return sink;
    }
};

/// output::table
class BCD_API table
  : public array_map<slab>
{
public:
    using array_map<slab>::arraymap;
};

BC_POP_WARNING()

} // namespace output
} // namespace database
} // namespace libbitcoin

#endif
