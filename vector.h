#ifndef VECTOR_H
#define VECTOR_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CAT(X,Y) X##_##Y
#define TEMPLATE(X,Y) CAT(X,Y)

// Type of vector
#define Vector(type) TEMPLATE(Vector, type)
// Type of iterator
#define Iterator(type) TEMPLATE(Iterator, type)

// Constructor
#define new(cont, type, ...) TEMPLATE(init, TEMPLATE(cont, type))(calloc(1, sizeof(TEMPLATE(cont, type))), ## __VA_ARGS__)
#define $(cont, type, ...) TEMPLATE(init, TEMPLATE(cont, type))((void *)(char[sizeof(TEMPLATE(cont, type))]){}, ##  __VA_ARGS__)
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
#define m_remove(iterator) iterator->remove(iterator)
#define m_add(iterator, elem) iterator->add(iterator, elem)

typedef struct Class
{
	void	(*release)(void *);
}	Class;

/**
 * Деструктор
 */

void	delete(void *obj)
{
	Class	*m;

	if (!obj)
		return ;
	m = *(Class **)obj;
	if (m && m->release)
		m->release(obj);
	free(obj);
}

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
	void	(*release)(void *);
	bool	(*insert)(struct TEMPLATE(Vector, T) *, T, int);
	int		(*size)(struct TEMPLATE(Vector, T) *);
	bool	(*push_back)(struct TEMPLATE(Vector, T) *, T);
	bool	(*push_front)(struct TEMPLATE(Vector, T) *, T);
	bool	(*erase)(struct TEMPLATE(Vector, T) *, int);
	void	(*clear)(struct TEMPLATE(Vector, T) *);
	T		(*at)(struct TEMPLATE(Vector, T) *, int);
	bool	(*load)(struct TEMPLATE(Vector, T) *, void *, size_t);
	bool	(*add_mem)(struct TEMPLATE(Vector, T) *, void *, int);
	struct TEMPLATE(Iterator, T)	*(*iter)(struct TEMPLATE(Vector, T) *);
}				TEMPLATE(t_methods, T);

typedef struct TEMPLATE(Vector, T)
{
	TEMPLATE(t_methods, T)	*method;
	T			*mem;
	int			size;
	int			capacity;
}				TEMPLATE(Vector, T);

/**************************************************************************/
/**************************************************************************/
/***************************************************************************/

#ifdef template
/**
 * Шаблоны прототипов встроенных функций
 */

static bool		TEMPLATE(erase, T)(struct TEMPLATE(Vector, T) *this, int position);
static int		TEMPLATE(size, T)(struct TEMPLATE(Vector, T) *this);
static void		TEMPLATE(release, T)(void *obj);
static bool		TEMPLATE(push_front, T)(struct TEMPLATE(Vector, T) *this, T);
static bool		TEMPLATE(push_back, T)(struct TEMPLATE(Vector, T) *this, T);
static bool		TEMPLATE(insert, T)(struct TEMPLATE(Vector, T) *this, T, int position);
static T		TEMPLATE(at, T)(struct TEMPLATE(Vector, T) *this, int elem);
static void		TEMPLATE(clear, T)(struct TEMPLATE(Vector, T) *this);
static bool		TEMPLATE(load, T)(struct TEMPLATE(Vector, T) *this, void *mem, size_t n);
static bool		TEMPLATE(add_mem, T)(struct TEMPLATE(Vector, T) *this, void *mem, int size);
static struct TEMPLATE(Iterator, T)		*TEMPLATE(newIterator, T)(TEMPLATE(Vector, T) *vec);


static TEMPLATE(t_methods, T)	TEMPLATE(g_methods, T) =
{
	.release = TEMPLATE(release, T),
	.insert = TEMPLATE(insert, T),
	.size = TEMPLATE(size, T),
	.push_back = TEMPLATE(push_back, T),
	.push_front = TEMPLATE(push_front, T),
	.erase = TEMPLATE(erase, T),
	.clear = TEMPLATE(clear, T),
	.at = TEMPLATE(at, T),
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

TEMPLATE(Vector, T)	*TEMPLATE(init_Vector, T)(TEMPLATE(Vector, T) *vec, ...)
{
//	TEMPLATE(Vector, T)	*vec;
	T	*mem;

//	vec = (TEMPLATE(Vector, T) *)calloc(1, sizeof(TEMPLATE(Vector, T)));
	mem = calloc(1, sizeof(T));
//	if (!vec || !mem)
	if (!mem)
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
static bool	TEMPLATE(add_mem, T)(TEMPLATE(Vector, T) *this, void *mem, int size)
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

static T	TEMPLATE(at, T)(TEMPLATE(Vector, T) *this, int elem)
{
	assert(elem < this->size);
	return (this->mem[elem]);
}

static void	TEMPLATE(clear, T)(TEMPLATE(Vector, T) *vector)
{
	bzero(vector->mem, vector->capacity * sizeof(T));
	vector->size = 0;
}

static bool	TEMPLATE(erase, T)(TEMPLATE(Vector, T) *this, int position)
{
	if (position < this->size)
	{
		if (position < this->size - 1)
			memmove(this->mem + position,
				this->mem + (position + 1),
				(this->size - position - 1) * sizeof(T));
		this->size -= 1;
		return (true);
	}
	return (false);
}

static bool	TEMPLATE(insert, T)(TEMPLATE(Vector, T) *this, T elem, int position)
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
	memmove(this->mem + position + 1,
		this->mem + position,
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
	Class						*class;
	struct TEMPLATE(Vector, T)	*container;
	bool						(*has_next)(struct TEMPLATE(Iterator, T) *);
	T							(*next)(struct TEMPLATE(Iterator, T) *);
	void						(*remove)(struct TEMPLATE(Iterator, T) *);
	void						(*add)(struct TEMPLATE(Iterator, T) *, T);
	int							iter;
}	TEMPLATE(Iterator, T);

/**
 * has_next checked next element
 * 
 */
static bool	TEMPLATE(has_next, T)(TEMPLATE(Iterator, T) *t)
{
	if (t->iter < t->container->size - 1)
		return (true);
	return (false);
}
/**
 * next return value and increment iterator
 */
static T	TEMPLATE(next, T)(TEMPLATE(Iterator, T) *t)
{
	t->iter += 1;
	return (t->container->method->at(t->container, t->iter));
}

void		TEMPLATE(removeIterator, T)(TEMPLATE(Iterator, T) *t)
{
	t->container->method->erase(t->container, t->iter);
	t->iter -= 1;
}

void		TEMPLATE(addIterator, T)(TEMPLATE(Iterator, T) *t, T elem)
{
	t->container->method->insert(t->container, elem, t->iter);
	t->iter += 1;
}

/**
 * Constructor iterator in stack
 */
TEMPLATE(Iterator, T)	*TEMPLATE(newIterator, T)(TEMPLATE(Vector, T) *vec)
{
	TEMPLATE(Iterator, T) *t = calloc(1, sizeof(TEMPLATE(Iterator, T)));
	assert(t);
	*t = (TEMPLATE(Iterator, T)){
		.container = vec,
		.has_next = TEMPLATE(has_next, T),
		.next = TEMPLATE(next, T),
		.remove = TEMPLATE(removeIterator, T),
		.add = TEMPLATE(addIterator, T),
		.iter = -1,
		.class = 0
	};
	return (t);
}




# endif

#undef T
#endif
