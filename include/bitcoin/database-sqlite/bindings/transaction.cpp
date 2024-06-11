#include "transaction.hpp"
#include "db.hpp"
#include "db_error.hpp"
namespace db {
    
    transaction::transaction(database& db, bool fcommit, bool freserve) : db_(&db), fcommit_(fcommit)
    {
        int rc = db_->exec(freserve ? "BEGIN IMMEDIATE" : "BEGIN");
        if (rc != SQLITE_OK)
        throw db_error(*db_);
    }

    transaction::~transaction()
    {
        if (db_) {
        // exec() can return error. If you want to check the error,
        // shall call commit() or rollback() explicitly before this object is

        db_->exec(fcommit_ ? "COMMIT" : "ROLLBACK");
        }
    }

    int transaction::commit()
    {
        auto db = db_;
        db_ = nullptr;
        int rc = db->exec("COMMIT");
        return rc;
    }

    int transaction::rollback()
    {
        auto db = db_;
        db_ = nullptr;
        int rc = db->exec("ROLLBACK");
        return rc;
    }
}

