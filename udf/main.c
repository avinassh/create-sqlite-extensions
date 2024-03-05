#include "sqlite3.h"

#include <stdio.h>


const char *greet() {
  return "Hello, World!";
}

static void hello_world(sqlite3_context *context, int argc, sqlite3_value **argv) {
  sqlite3_result_text(context, greet(), -1, SQLITE_TRANSIENT);
}

int main() {
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // register our hello function
  rc = sqlite3_create_function(db, "hello", 0, SQLITE_UTF8, NULL, hello_world, NULL, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "failed to register hello function: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  // run a query that calls our hello function
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, "SELECT hello();", -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "failed to prepare query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    fprintf(stderr, "failed to execute query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  printf("%s\n", sqlite3_column_text(stmt, 0));
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}
