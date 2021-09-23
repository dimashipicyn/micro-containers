#ifndef VECTOR_H
#define VECTOR_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "object.h"

typedef int* int_ptr_t;
typedef char* char_ptr_t;
typedef double* double_ptr_t;
typedef float* float_ptr_t;
typedef void* void_ptr_t;

// Type of vector
#define Vector(type) CONCAT(Vector, type)
#define Iterator(type) CONCAT(Iterator, type)

// container methods
#define m_insert(obj, elem, pos) obj->vtable->insert(obj, elem, pos)
#define m_size(obj) obj->vtable->size(obj)
#define m_push_back(obj, elem) obj->vtable->push_back(obj, elem)
#define m_push_front(obj, elem) obj->vtable->push_front(obj, elem)
#define m_erase(obj, pos) obj->vtable->erase(obj, pos)
#define m_clear(obj) obj->vtable->clear(obj)
#define m_at(obj, pos) obj->vtable->at(obj, pos)
#define m_load(obj, mem, size) obj->vtable->load(obj, mem, size)
#define m_add_mem(obj, mem, size) obj->vtable->add_mem(obj, mem, size)

// iterator methods
#define m_has_next(iterator) iterator->container->vtable->has_next(iterator)
#define m_next(iterator) iterator->container->vtable->next(iterator)
#define m_prev(iterator) iterator->container->vtable->prev(iterator)
#define m_get(iterator) iterator->container->vtable->get(iterator)
#define m_remove(iterator) iterator->container->vtable->remove(iterator)
#define m_add(iterator, elem) iterator->container->vtable->add(iterator, elem)

#define T int
#include "vector.h"

#define T char
#include "vector.h"

#define T double
#include "vector.h"

#define T float
#include "vector.h"

#define T int_ptr_t
#include "vector.h"

#define T char_ptr_t
#include "vector.h"

#define T double_ptr_t
#include "vector.h"

#define T float_ptr_t
#include "vector.h"

#define T void_ptr_t
#include "vector.h"

#endif

/**
 * Многократно включаемая часть
 * для генерации функций и типов
 * 
 */


#ifdef T

/**
 * Шаблоны структур и типов которые генерирует препроцессор
 */

typedef struct	CONCAT(Vector, T) CONCAT(Vector, T);
typedef struct	CONCAT(Iterator, T) CONCAT(Iterator, T);

typedef struct CONCAT(s_vtable, T)
{
	bool	(*insert)(void *, T, int);
	int		(*size)(void *);
	bool	(*push_back)(void *, T);
	bool	(*push_front)(void *, T);
	bool	(*erase)(void *, int);
	void	(*clear)(void *);
	T		(*at)(void *, int);
	bool	(*load)(void *, void *, size_t);
	bool	(*add_mem)(void *, void *, int);
	bool	(*has_next)(struct CONCAT(Iterator, T) *);
	T		(*next)(struct CONCAT(Iterator, T) *);
	void    (*prev)(struct CONCAT(Iterator, T) *);
	T		(*get)(struct CONCAT(Iterator, T) *);
	void	(*remove)(struct CONCAT(Iterator, T) *);
	void	(*add)(struct CONCAT(Iterator, T) *, T);
}				CONCAT(t_vtable, T);

typedef struct CONCAT(Vector, T)
{
	Class				*class;
	CONCAT(t_vtable, T)	*vtable;
	T			*mem;
	int			size;
	int			capacity;
}				CONCAT(Vector, T);

typedef struct	CONCAT(Iterator, T)
{
	Class						*class;
	CONCAT(Vector, T)			*container;
	int							iter;
}	CONCAT(Iterator, T);

// constructor vector declare
CONCAT(Vector, T)	*CONCAT(constructor_Vector, T)(void *v, ...);

// constructor iterator declare
void	*CONCAT(constructor_Iterator, T)(void *this, void *vec);


/**************************************************************************/
/**************************************************************************/
/***************************************************************************/

#ifdef template

/**
 * Шаблоны прототипов встроенных функций
 */

