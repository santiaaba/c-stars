#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista lista_t;

typedef struct lista_node {
	void *data;
	struct lista_node *next;
} lista_node;

struct lista{
	int e_size;
	unsigned int size;
	lista_node *first;
	lista_node *last;
	lista_node *actual;
	pthread_mutex_t lock;
};

void lista_init(lista_t *l, int e_size);
void lista_add(lista_t *l, void *e);
void *lista_get(lista_t *l);
void *lista_get_first(lista_t *l);
void *lista_get_last(lista_t *l);
void *lista_by_index(lista_t *l,int index);
void lista_first(lista_t *l);
void lista_next(lista_t *l);
unsigned int lista_size(lista_t *l);
unsigned int lista_eol(lista_t *l);
void lista_copy(lista_t *l, lista_t *t);
void *lista_remove(lista_t *l);
void lista_clean(lista_t *l,void(*clean)(void**));
void *lista_exclude(lista_t *l,int (*find_id)(void*), int value);
void lista_erase(lista_t *l);
void *lista_find(lista_t *l, int (*find_id)(void*), int value);
void *lista_sort(lista_t *l, float (*get_value)(void*), int asc);

#endif
