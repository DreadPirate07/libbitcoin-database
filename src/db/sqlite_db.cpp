//
// Created by dpr on 9/6/24.
//
#include "bitcoin/database-sqlite/wrapper/noncopyable.hpp"
#include "bitcoin/database-sqlite/wrapper/db.hpp"
#include "bitcoin/database-sqlite/wrapper/db_error.hpp"
#include <sqlite3.h>
#include <cstring>
#include <memory>

namespace db
{
    database::database(const char *dbname, int flags, const char *vfs)
    {
        if (dbname)
        {
            int rc = connect(dbname, flags, vfs);
            if (rc != SQLITE_OK)
                throw db_error("Cannot connect to db"); // definition remaining
        }
    }

    database::database(sqlite3 *db) : db_(db), borrowing_(true) {}

    // move semantics for shifting data to new object.
    // for this we empty the old object and place the data under the new object.
    database::database(database &&db) : db_(std::move(db.db_)),
                                        borrowing_(std::move(db.borrowing_)),
                                        bh_(std::move(db.bh_)),
                                        ch_(std::move(db.ch_)),
                                        rh_(std::move(db.rh_)),
                                        uh_(std::move(db.uh_)),
                                        ah_(std::move(db.ah_))
    {
        db.db_ = nullptr;
    }

    database &database::operator=(database &&db)
    {
        db_ = std::move(db.db_);
        db.db_ = nullptr;
        borrowing_ = std::move(db.borrowing_);
        bh_ = std::move(db.bh_);
        ch_ = std::move(db.ch_);
        rh_ = std::move(db.rh_);
        uh_ = std::move(db.uh_);
        ah_ = std::move(db.ah_);

        return *this;
    }
    // database destructor
    database::~database()
    {
        if (!borrowing_)
        {
            disconnect();
        }
    }

    int database::connect(char const *dbname, int flags, char const *vfs)
    {
        if (!borrowing_)
            disconnect();
        return sqlite3_open_v2(dbname, &db_, flags, vfs);
    }

    int database::disconnect()
    {
        int rc = SQLITE_OK;
        if (db_)
        {
            rc = sqlite3_close(db_);
            if (rc = SQLITE_OK)
            {
                db_ = nullptr;
            }
        }
        return rc;
    }
    // to attach two databases
    int database::attach(char const *dbname, char const *name)
    {
        return execf("ATTACH '%q' AS '%q'", dbname, name); // definintion remaining
    }

    // detach databases
    int database::detach(char const *name)
    {
        return execf("DETACH '%q'", name); //definition remaining
    }

    // The method begins by calling sqlite3_backup_init to initialize a backup process.
    // This function takes four parameters: the destination database, the name of the destination database, the source database, and the name of the source database.
    // It returns a pointer to a sqlite3_backup object, which is stored in bkup.
    // If sqlite3_backup_init fails to initialize the backup process (i.e., if bkup is nullptr), the method calls error_code to get the error code of the last SQLite operation on the source database and returns this error code.
    // If the backup process is successfully initialized, the method enters a do-while loop. In each iteration of the loop, it calls sqlite3_backup_step to back up a number of pages specified by step_page.
    // The return value of sqlite3_backup_step is stored in rc.
    // If the backup_handler object h is not nullptr, it is called with three parameters: the number of remaining pages to back up, the total number of pages to back up, and the return code of the last call to sqlite3_backup_step.
    // The loop continues as long as sqlite3_backup_step returns SQLITE_OK, SQLITE_BUSY, or SQLITE_LOCKED.
    // These return codes mean that the backup process is not yet complete, the database file is busy, or the database file is locked, respectively.
    // After the loop, the method calls sqlite3_backup_finish to finalize the backup process and then returns the return code of the last call to sqlite3_backup_step.


