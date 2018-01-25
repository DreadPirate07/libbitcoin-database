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
#ifndef LIBBITCOIN_DATABASE_SLAB_HASH_TABLE_HPP
#define LIBBITCOIN_DATABASE_SLAB_HASH_TABLE_HPP

#include <cstddef>
#include <cstdint>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/primitives/hash_table_header.hpp>
#include <bitcoin/database/primitives/linked_list.hpp>
#include <bitcoin/database/primitives/slab_manager.hpp>
#include <bitcoin/database/memory/storage.hpp>

namespace libbitcoin {
namespace database {

/**
 * A hash table mapping hashes to variable sized values (slabs).
 * Uses a combination of the hash_table and slab_manager.
 *
 * The hash_table is basically a bucket list containing the start value for the
 * linked_list.
 *
 *  [   size:Index  ]
 *  [ [ item:Link ] ]
 *  [ [    ...    ] ]
 *  [ [ item:Link ] ]
 *
 * The slab_manager is used to create a payload of linked chains. A header
 * containing the hash of the item, and the next value is stored with each
 * slab.
 *
 *   [ key:Key     ]
 *   [ next:Link   ]
 *   [ record:data ]
 *
 * The payload is prefixed with [ size:Link ].
 */
template <typename Manager, typename Key, typename Index, typename Link>
class slab_hash_table
{
public:
    typedef byte_serializer::functor write_function;
    typedef byte_deserializer::functor read_function;
    typedef linked_list<Manager, Link, Key> value_type;
    typedef linked_list<const Manager, Link, Key> const_value_type;
    
    /// Construct a hash table for variable size entries.
    static const Link not_found = hash_table_header<Index, Link>::empty;

    /// Construct a hash table for variable size entries.
    slab_hash_table(storage& file, Index buckets);

    /// Construct a hash table for fixed size entries.
    slab_hash_table(storage& file, Index buckets, size_t value_size);

    /// Create hash table in the file (left in started state).
    bool create();

    /// Verify the size of the hash table in the file.
    bool start();

    /// Commit changes to the hash table.
    void commit();

    /// Use to allocate an element in the hash table. 
    value_type allocator();

    /// Find an element with the given key in the hash table.
    const_value_type find(const Key& key) const;

    /// Get the element with the given link from the hash table.
    const_value_type find(Link link) const;

    /// Add the given element to the hash table.
    void link(value_type& element);

    /// Remove an element with the given key from the hash table.
    bool unlink(const Key& key);

private:
    Link bucket_value(Index index) const;
    Link bucket_value(const Key& key) const;
    Index bucket_index(const Key& key) const;

    hash_table_header<Index, Link> header_;
    Manager manager_;
    mutable shared_mutex root_mutex_;
    mutable shared_mutex list_mutex_;
};

} // namespace database
} // namespace libbitcoin

#include <bitcoin/database/impl/slab_hash_table.ipp>

#endif
