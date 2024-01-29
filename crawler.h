#ifndef CRAWLER_H
#define CRAWLER_H

#include <stddef.h>
#include "hashtable.h"
#include <stdlib.h>
#include <unistd.h>
#include "hashtable.h"
#include "set.h"

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;

typedef struct {
	int size;
	webpage_t **pages;
} bag_t;


/*
typedef struct {
	// Fill in with your implementation
} bag_t;


typedef struct {
	// Fill in with your implementation
} hashtable_t;
*/
#endif
