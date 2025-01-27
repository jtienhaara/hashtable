#include <stdio.h>
#include <malloc.h>  // For malloc(), free().

#include "hashtable.h"


static int INFINITE_LOOP_PROTECTOR = 1024;
static double GOLDEN_RATIO = 1.618033988749;


hashtable_functions_t hashtable_default_functions = {
  hashtable_resize_buckets_default,
  hashtable_get_max_load_default,
  hashtable_get_min_load_default,
  hashtable_hash_default
};
hashtable_functions_t *HASHTABLE_DEFAULT_FUNCTIONS =
    &hashtable_default_functions;


hashtable_error_t hashtable_resize_buckets_default(
        int old_num_buckets,
        int new_num_buckets,
        hashtable_functions_t *functions,
        hashtable_bucket_t ***buckets_pointer
        )
{
  if (buckets_pointer == NULL
      || functions == NULL
      || functions->hash == NULL)
  {
    return HASHTABLE_ERROR_NULL_POINTER;
  }
  else if (new_num_buckets < 0)
  {
    return HASHTABLE_ERROR_NUM_BUCKETS;
  }

  hashtable_bucket_t **new_buckets;
  if (new_num_buckets == 0)
  {
    new_buckets = NULL;
  }
  else
  {
    new_buckets = (hashtable_bucket_t **) malloc(
            (size_t) new_num_buckets * sizeof(hashtable_bucket_t));
    if (new_buckets == NULL)
    {
      return HASHTABLE_ERROR_MALLOC;
    }
  }

  for (int nb = 0; nb < new_num_buckets; nb ++)
  {
    new_buckets[nb] = NULL;
  }

  hashtable_error_t error = HASHTABLE_OK;
  for (int ob = 0; ob < old_num_buckets; ob ++)
  {
    unsigned long x = (*buckets_pointer)[ob]->x;
    void *value = (*buckets_pointer)[ob]->value;
    int hash;
    error = functions->hash(x, new_num_buckets, &hash);
    if (error != HASHTABLE_OK)
    {
      break;
    }
    else if (hash < 0
             || hash >= new_num_buckets)
    {
      error = HASHTABLE_ERROR_HASH_OUT_OF_BOUNDS;
      break;
    }

    hashtable_bucket_t *new_prev_bucket = new_buckets[hash];
    for (int ilp = 0; ilp < INFINITE_LOOP_PROTECTOR; ilp ++)
    {
      if (new_prev_bucket->next == NULL)
      {
        break;
      }

      new_prev_bucket = new_prev_bucket->next;
    }

    if (new_prev_bucket != NULL
        && new_prev_bucket->next != NULL)
    {
      error = HASHTABLE_ERROR_BUCKET_INFINITE_LOOP;
      break;
    }

    hashtable_bucket_t *new_bucket;
    if (new_prev_bucket == NULL)
    {
      new_bucket = new_buckets[hash];
    }
    else
    {
      // Start a chain after the toplevel bucket.
      new_bucket = (hashtable_bucket_t *) malloc(sizeof(hashtable_bucket_t));
      if (new_bucket == NULL)
      {
        error = HASHTABLE_ERROR_MALLOC;
        break;
      }
    }

    error = hashtable_bucket_init(
            new_bucket,
            x,
            hash,
            value,
            new_prev_bucket);
    if (error != HASHTABLE_OK)
    {
      break;
    }
  }

  if (error != HASHTABLE_OK)
  {
    for (int nb = 0; nb < new_num_buckets; nb ++)  // No effect if 0/NULL
    {
      if (new_buckets[nb] != NULL)
      {
        hashtable_bucket_t *bucket_to_free = new_buckets[nb]->next;
        hashtable_bucket_clear(new_buckets[nb]);

        for (int ilp = 0; ilp < INFINITE_LOOP_PROTECTOR; ilp ++)
        {
          if (bucket_to_free == NULL)
          {
            break;
          }

          hashtable_bucket_t *next_bucket_to_free = bucket_to_free->next;
          hashtable_bucket_clear(bucket_to_free);
          free(bucket_to_free);

          bucket_to_free = next_bucket_to_free;
        }
      }
    }

    if (new_buckets != NULL)
    {
      free(new_buckets);
    }

    return error;
  }

  // Now free the old buckets.
  for (int ob = 0; ob < old_num_buckets; ob ++)  // No effect if 0/NULL
  {
    if ((*buckets_pointer)[ob] != NULL)
    {
      hashtable_bucket_t *bucket_to_free = (*buckets_pointer)[ob]->next;
      hashtable_bucket_clear((*buckets_pointer)[ob]);

      for (int ilp = 0; ilp < INFINITE_LOOP_PROTECTOR; ilp ++)
      {
        if (bucket_to_free == NULL)
        {
          break;
        }

        hashtable_bucket_t *next_bucket_to_free = bucket_to_free->next;
        hashtable_bucket_clear(bucket_to_free);
        free(bucket_to_free);

        bucket_to_free = next_bucket_to_free;
      }
    }
  }

  if (*buckets_pointer != NULL)
  {
    free(*buckets_pointer);
  }

  *buckets_pointer = new_buckets;

  return HASHTABLE_OK;
}


