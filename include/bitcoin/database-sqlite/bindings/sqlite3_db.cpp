//
// Created by dpr on 9/6/24.
//
#include <sqlite3.h>
#include <cstring>
#include "noncopyable.hpp"

namespace db {
    database::database(const char *dbname, int flags, const char *vfs) {
        if (dbname) {
            int rc = connect(dbname,flags,vfs);
            if (rc!=SQLITE_OK) throw db_error("Cannot connect to db");
        }
    }
}