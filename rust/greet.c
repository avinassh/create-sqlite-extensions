#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#include <assert.h>
#include <stdio.h>

extern void greet(sqlite3_context *context, int argc, sqlite3_value **argv);

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_greet_init(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pApi
) {
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);

  rc = sqlite3_create_function(db, "greet", 1, SQLITE_UTF8, NULL, greet, NULL,
                               NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "error creating function: %s\n", sqlite3_errmsg(db));
    return rc;
  }
  return rc;
}