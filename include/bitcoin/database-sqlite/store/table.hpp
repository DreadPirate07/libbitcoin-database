/**
 * Copyright (c) 2011-2023 libbitcoin developers (see AUTHORS)
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

// probably we don't need that old table object!

#ifndef LIBBITCOIN_DATABASE_TABLES_TABLE_HPP
#define LIBBITCOIN_DATABASE_TABLES_TABLE_HPP

namespace libbitcoin {
namespace database {

enum class table_t
{
    store,

    /// Archives.
    header_table,
    input_table,
    output_table,
    point_table,
    puts_table,
    spend_table,
    tx_table,
    txs_table, // transactions table

    /// Indexes.
    candidate_table,
    confirmed_table,
    strong_tx_table,

    /// Caches.
    validated_bk_table,
    validated_tx_table,

    /// Optionals.
    address_table,
    neutrino_table,
};

} // namespace database
} // namespace libbitcoin

#endif
