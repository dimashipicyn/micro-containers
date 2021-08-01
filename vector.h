#ifndef VECTOR_H
#define VECTOR_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "templates.h"

// Type of vector
#define Vector(type) TEMPLATE(Vector, type)
// Type of iterator
#define Iterator(type) TEMPLATE(Iterator, type)

// Constructor
#define new(type) TEMPLATE(new, type)()
// Destructor
void	delete(void *obj);

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
#define m_insert(obj, elem, pos) obj->method->insert(obj, elem, pos)
/**
 * m_size Возвращает размер массива
 */
#define m_size(obj) obj->method->size(obj)
/**
 * Вставка елемента в конец массива
 */
#define m_push_back(obj, elem) obj->method->push_back(obj, elem)
#define m_push_front(obj, elem) obj->method->push_front(obj, elem)
#define m_erase(obj, pos) obj->method->erase(obj, pos)
#define m_clear(obj) obj->method->clear(obj)
#define m_at(obj, pos) obj->method->at(obj, pos)
#define m_iterator(obj) obj->method->iter(obj)
#define m_has_next(iterator) iterator->has_next(iterator)
#define m_next(iterator) iterator->next(iterator)

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

struct	TEMPLATE(Vector, T);
struct	TEMPLATE(Iterator, T);

typedef struct TEMPLATE(s_methods, T)
{
	bool	(*insert)(struct TEMPLATE(Vector, T) *, T, size_t);
	int		(*size)(struct TEMPLATE(Vector, T) *);
	bool	(*push_back)(struct TEMPLATE(Vector, T) *, T);
	bool	(*push_front)(struct TEMPLATE(Vector, T) *, T);
	bool	(*erase)(struct TEMPLATE(Vector, T) *, size_t);
	void	(*clear)(struct TEMPLATE(Vector, T) *);
	T		(*at)(struct TEMPLATE(Vector, T) *, size_t);
	void	(*release)(void *);
	bool	(*load)(struct TEMPLATE(Vector, T) *, void *, size_t);
	bool	(*add_mem)(struct TEMPLATE(Vector, T) *, void *, size_t);
	struct TEMPLATE(Iterator, T)	(*iter)(struct TEMPLATE(Vector, T) *);
}				TEMPLATE(t_methods, T);

typedef struct TEMPLATE(Vector, T)
{
	TEMPLATE(t_methods, T)	*method;
	T			*mem;
	size_t		size;
	size_t		capacity;
}				TEMPLATE(Vector, T);

/**************************************************************************/
/**************************************************************************/
/***************************************************************************/

#ifdef template
/**
 * Шаблоны прототипов встроенных функций
 */

static bool		TEMPLATE(erase, T)(struct TEMPLATE(Vector, T) *this, size_t position);
static int		TEMPLATE(size, T)(struct TEMPLATE(Vector, T) *this);
static void		TEMPLATE(release, T)(void *obj);
static bool		TEMPLATE(push_front, T)(struct TEMPLATE(Vector, T) *this, T);
static bool		TEMPLATE(push_back, T)(struct TEMPLATE(Vector, T) *this, T);
static bool		TEMPLATE(insert, T)(struct TEMPLATE(Vector, T) *this, T, size_t position);
static T		TEMPLATE(at, T)(struct TEMPLATE(Vector, T) *this, size_t elem);
static void		TEMPLATE(clear, T)(struct TEMPLATE(Vector, T) *this);
static bool		TEMPLATE(load, T)(struct TEMPLATE(Vector, T) *this, void *mem, size_t n);
static bool		TEMPLATE(add_mem, T)(struct TEMPLATE(Vector, T) *this, void *mem, size_t size);
static struct TEMPLATE(Iterator, T)		TEMPLATE(newIterator, T)(TEMPLATE(Vector, T) *vec);


static TEMPLATE(t_methods, T)	TEMPLATE(g_methods, T) =
{
	.insert = TEMPLATE(insert, T),
	.size = TEMPLATE(size, T),
	.push_back = TEMPLATE(push_back, T),
	.push_front = TEMPLATE(push_front, T),
	.erase = TEMPLATE(erase, T),
	.clear = TEMPLATE(clear, T),
	.at = TEMPLATE(at, T),
	.release = TEMPLATE(release, T),
	.load = TEMPLATE(load, T),
	.add_mem = TEMPLATE(add_mem, T),
	.iter = TEMPLATE(newIterator, T)
};
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

/**
 * Конструктор
 */

TEMPLATE(Vector, T)	*TEMPLATE(new, T)()
{
	TEMPLATE(Vector, T)	*vec;
	T	*mem;

	vec = (TEMPLATE(Vector, T) *)calloc(1, sizeof(TEMPLATE(Vector, T)));
	mem = calloc(1, sizeof(T));
	if (!vec || !mem)
	{
		free(vec);
		free(mem);
		return (0);
	}
	vec->mem = mem;
	vec->capacity = 1;
	vec->method = &TEMPLATE(g_methods, T);
	return (vec);
}

