//
// Created by dpr on 29/5/24.
//

#ifndef LIBBITCOIN_DATABASE_SQLITE3_HPP
#define LIBBITCOIN_DATABASE_SQLITE3_HPP

#include <sqlite3.h>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <tuple>


// defined header bindings here!
// write the classes and templates!

// implementation of bindings shall be written in src directory!

namespace db {
    // noncopyable class-template ensures that the object are non-copyable.
    // Object are allowed to be moved (move semantics).
    class noncopyable {
    protected:
        // defining the default constructor and destructor
        noncopyable() = default;
        ~noncopyable() = default;

        // deleted copy constructor and copy assignment operator
        noncopyable(noncopyable const &) = delete;

        noncopyable operator=(noncopyable const &) = delete;

        // move constructor and move assignment operator
        noncopyable(noncopyable &&) = default;

        noncopyable &operator=(noncopyable &&) = default;
    };

}

#endif //LIBBITCOIN_DATABASE_SQLITE3_HPP
