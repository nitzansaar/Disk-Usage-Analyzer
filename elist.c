#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "elist.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

bool idx_is_valid(struct elist *list, size_t idx);

/**
 * Creates a new elist using the given size (or the default if set to 0) and
 * element size. 
 */
struct elist *elist_create(size_t list_sz, size_t item_sz)
{
    return NULL;
}

/**
 * Frees memory for the specified elist.
 */
void elist_destroy(struct elist *list)
{

}

/**
 * Sets the capacity for an elist. This can grow or shrink the amount of memory
 * set aside for element storage.
 * 
 * @return -1 on failure, 0 on success
 */
int elist_set_capacity(struct elist *list, size_t capacity)
{
    return -1;
}

/**
 * Retrieves the elist capacity (amount of space set aside for elements in
 * memory).
 */
size_t elist_capacity(struct elist *list)
{
    return 0;
}

/**
 * Adds an item to the end of the list.
 * 
 * @return -1 on failure, 0 on success
 */
ssize_t elist_add(struct elist *list, void *item)
{
    return -1;
}

/**
 * Allocates memory for an item, adds it to the end of the list, and returns a
 * pointer to the newly-allocated memory.
 * 
 * @return -1 on failure, 0 on success
 */

void *elist_add_new(struct elist *list)
{
    return NULL;
}

/**
 * Copies an item into the list at the specified index.
 *
 * @return -1 on failure, 0 on success
 */
int elist_set(struct elist *list, size_t idx, void *item)
{
    return -1;
}

/**
 * Retrieves a particular item from the list based on its index.
 * 
 * @return NULL if the index does not exist or is invalid
 */
void *elist_get(struct elist *list, size_t idx)
{
    return NULL;
}

size_t elist_size(struct elist *list)
{
    return 0;
}

/**
 * Removes an item from the list.
 *
 * @return -1 on failure, 0 on success
 */
int elist_remove(struct elist *list, size_t idx)
{
    return -1;
}

/**
 * "Clears" the array by resetting its size to 0.
 */
void elist_clear(struct elist *list)
{

}

/**
 * Clears the array AND zeroes out its memory.
 */
void elist_clear_mem(struct elist *list)
{

}

/**
 * Sorts the list using the given comparator. This function does not actually
 * implement a sorting algorithm, but simply acts as interface to the qsort
 * function.
 */
void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{

}

bool idx_is_valid(struct elist *list, size_t idx)
{
    return false;
}
