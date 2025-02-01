#include <stdio.h>

#include "hashtable.h"

static hashtable_error_t test_hashtable_resize_buckets_default()
{
  printf("  Testing hashtable_resize_buckets_default():\n");
  fflush(stdout);

  hashtable_error_t error = HASHTABLE_OK;

  // -------------------------------------------------------------------
  printf("    NULL -> NULL buckets:\n");
  int old_buckets = 0;
  int new_buckets = 0;
  hashtable_bucket_t **buckets = NULL;
  error = hashtable_resize_buckets_default(
      old_buckets,
      new_buckets,
      HASHTABLE_DEFAULT_FUNCTIONS,
      &buckets);  // buckets_pointer

  if (error == HASHTABLE_OK)
  {
    printf("OK\n");
    fflush(stdout);
  }
  else
  {
    printf("FAILED\n");
    fflush(stdout);
  }

  return error;
}


int main(int argc, char *argv[])
{
  printf("Testing hashtable...\n");  fflush(stdout);

  // Test reading in an ASCii string, with \r\n fake single character:
  hashtable_error_t error = test_hashtable_resize_buckets_default();

  // !!! // Test !!!
  // !!! if (error == HASHTABLE_OK)
  // !!! {
  // !!!   error = test_!!!();
  // !!! } // else if (error != HASHTABLE_OK) then fall through, below.

  if (error == HASHTABLE_OK)
  {
    printf("SUCCESS Testing hashtable.\n");  fflush(stdout);
    fflush(stdout);
    fflush(stderr);
    return 0;
  }
  else
  {
    fprintf(stderr,
            "ERROR test_hashtable: Failed with error code: %d\n",
            (int) error);
    return (int) error;
  }
}