static void			CONCAT(destructor, T)(void *obj);
static void			*CONCAT(clone, T)(void *_self);
static int			CONCAT(equal, T)(void *_self, void *_other);
static unsigned int	CONCAT(vector_hash, T)(void *_self);
static bool			CONCAT(erase, T)(void *this, int position);
static int			CONCAT(size, T)(void *this);
static bool			CONCAT(push_front, T)(void *this, T);
static bool			CONCAT(push_back, T)(void *this, T);
static bool			CONCAT(insert, T)(void *this, T, int position);
static T			CONCAT(at, T)(void *this, int elem);
static void			CONCAT(clear, T)(void *this);
static bool			CONCAT(load, T)(void *this, void *mem, size_t n);
static bool			CONCAT(add_mem, T)(void *this, void *mem, int size);
static bool			CONCAT(has_next, T)(CONCAT(Iterator, T) *t);
static T			CONCAT(next, T)(CONCAT(Iterator, T) *t);
static void			CONCAT(prev, T)(CONCAT(Iterator, T) *t);
static T	        CONCAT(get, T)(CONCAT(Iterator, T) *t);
void				CONCAT(addIterator, T)(CONCAT(Iterator, T) *t, T elem);
void				CONCAT(removeIterator, T)(CONCAT(Iterator, T) *t);

static Class	CONCAT(g_class, T) =
{
	.destructor = CONCAT(destructor, T),
	.clone = CONCAT(clone, T),
	.equal = CONCAT(equal, T),
	.hash = CONCAT(vector_hash, T)
};

static CONCAT(t_vtable, T)	CONCAT(g_vtable, T) =
{
	.insert = CONCAT(insert, T),
	.size = CONCAT(size, T),
	.push_back = CONCAT(push_back, T),
	.push_front = CONCAT(push_front, T),
	.erase = CONCAT(erase, T),
	.clear = CONCAT(clear, T),
	.at = CONCAT(at, T),
	.load = CONCAT(load, T),
	.add_mem = CONCAT(add_mem, T),
	.has_next = CONCAT(has_next, T),
	.next = CONCAT(next, T),
	.prev = CONCAT(prev, T),
	.get = CONCAT(get, T),
	.remove = CONCAT(removeIterator, T),
	.add = CONCAT(addIterator, T)
};


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

/**
 * Конструктор
 */

CONCAT(Vector, T)	*CONCAT(constructor_Vector, T)(void *v, ...)
{
	CONCAT(Vector, T) *vec;
	T					*mem;

	vec = v;
	mem = calloc(1, sizeof(T));
	if (!mem)
	{
		free(vec);
		free(mem);
		return (0);
	}
	vec->mem = mem;
	vec->capacity = 1;
	vec->class = &CONCAT(g_class, T);
	vec->vtable = &CONCAT(g_vtable, T);
	return (vec);
}

static void	CONCAT(destructor, T)(void *obj)
{
	CONCAT(Vector, T) *t;
	
	t = obj;
	free(t->mem);
}

void *CONCAT(clone, T)(void *_self)
{
	Vector(T)	*self = _self;
	Vector(T)	*clone = new(Vector(T));
	T			*mem = calloc(self->size + 1, sizeof(T));
	
	if (!clone || !mem) {
		delete(clone);
		free(mem);
		return 0;
	}
	memcpy(mem, self->mem, self->size * sizeof(T));
	free(clone->mem);
	clone->mem = mem;
	clone->size = self->size;
	clone->capacity = self->size + 1;
	return clone;
}

int	CONCAT(equal, T)(void *_self, void *_other)
{
	Vector(T) *self = _self;
	Vector(T) *other = _other;

	if ((!self && other) || (self && !other))
		return 0;
	unsigned int hash_self = CONCAT(vector_hash, T)(self);
	unsigned int hash_other = CONCAT(vector_hash, T)(other);
	return hash_self == hash_other;
}
unsigned int CONCAT(vector_hash, T)(void *_self)
{
	Vector(T)		*self = _self;
	unsigned int	hash = 0;
	unsigned char	*mem = (unsigned char *)self->mem;

	for (int i = 0; i < (self->size * (int)sizeof(T)) && i < 1000; i++) {
		hash = hash * 33 + mem[i];
	}
	return hash;
}

static bool	CONCAT(add_mem, T)(void *this, void *mem, int size)
{
	void					*nmem;
	CONCAT(Vector, T)	*this_;

	this_ = this;
	nmem = calloc(this_->size + size + 2, sizeof(T));
	if (!nmem)
		return (false);
	memmove(nmem, this_->mem, this_->size * sizeof(T));
	memmove(nmem + (this_->size * sizeof(T)), mem, size * sizeof(T));
	free(this_->mem);
	this_->mem = nmem;
	this_->size = this_->size + size;
	this_->capacity = this_->size + 1;
	return (true);
}