/**
 * 
 */
static bool	TEMPLATE(add_mem, T)(TEMPLATE(Vector, T) *this, void *mem, size_t size)
{
	void	*nmem;

	nmem = calloc(this->size + size + 2, sizeof(T));
	if (!nmem)
		return (false);
	memmove(nmem, this->mem, this->size * sizeof(T));
	memmove(nmem + (this->size * sizeof(T)), mem, size * sizeof(T));
	free(this->mem);
	this->mem = nmem;
	this->size = this->size + size;
	this->capacity = this->size + 1;
	return (true);
}

static T	TEMPLATE(at, T)(TEMPLATE(Vector, T) *this, size_t elem)
{
	assert(elem < this->size);
	return (this->mem[elem]);
}

static void	TEMPLATE(clear, T)(TEMPLATE(Vector, T) *vector)
{
	bzero(vector->mem, vector->capacity * sizeof(T));
	vector->size = 0;
}

static bool	TEMPLATE(erase, T)(TEMPLATE(Vector, T) *this, size_t position)
{
	if (position < this->size)
	{
		memcpy(this->mem + position * sizeof(T),
			this->mem + (position + 1) * sizeof(T),
			(this->size - position + 1) * sizeof(T));
		this->size -= 1;
		bzero(this->mem + (this->size * sizeof(T)), sizeof(T));
		return (true);
	}
	return (false);
}

static bool	TEMPLATE(insert, T)(TEMPLATE(Vector, T) *this, T elem, size_t position)
{
	T	*tmp;

	if (position > this->size)
		return (false);
	if (this->capacity <= this->size)
	{
		tmp = realloc(this->mem, this->capacity * sizeof(T) * 2);
		if (!tmp)
			return (false);
		this->mem = tmp;
		this->capacity = this->capacity * 2;
	}
	memmove(this->mem + ((position + 1) * sizeof(T)),
		this->mem + (position * sizeof(T)),
		(this->size - position) * sizeof(T));
	this->mem[position] = elem;
	this->size += 1;
	return (true);
}

static bool	TEMPLATE(load, T)(TEMPLATE(Vector, T) *this, void *mem, size_t n)
{
	void	*m;

	m = calloc(n + 1, sizeof(T));
	if (!m)
		return (false);
	memcpy(m, mem, n * sizeof(T));
	free(this->mem);
	this->mem = m;
	this->size = n;
	this->capacity = n + 1;
	return (true);
}

static bool	TEMPLATE(push_back, T)(TEMPLATE(Vector, T) *vector, T elem)
{
	return (vector->method->insert(vector, elem, vector->size));
}

static bool	TEMPLATE(push_front, T)(TEMPLATE(Vector, T) *vector, T elem)
{
	return (vector->method->insert(vector, elem, 0));
}

static void	TEMPLATE(release, T)(void *obj)
{
	TEMPLATE(Vector, T) *t;
	
	t = (TEMPLATE(Vector, T) *)obj;
	free(t->mem);
	free(t);
}

static int	TEMPLATE(size, T)(TEMPLATE(Vector, T) *vector)
{
	return (vector->size);
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

/**
 * Iterator container template
 * 
 */

typedef struct	TEMPLATE(Iterator, T)
{
	size_t						iter;
	struct TEMPLATE(Vector, T)	*container;
	bool						(*has_next)(struct TEMPLATE(Iterator, T) *);
	T							(*next)(struct TEMPLATE(Iterator, T) *);
}	TEMPLATE(Iterator, T);

/**
 * has_next checked next element
 * 
 */
static bool	TEMPLATE(has_next, T)(TEMPLATE(Iterator, T) *t)
{
	if (t->iter < t->container->size)
		return (true);
	return (false);
}
/**
 * next return value and increment iterator
 */
static T	TEMPLATE(next, T)(TEMPLATE(Iterator, T) *t)
{
	return (t->container->method->at(t->container, t->iter++));
}

/**
 * Constructor iterator in stack
 */
TEMPLATE(Iterator, T)	TEMPLATE(newIterator, T)(TEMPLATE(Vector, T) *vec)
{
	TEMPLATE(Iterator, T) t = (TEMPLATE(Iterator, T)){
		.container = vec,
		.has_next = TEMPLATE(has_next, T),
		.next = TEMPLATE(next, T),
		.iter = 0
	};
	return (t);
}

#  ifndef Destructor
# define Destructor
/**
 * Деструктор
 */

void	delete(void *obj)
{
	TEMPLATE(t_methods, T)	*m;

	if (!obj)
		return ;
	m = *(TEMPLATE(t_methods, T) **)obj;
	if (m && m->release)
		m->release(obj);
}

#  endif

# endif

#undef T
#endif