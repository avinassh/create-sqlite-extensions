#include <sqlite3.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  char *err_msg = 0;
  char *result;

  printf("SQLite version: %s\n", sqlite3_libversion());

  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  rc = sqlite3_enable_load_extension(db, 1);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot enable load extension: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return rc;
  }

  rc = sqlite3_load_extension(db, "./greet", "sqlite3_greet_init", &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to load extension: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return rc;
  }

  sqlite3_prepare_v2(db, "SELECT greet('World');", -1, &stmt, NULL);
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    result = (char *)sqlite3_column_text(stmt, 0);
    printf("Result: %s\n", result);
  } else {
    fprintf(stderr, "Error calling greet function: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_finalize(stmt);

  sqlite3_close(db);
  return 0;
}