    int database::backup(char const* dbname, database& destdb, char const* destdbname, backup_handler h,int step_page){
        sqlite3_backup* bkup = sqlite3_backup_init(destdb.db_,destdbname,db_,dbname);

        if (!bkup) {
            return error_code();
        }

        int rc = SQLITE_OK;
        do 
        {
            rc = sqlite3_backup_step(bkup,step_page);
            if (h) {
                h(sqlite3_backup_remaining(bkup),sqlite3_backup_pagecount(bkup),rc);
            }
        } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);
        sqlite3_backup_finish(bkup);
        return rc;
    }

    int database::backup(database &destdb, backup_handler h)
    {
        return backup("main", destdb, "main", h);
    }

    long long int database::last_insert_row_id() const
    {
        return sqlite3_last_insert_rowid(db_);
    }

    int database::enable_foreign_keys(bool enable)
    {
        return sqlite3_db_config(db_, SQLITE_DBCONFIG_ENABLE_FKEY, enable ? 1 : 0, nullptr);
    }

    int database::enable_triggers(bool enable)
    {
        return sqlite3_db_config(db_, SQLITE_DBCONFIG_ENABLE_TRIGGER, enable ? 1 : 0, nullptr);
    }

    int database::enable_extended_result_codes(bool enable)
    {
        return sqlite3_extended_result_codes(db_, enable ? 1 : 0);
    }

    int database::changes() const
    {
        return sqlite3_changes(db_);
    }

    int database::error_code() const
    {
        return sqlite3_errcode(db_);
    }

    int database::extended_error_code() const
    {
        return sqlite3_extended_errcode(db_);
    }

    char const* database::error_msg() const
    {
        return sqlite3_errmsg(db_);
    }

    int database::exec(char const* sql)
    {
        return sqlite3_exec(db_, sql, 0, 0, 0);
    }

    int database::execf(char const* sql, ...)
    {
        va_list ap;
        va_start(ap, sql);
        std::shared_ptr<char> msql(sqlite3_vmprintf(sql, ap), sqlite3_free);
        va_end(ap);

        return exec(msql.get());
    }

    int database::set_busy_timeout(int ms)
    {
        return sqlite3_busy_timeout(db_, ms);
    }

    void database::set_busy_handler(busy_handler h)
    {
        bh_ = h;
        sqlite3_busy_handler(db_, bh_ ? busy_handler_impl : 0, &bh_);
    }

    void database::set_commit_handler(commit_handler h)
    {
        ch_ = h;
        sqlite3_commit_hook(db_, ch_ ? commit_hook_impl : 0, &ch_);
    }

    void database::set_rollback_handler(rollback_handler h)
    {
        rh_ = h;
        sqlite3_rollback_hook(db_, rh_ ? rollback_hook_impl : 0, &rh_);
    }

    void database::set_update_handler(update_handler h)
    {
        uh_ = h;
        sqlite3_update_hook(db_, uh_ ? update_hook_impl : 0, &uh_);
    }

    void database::set_authorize_handler(authorize_handler h)
    {
        ah_ = h;
        sqlite3_set_authorizer(db_, ah_ ? authorizer_impl : 0, &ah_);
    }


    // implementation for handlers!
    namespace
    {
        int busy_handler_impl(void* p, int cnt)
        {
        auto h = static_cast<database::busy_handler*>(p);
        return (*h)(cnt);
        }

        int commit_hook_impl(void* p)
        {
        auto h = static_cast<database::commit_handler*>(p);
        return (*h)();
        }

        void rollback_hook_impl(void* p)
        {
        auto h = static_cast<database::rollback_handler*>(p);
        (*h)();
        }

        void update_hook_impl(void* p, int opcode, char const* dbname, char const* tablename, long long int rowid)
        {
        auto h = static_cast<database::update_handler*>(p);
        (*h)(opcode, dbname, tablename, rowid);
        }

        int authorizer_impl(void* p, int evcode, char const* p1, char const* p2, char const* dbname, char const* tvname)
        {
        auto h = static_cast<database::authorize_handler*>(p);
        return (*h)(evcode, p1, p2, dbname, tvname);
        }

    }

}