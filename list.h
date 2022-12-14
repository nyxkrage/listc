#ifndef LIST_TYPE
#define LIST_TYPE int
#endif
#define TOKENPASTE(x, y) x ## y
#define LIST_FN(N, T) TOKENPASTE(N ## _list_, T)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "eprintf.h"
#include "memswp.h"

#ifndef LIST_H
#define LIST_H
#define ASSERT_NOT_FREE(r) if (self->capacity == -1) { \
    eprintf("WARN: List has already been free'd\n"); \
    errno = 26; \
    r; \
}
#define push_list(T) LIST_FN(push, T)
#define get_list(T) LIST_FN(get, T)
#define dedup_list(T) LIST_FN(dedup, T)
#define swap_list(T) LIST_FN(swap, T)
#define remove_list(T) LIST_FN(remove, T)
#define with_capacity_list(T) LIST_FN(with_capacity, T)

typedef struct {
  void* data;
  long length;
  long capacity;
} List;

void free_list(List* self) {
  ASSERT_NOT_FREE(return)
  free(self->data);
  self->length = -1;
  self->capacity = -1;
}

long length_list(List* self) {
  return self->length;
}
long capacity_list(List* self) {
  return self->capacity;
}
void clear_list(List* self) {
  ASSERT_NOT_FREE(return)
  free(self->data);
  self->length = 0;
  self->capacity = 0;
}
int is_free_list(List* self) {
  return self->capacity == -1;
}
int is_empty_list(List* self) {
  return self->length == 0;
}
void shrink_list (List* self, long count) {
  self->length -= count;
}
List new_list() {
  return (List){
    .data = (void*)0,
    .length = 0,
    .capacity = 0,
  };
}

#endif
void LIST_FN(push,LIST_TYPE) (List* self, LIST_TYPE data);
LIST_TYPE LIST_FN(get,LIST_TYPE) (List* self, long index);
void LIST_FN(dedup, LIST_TYPE) (List* self, int (*compare)(LIST_TYPE, LIST_TYPE));
void LIST_FN(swap,LIST_TYPE) (List* self, long first_index, long second_index);
void LIST_FN(remove,LIST_TYPE) (List* self, long index);
List LIST_FN(with_capacity,LIST_TYPE) (long capacity);

List LIST_FN(with_capacity,LIST_TYPE) (long capacity) {
  return (List){
    .data = malloc(sizeof(LIST_TYPE) * capacity),
    .length = 0,
    .capacity = capacity,
  };
}

void LIST_FN(push, LIST_TYPE) (List* self, LIST_TYPE data) {
  ASSERT_NOT_FREE(return)
  if (self->length < self->capacity) {
    void* new_data_location = self->data + sizeof(LIST_TYPE) * (self->length);
    *(LIST_TYPE*)new_data_location = data;
    self->length++;
  } else {
    long new_capacity = self->capacity * 2;
    if (new_capacity == 0) {
      new_capacity = 1;
    }
    void* new_data = malloc(sizeof(LIST_TYPE) * (new_capacity + 1));
    memcpy(new_data, self->data, sizeof(LIST_TYPE) * (self->capacity));
    free(self->data);
    self->data = new_data;
    self->capacity = new_capacity;
    long offset = sizeof(LIST_TYPE) * (self->length);
    void* new_data_location = self->data + offset;
    *(LIST_TYPE*)new_data_location = data;
    self->length++;
  }
}

LIST_TYPE LIST_FN(get, LIST_TYPE) (List* self, long index) {
  ASSERT_NOT_FREE(volatile LIST_TYPE* ptr = 0; return *ptr)
  if (index < self->length && index >= 0) {
    return *(LIST_TYPE*)(self->data + sizeof(LIST_TYPE) * index);
  } else {
    eprintf("WARN: Tried to index out of bounds\n");
    errno = 27;
    volatile LIST_TYPE* ptr = 0;
    return *ptr;
  }
}

void LIST_FN(dedup, LIST_TYPE) (List* self, int (*compare)(LIST_TYPE, LIST_TYPE)) {
  if (self->length <= 1) {
    return;
  }
  ASSERT_NOT_FREE(return)

  long count = 0;
  for (long i = 0; i < self->length - count - 1; i++) {
    for (long j = i + 1; j < self->length - count; j++) {
      if (compare(LIST_FN(get, LIST_TYPE)(self,i), LIST_FN(get, LIST_TYPE)(self,j))) {
        if (j != self->length - count) {
          LIST_FN(swap, LIST_TYPE)(self,i,self->length - count);
        }
        count += 1;
      }
    }
  }
  shrink_list(self, count);
}


void LIST_FN(swap,LIST_TYPE) (List* self, long first_index, long second_index) {
  ASSERT_NOT_FREE(return)
  memswp(self->data + sizeof(LIST_TYPE) * first_index, self->data + sizeof(LIST_TYPE) * second_index, sizeof(LIST_TYPE));
}

void LIST_FN(remove,LIST_TYPE) (List* self, long index) {
  ASSERT_NOT_FREE(return)
  for (long i = index; i < self->length; i++) {
    *(LIST_TYPE*)(self->data + sizeof(LIST_TYPE) * i) = *(LIST_TYPE*)(self->data + sizeof(LIST_TYPE) * (i + 1));
  }
  shrink_list(self, 1);
}

List LIST_FN(filter,LIST_TYPE) (List* self, int (*pred)(LIST_TYPE*)) {
  ASSERT_NOT_FREE(volatile List* ptr = 0; return *ptr);
  List new = with_capacity_list(LIST_TYPE)(self->capacity);
  for (int i = 0; i < self->length; i++) {
    if (pred((LIST_TYPE*)(self->data + sizeof(LIST_TYPE) * i))) {
      LIST_FN(push,LIST_TYPE)(&new, LIST_FN(get, LIST_TYPE)(self, i));
    }
  }
  return new;
}
