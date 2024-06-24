#ifndef VECTORSPE_H
#define VECTORSPE_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// Macro pour déclarer une structure de vecteur pour un type donné
#define VECTOR_DECLARATION(TYPE)                           \
typedef struct {                                           \
    TYPE *data;                                            \
    size_t size;                                           \
    size_t capacity;                                       \
    void (*free_elem)(TYPE);                              \
} Vector_##TYPE;                                           \
                                                           \
Vector_##TYPE vector_##TYPE##_init(size_t initial_capacity, void (*free_elem)(TYPE)); \
void vector_##TYPE##_add(Vector_##TYPE *vec, TYPE value);  \
void vector_##TYPE##_remove(Vector_##TYPE *vec, size_t index); \
void vector_##TYPE##_free(Vector_##TYPE *vec);

// Macro pour définir les fonctions d'un vecteur pour un type donné
#define VECTOR_DEFINITION(TYPE)                                                        \
Vector_##TYPE vector_##TYPE##_init(size_t initial_capacity, void (*free_elem)(TYPE)) { \
    Vector_##TYPE vec;                                                                 \
    vec.data = (TYPE*)malloc(initial_capacity * sizeof(TYPE));                         \
    if (!vec.data) {                                                                   \
        perror("Failed to allocate vector data");                                      \
        exit(EXIT_FAILURE);                                                            \
    }                                                                                  \
    vec.size = 0;                                                                      \
    vec.capacity = initial_capacity;                                                   \
    vec.free_elem = free_elem;                                                         \
    return vec;                                                                        \
}                                                                                      \
                                                                                       \
void vector_##TYPE##_add(Vector_##TYPE *vec, TYPE value) {                             \
    if (vec->size >= vec->capacity) {                                                  \
        vec->capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;                  \
        vec->data = (TYPE*)realloc(vec->data, vec->capacity * sizeof(TYPE));           \
        if (!vec->data) {                                                              \
            perror("Failed to reallocate vector data");                                \
            exit(EXIT_FAILURE);                                                        \
        }                                                                              \
    }                                                                                  \
    vec->data[vec->size++] = value;                                                    \
}                                                                                      \
                                                                                       \
void vector_##TYPE##_remove(Vector_##TYPE *vec, size_t index) {                        \
    if (index >= vec->size) {                                                          \
        return; /* Index out of bounds */                                              \
    }                                                                                  \
    vec->data[index] = vec->data[vec->size - 1];                                       \
    vec->size--;                                                                       \
    if (vec->size <= vec->capacity / 3) {                                              \
        vec->capacity /= 2;                                                            \
        vec->data = (TYPE*)realloc(vec->data, vec->capacity * sizeof(TYPE));           \
        if (!vec->data) {                                                              \
            perror("Failed to reallocate vector data");                                \
            exit(EXIT_FAILURE);                                                        \
        }                                                                              \
    }                                                                                  \
}                                                                                      \
                                                                                       \
void vector_##TYPE##_free(Vector_##TYPE *vec) {                                        \
    if (vec->free_elem) {                                                              \
        for (size_t i = 0; i < vec->size; i++) {                                       \
            vec->free_elem(vec->data[i]);                                             \
        }                                                                              \
    }                                                                                  \
    free(vec->data);                                                                   \
}

#endif // VECTORSPE_H