hashtable_error_t hashtable_get_max_load_default(
        int num_buckets,
        int *max_load_pointer
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}


hashtable_error_t hashtable_get_min_load_default(
        int num_buckets,
        int max_load,
        int *min_load_pointer
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}


hashtable_error_t hashtable_hash_default(
        unsigned long x,
        int num_buckets,
        int *hash_pointer
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}




hashtable_error_t hashtable_functions_init(
        hashtable_functions_t *self,
        hashtable_error_t (*resize_buckets)(
                int old_num_buckets,
                int new_num_buckets,
                hashtable_functions_t *functions,
                hashtable_bucket_t ***buckets_pointer
                ),
        hashtable_error_t (*get_max_load_or_null)(
                int num_buckets,
                int *max_load_pointer
                ),
        hashtable_error_t (*get_min_load_or_null)(
                int num_buckets,
                int max_load,
                int *min_load_pointer
                ),
        hashtable_error_t (*hash_or_null)(
                unsigned long x,
                int num_buckets,
                int *hash_pointer
                )
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}


hashtable_error_t hashtable_functions_clear(
        hashtable_functions_t *self
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}




hashtable_error_t hashtable_init(
        hashtable_t *self,
        hashtable_functions_t *functions
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}


hashtable_error_t hashtable_clear(
        hashtable_t *self
        )
{
  return HASHTABLE_ERROR_NOT_IMPLEMENTED;
}




hashtable_error_t hashtable_bucket_init(
        hashtable_bucket_t *self,
        unsigned long x,
        int hash,
        void *value,
        hashtable_bucket_t *prev
        )
{
  if (self == NULL)
  {
    return HASHTABLE_ERROR_NULL_POINTER;
  }
  else if (prev != NULL
           && prev->next != NULL)
  {
    return HASHTABLE_ERROR_BUCKET_INSERT;
  }

  self->x = x;
  self->hash = hash;
  self->value = value;

  self->prev = prev;
  self->next = NULL;

  if (self->prev != NULL)
  {
    self->prev->next = self;
  }

  return HASHTABLE_OK;
}


hashtable_error_t hashtable_bucket_clear(
        hashtable_bucket_t *self
        )
{
  if (self == NULL)
  {
    return HASHTABLE_ERROR_NULL_POINTER;
  }

  if (self->prev != NULL)
  {
    self->prev->next = self->next;
  }
  if (self->next != NULL)
  {
    self->next->prev = self->prev;
  }

  self->x = (unsigned long) 0;
  self->hash = 0;
  self->value = NULL;

  self->prev = NULL;
  self->next = NULL;

  return HASHTABLE_OK;
}
