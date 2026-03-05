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

    /* Table without column types (typeless) */
    sqlite3_exec(db, "CREATE TABLE t1(a, b, c);", NULL, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "CREATE TABLE error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    /* Values as SQL literals → storage type by syntax */
    sqlite3_exec(db, "INSERT INTO t1 VALUES('hello', 123, 3.14);", NULL, NULL, &errMsg);
    if (errMsg) { fprintf(stderr, "%s\n", errMsg); sqlite3_free(errMsg); errMsg = NULL; }
    sqlite3_exec(db, "INSERT INTO t1 VALUES(2e5, NULL, X'ABCD');", NULL, NULL, &errMsg);
    if (errMsg) { fprintf(stderr, "%s\n", errMsg); sqlite3_free(errMsg); errMsg = NULL; }

    printf("--- Storage type from SQL literals (typeof) ---\n\n");
    sqlite3_exec(db,
        "SELECT a, typeof(a) AS type_a, b, typeof(b) AS type_b, c, typeof(c) AS type_c FROM t1;",
        callback, NULL, &errMsg);
    if (errMsg) {
        fprintf(stderr, "SELECT error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return 0;
}