static T	CONCAT(at, T)(void *this, int elem)
{
	CONCAT(Vector, T) *this_;

	this_ = this;
	printf("%d\n", elem);
	assert(elem < this_->size);
	assert(elem >= 0);
	return (this_->mem[elem]);
}

static void	CONCAT(clear, T)(void *vec)
{
	CONCAT(Vector, T) *vector;

	vector = vec;
	bzero(vector->mem, vector->capacity * sizeof(T));
	vector->size = 0;
}

static bool	CONCAT(erase, T)(void *_self, int position)
{
	CONCAT(Vector, T) *self;

	self = _self;
	if (position < self->size && self->size > 0)
	{
		if (position < self->size - 1)
			memmove(self->mem + position,
				self->mem + (position + 1),
				(self->size - position - 1) * sizeof(T));
		self->size -= 1;
		memset(self->mem + self->size, 0, sizeof(T));
		return (true);
	}
	return (false);
}

static bool	CONCAT(insert, T)(void *_self, T elem, int position)
{
	T					*tmp;
	CONCAT(Vector, T)	*self;

	self = _self;
	if (position > self->size)
		return (false);
	if (self->capacity <= self->size + 1)
	{
		tmp = realloc(self->mem, self->capacity * sizeof(T) * 2);
		memset(tmp + self->size, 0, sizeof(T) * (self->capacity * 2 - self->size));
		if (!tmp)
			return (false);
		self->mem = tmp;
		self->capacity = self->capacity * 2;
	}
	memmove(self->mem + position + 1,
		self->mem + position,
		(self->size - position) * sizeof(T));
	self->mem[position] = elem;
	self->size += 1;
	return (true);
}

static bool	CONCAT(load, T)(void *this, void *mem, size_t n)
{
	void					*m;
	CONCAT(Vector, T)	*this_;

	this_ = this;
	m = calloc(n + 1, sizeof(T));
	if (!m)
		return (false);
	memcpy(m, mem, n * sizeof(T));
	free(this_->mem);
	this_->mem = m;
	this_->size = n;
	this_->capacity = n + 1;
	return (true);
}

static bool	CONCAT(push_back, T)(void *vec, T elem)
{
	CONCAT(Vector, T)	*vector;

	vector = vec;
	return (vector->vtable->insert(vector, elem, vector->size));
}

static bool	CONCAT(push_front, T)(void *vec, T elem)
{
	CONCAT(Vector, T)	*vector;

	vector = vec;
	return (vector->vtable->insert(vector, elem, 0));
}

static int	CONCAT(size, T)(void *vec)
{
	CONCAT(Vector, T)	*vector;

	vector = vec;
	return (vector->size);
}


/*****************************************************************/
/*****************************************************************/
/*****************************************************************/


/**
 * has_next checked next element
 * 
 */
 
static bool	CONCAT(has_next, T)(CONCAT(Iterator, T) *t)
{
	if (t->iter < t->container->size)
		return (true);
	return (false);
}

/**
 * next return value and increment iterator
 */

static T	CONCAT(next, T)(CONCAT(Iterator, T) *t)
{
    T ret = t->container->vtable->at(t->container, t->iter);
    t->iter += 1;
	return (ret);
}

static void	CONCAT(prev, T)(CONCAT(Iterator, T) *t)
{
    t->iter -= 1;
    //T ret = t->container->vtable->at(t->container, t->iter);
    //return (ret);
}

static T	CONCAT(get, T)(CONCAT(Iterator, T) *t)
{
    return (t->container->vtable->at(t->container, t->iter));
}

void		CONCAT(removeIterator, T)(CONCAT(Iterator, T) *t)
{
	t->container->vtable->erase(t->container, t->iter);
	t->iter -= 1;
}

void		CONCAT(addIterator, T)(CONCAT(Iterator, T) *t, T elem)
{
	t->container->vtable->insert(t->container, elem, t->iter);
	t->iter += 1;
}


/**
 * Constructor iterator
 */

void	*CONCAT(constructor_Iterator, T)(void *this, void *vec)
{
	CONCAT(Iterator, T) *this_;

	this_ = this;
	*this_ = (CONCAT(Iterator, T)) {
		.class = 0,
		.container = vec,
		.iter = 0
	};
	return (this);
}

# endif

#undef T
#endif
