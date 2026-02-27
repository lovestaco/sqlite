#include <stdio.h>
#include "sqlite3.h"

static int callback(void *unused, int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
        printf("%-10s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(void) {
    sqlite3 *db = NULL;
    char *errMsg = NULL;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_exec(db, "CREATE TABLE t1(c1 TEXT);", NULL, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "CREATE TABLE error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    /*
     * c1 is declared TEXT, yet SQLite happily stores an integer.
     * The storage class travels with the value, not the column declaration.
     */
    sqlite3_exec(db, "INSERT INTO t1(c1) VALUES(123);", NULL, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "INSERT error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_exec(db, "INSERT INTO t1(c1) VALUES('abc');", NULL, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "INSERT error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    printf("--- All rows with typeof(c1) ---\n\n");
    sqlite3_exec(db,
        "SELECT c1, typeof(c1) AS type FROM t1;",
        callback, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "SELECT error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return 0;
}
