#ifndef VECTOR_H
#define VECTOR_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "object.h"

// Type of vector
#define Vector(type) CONCAT(Vector, type)
#define Iterator(type) CONCAT(Iterator, type)

/**
 * Обертки над встроенными функциями
 * m_insert(object, element, position);
 * m_size(object);
 * m_push_back(object, element);
 * m_push_front(object, element);
 * m_erase(object, element);
 * m_clear(object);
 * m_at(object, position);
 */

/**
 * m_insert Вставка елемента в позицию в массиве
 */
#define m_insert(obj, elem, pos) obj->vtable->insert(obj, elem, pos)
/**
 * m_size Возвращает размер массива
 */
#define m_size(obj) obj->vtable->size(obj)
/**
 * Вставка елемента в конец массива
 */
#define m_push_back(obj, elem) obj->vtable->push_back(obj, elem)
#define m_push_front(obj, elem) obj->vtable->push_front(obj, elem)
#define m_erase(obj, pos) obj->vtable->erase(obj, pos)
#define m_clear(obj) obj->vtable->clear(obj)
#define m_at(obj, pos) obj->vtable->at(obj, pos)

#define m_has_next(iterator) iterator->container->vtable->has_next(iterator)
#define m_next(iterator) iterator->container->vtable->next(iterator)
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

typedef int* int_ptr_t;
#define T int_ptr_t
#include "vector.h"

typedef char* char_ptr_t;
#define T char_ptr_t
#include "vector.h"

typedef double* double_ptr_t;
#define T double_ptr_t
#include "vector.h"

typedef float* float_ptr_t;
#define T float_ptr_t
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
	void	(*remove)(struct CONCAT(Iterator, T) *);
	void	(*add)(struct CONCAT(Iterator, T) *, T);
}				CONCAT(t_vtable, T);

typedef struct CONCAT(Vector, T)
{
	Class					*class;
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

static void		CONCAT(destructor, T)(void *obj);
static bool		CONCAT(erase, T)(void *this, int position);
static int		CONCAT(size, T)(void *this);
static bool		CONCAT(push_front, T)(void *this, T);
static bool		CONCAT(push_back, T)(void *this, T);
static bool		CONCAT(insert, T)(void *this, T, int position);
static T		CONCAT(at, T)(void *this, int elem);
static void		CONCAT(clear, T)(void *this);
static bool		CONCAT(load, T)(void *this, void *mem, size_t n);
static bool		CONCAT(add_mem, T)(void *this, void *mem, int size);
static bool		CONCAT(has_next, T)(CONCAT(Iterator, T) *t);
static T		CONCAT(next, T)(CONCAT(Iterator, T) *t);
void			CONCAT(addIterator, T)(CONCAT(Iterator, T) *t, T elem);
void			CONCAT(removeIterator, T)(CONCAT(Iterator, T) *t);

static Class	CONCAT(g_class, T) =
{
	.destructor = CONCAT(destructor, T)
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

/**
 * 
 */
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
	assert(elem < this_->size);
	return (this_->mem[elem]);
}

static void	CONCAT(clear, T)(void *vec)
{
	CONCAT(Vector, T) *vector;

	vector = vec;
	bzero(vector->mem, vector->capacity * sizeof(T));
	vector->size = 0;
}

static bool	CONCAT(erase, T)(void *this, int position)
{
	CONCAT(Vector, T) *this_;

	this_ = this;
	if (position < this_->size)
	{
		if (position < this_->size - 1)
			memmove(this_->mem + position,
				this_->mem + (position + 1),
				(this_->size - position - 1) * sizeof(T));
		this_->size -= 1;
		return (true);
	}
	return (false);
}

static bool	CONCAT(insert, T)(void *this, T elem, int position)
{
	T						*tmp;
	CONCAT(Vector, T)	*this_;

	this_ = this;
	if (position > this_->size)
		return (false);
	if (this_->capacity <= this_->size)
	{
		tmp = realloc(this_->mem, this_->capacity * sizeof(T) * 2);
		if (!tmp)
			return (false);
		this_->mem = tmp;
		this_->capacity = this_->capacity * 2;
	}
	memmove(this_->mem + position + 1,
		this_->mem + position,
		(this_->size - position) * sizeof(T));
	this_->mem[position] = elem;
	this_->size += 1;
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

static void	CONCAT(destructor, T)(void *obj)
{
	CONCAT(Vector, T) *t;
	
	t = obj;
	free(t->mem);
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
	if (t->iter < t->container->size - 1)
		return (true);
	return (false);
}

/**
 * next return value and increment iterator
 */

static T	CONCAT(next, T)(CONCAT(Iterator, T) *t)
{
	t->iter += 1;
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
		.iter = -1
	};
	return (this);
}

# endif

#undef T
#endif
