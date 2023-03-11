#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
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
    struct elist *list = malloc(sizeof(struct elist));
    if (list == NULL) {
        free(list);
        return NULL;
    }
    if (list_sz == 0) {
        list_sz = DEFAULT_INIT_SZ;
    }
    list->capacity = list_sz;
    list->size = 0;
    list->item_sz = item_sz;
    list->element_storage = malloc(list_sz * item_sz);
    if (list->element_storage == NULL) {
        free(list);
        return NULL;
    }
    else 
    {
        return list;
    }
}

/**
 * Frees memory for the specified elist.
 */
void elist_destroy(struct elist *list)
{
    free(list->element_storage);
    free(list);
}

/**
 * Sets the capacity for an elist. This can grow or shrink the amount of memory
 * set aside for element storage.
 * 
 * @return -1 on failure, 0 on success
 */
int elist_set_capacity(struct elist *list, size_t capacity)
{
    // capacity is already set to what we want
    if (capacity == list->capacity) {
        return 0;
    }
    // destory old list and create new one with default capacity
    if (capacity == 0) {
        size_t s = list->item_sz;
        elist_destroy(list);
        list = elist_create(DEFAULT_INIT_SZ, s);
        return 0;
    }
    // allocate new memory for new capacity
    void *new_storage = realloc(list->element_storage, capacity * list->item_sz);
    if (new_storage == NULL) {
        return -1;
    }
    list->element_storage = new_storage;
    if (capacity < list->size) {
        list->size = capacity;
    }
    list->capacity = capacity;
    return 0;
}

/**
 * Retrieves the elist capacity (amount of space set aside for elements in
 * memory).
 */
size_t elist_capacity(struct elist *list)
{
    return list->capacity;
}

/**
 * Adds an item to the end of the list.
 * 
 * @return -1 on failure, 0 on success
 */
ssize_t elist_add(struct elist *list, void *item)
{
    if (list->size == list->capacity) {
        list->capacity*=RESIZE_MULTIPLIER;
        void *new_storage = realloc(list->element_storage, list->capacity * list->item_sz);
        if (new_storage == NULL) {
            return -1;
        }
        list->element_storage = new_storage;
    }
    char *dest = (char*)list->element_storage + list->size * list->item_sz;
    memcpy(dest, item, list->item_sz);
    list->size++;
    return 0;
}

/**
 * Allocates memory for an item, adds it to the end of the list, and returns a
 * pointer to the newly-allocated memory.
 * 
 * @return -1 on failure, 0 on success
 */

void *elist_add_new(struct elist *list)
{
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        void *new_storage = realloc(list->element_storage, new_capacity * list->item_sz);
        if (!new_storage) {
            return NULL;
        }
        list->element_storage = new_storage;
        list->capacity = new_capacity;
    }
    void *new_element_storage = (char*)list->element_storage + list->size * list->item_sz;
    memset(new_element_storage, 0, list->item_sz);
    list->size++;
    return new_element_storage;
}

/**
 * Copies an item into the list at the specified index.
 *
 * @return -1 on failure, 0 on success
 */
int elist_set(struct elist *list, size_t idx, void *item)
{
    if (!idx_is_valid(list, idx)) {
        return -1; // out of bounds/invalid
    }
    char *element_storage = (char*)list->element_storage; // ensure arithmetic is done in bytes 
    char *target = element_storage + idx * list->item_sz;
    memcpy(target, item, list->item_sz);
    return 0;
    
}

/**
 * Retrieves a particular item from the list based on its index.
 * 
 * @return NULL if the index does not exist or is invalid
 */
void *elist_get(struct elist *list, size_t idx)
{
    if (!idx_is_valid(list, idx)) {
        return NULL; // out of bounds/invalid
    }
    
    char *element_storage = (char*)list->element_storage;
    void *element = element_storage + idx * list->item_sz;
    
    return element;
}
/**
* @return size of eList
*/
size_t elist_size(struct elist *list)
{
    return list->size;
}

/**
 * Removes an item from the list.
 *
 * @return -1 on failure, 0 on success
 */
int elist_remove(struct elist *list, size_t idx)
{
    if (!idx_is_valid(list, idx)) {
        return -1; // out of bounds/invalid
    }
    char *ptr = (char*)list->element_storage + idx * list->item_sz;
    memmove(ptr, ptr + list->item_sz, (list->size - idx - 1) * list->item_sz);
    list->size--;
    return 0; 
}

/**
 * "Clears" the array by resetting its size to 0.
 */
void elist_clear(struct elist *list)
{
    list->size = 0;
}

/**
 * Clears the array AND zeroes out its memory.
 */
void elist_clear_mem(struct elist *list)
{
    if (list->element_storage != NULL) {
        memset(list->element_storage, 0, list->size * list->item_sz);
    }
    list->size = 0;
}

/**
 * Sorts the list using the given comparator. This function does not actually
 * implement a sorting algorithm, but simply acts as interface to the qsort
 * function.
 */
void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{
    qsort(list->element_storage, list->size, list->item_sz, comparator);
}
/*
* @return false is the idx is out of range or invalid, true otherwise
*/
bool idx_is_valid(struct elist *list, size_t idx)
{
    return idx < list->size;
}


