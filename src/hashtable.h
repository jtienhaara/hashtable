/*
 * hashtable
 * Copyright © 2025-2025 Johann Tienhaara
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

typedef struct _STRUCT_hashtable                  hashtable_t;
typedef enum _ENUM_hashtable_error                hashtable_error_t;
typedef struct _STRUCT_hashtable_bucket           hashtable_bucket_t;
typedef struct _STRUCT_hashtable_functions        hashtable_functions_t;


enum _ENUM_hashtable_error
{
  HASHTABLE_OK = 0,

  HASHTABLE_ERROR_NULL_POINTER,

  HASHTABLE_ERROR_NOT_IMPLEMENTED,  // Functionality that isn't yet implemented.

  HASHTABLE_ERROR_NUM_BUCKETS,  // Invalid # of buckets (e.g. -1).
  HASHTABLE_ERROR_BUCKET_INFINITE_LOOP,  // bucket1->next = bucket1, etc.
  HASHTABLE_ERROR_BUCKET_INSERT,  // Can't insert buckets, only append.
  HASHTABLE_ERROR_HASH_OUT_OF_BOUNDS,  // hash value < 0 or >= num_buckets

  HASHTABLE_ERROR_MALLOC,  // Could not allocate memory
  HASHTABLE_ERROR_FREE,  // Could not free memory

  HASHTABLE_ERROR_MAX
};

struct _STRUCT_hashtable_functions
{
  hashtable_error_t (*resize_buckets)(
          int old_num_buckets,
          int new_num_buckets,
          hashtable_functions_t *functions,
          hashtable_bucket_t ***buckets_pointer
          );
  hashtable_error_t (*get_max_load)(
          int num_buckets,
          int *max_load_pointer
          );
  hashtable_error_t (*get_min_load)(
          int num_buckets,
          int max_load,
          int *min_load_pointer
          );
  hashtable_error_t (*hash)(
          unsigned long x,
          int num_buckets,
          int *hash_pointer
          );
};

extern hashtable_functions_t *HASHTABLE_DEFAULT_FUNCTIONS;

extern hashtable_error_t hashtable_resize_buckets_default(
        int old_num_buckets,
        int new_num_buckets,
        hashtable_functions_t *functions,
        hashtable_bucket_t ***buckets_pointer
        );
extern hashtable_error_t hashtable_get_max_load_default(
        int num_buckets,
        int *max_load_pointer
        );
extern hashtable_error_t hashtable_get_min_load_default(
        int num_buckets,
        int max_load,
        int *min_load_pointer
        );
extern hashtable_error_t hashtable_hash_default(
        unsigned long x,
        int num_buckets,
        int *hash_pointer
        );

extern hashtable_error_t hashtable_functions_init(
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
        );
extern hashtable_error_t hashtable_functions_clear(
        hashtable_functions_t *self
        );


struct _STRUCT_hashtable
{
  int num_buckets;
  int num_entries;
  int min_load;
  int max_load;
  hashtable_bucket_t **buckets;

  hashtable_functions_t *functions;
};

extern hashtable_error_t hashtable_init(
        hashtable_t *self,
        hashtable_functions_t *functions
        );
extern hashtable_error_t hashtable_clear(
        hashtable_t *self
        );


struct _STRUCT_hashtable_bucket
{
  unsigned long x;
  int hash;
  void *value;

  hashtable_bucket_t *prev;
  hashtable_bucket_t *next;
};

extern hashtable_error_t hashtable_bucket_init(
        hashtable_bucket_t *self,
        unsigned long x,
        int hash,
        void *value,
        hashtable_bucket_t *prev
        );
extern hashtable_error_t hashtable_bucket_clear(
        hashtable_bucket_t *self
        );

#endif  // HASHTABLE_H_INCLUDED
