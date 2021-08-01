#ifndef VECTOR_H
#define VECTOR_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "templates.h"

//Type of vector
#define Vector(type) TEMPLATE(Vector, type)

// Constructor and destructor
#define new(type) TEMPLATE(new, type)()
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
#define m_insert(obj, elem, pos) obj->method->insert(obj, elem, pos)
#define m_size(obj) obj->method->size(obj)
#define m_push_back(obj, elem) obj->method->push_back(obj, elem)
#define m_push_front(obj, elem) obj->method->push_front(obj, elem)
#define m_erase(obj, pos) obj->method->erase(obj, pos)
#define m_clear(obj) obj->method->clear(obj)
#define m_at(obj, pos) obj->method->at(obj, pos)



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

/**
 * Шаблоны прототипов встроенных функций
 */

bool    TEMPLATE(erase, T)(struct TEMPLATE(Vector, T) *this, size_t position);
int		TEMPLATE(size, T)(struct TEMPLATE(Vector, T) *this);
void	TEMPLATE(release, T)(void *obj);
bool    TEMPLATE(push_front, T)(struct TEMPLATE(Vector, T) *this, T);
bool    TEMPLATE(push_back, T)(struct TEMPLATE(Vector, T) *this, T);
bool    TEMPLATE(insert, T)(struct TEMPLATE(Vector, T) *this, T, size_t position);
T		TEMPLATE(at, T)(struct TEMPLATE(Vector, T) *this, size_t elem);
void	TEMPLATE(clear, T)(struct TEMPLATE(Vector, T) *this);
bool	TEMPLATE(load, T)(struct TEMPLATE(Vector, T) *this, void *mem, size_t n);
bool    TEMPLATE(add_mem, T)(struct TEMPLATE(Vector, T) *this, void *mem, size_t size);


static TEMPLATE(t_methods, T)	TEMPLATE(g_methods, T) =
{
	TEMPLATE(insert, T),
	TEMPLATE(size, T),
	TEMPLATE(push_back, T),
	TEMPLATE(push_front, T),
	TEMPLATE(erase, T),
	TEMPLATE(clear, T),
	TEMPLATE(at, T),
	TEMPLATE(release, T),
	TEMPLATE(load, T),
	TEMPLATE(add_mem, T),
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
bool	TEMPLATE(add_mem, T)(TEMPLATE(Vector, T) *this, void *mem, size_t size)
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

T	TEMPLATE(at, T)(TEMPLATE(Vector, T) *this, size_t elem)
{
	assert(elem < this->size);
	return (this->mem[elem]);
}

void	TEMPLATE(clear, T)(TEMPLATE(Vector, T) *vector)
{
	bzero(vector->mem, vector->capacity * sizeof(T));
	vector->size = 0;
}

bool	TEMPLATE(erase, T)(TEMPLATE(Vector, T) *this, size_t position)
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

bool	TEMPLATE(insert, T)(TEMPLATE(Vector, T) *this, T elem, size_t position)
{
	T	*tmp;

	if (position > this->size)
		return (false);
	if (this->capacity <= this->size)
	{
		tmp = realloc(this->mem, this->capacity * sizeof(T) * 2);
		if (!tmp)
			return (false);
		//free(this->mem);
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

bool	TEMPLATE(load, T)(TEMPLATE(Vector, T) *this, void *mem, size_t n)
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

bool	TEMPLATE(push_back, T)(TEMPLATE(Vector, T) *vector, T elem)
{
	return (vector->method->insert(vector, elem, vector->size));
}

bool	TEMPLATE(push_front, T)(TEMPLATE(Vector, T) *vector, T elem)
{
	return (vector->method->insert(vector, elem, 0));
}

void	TEMPLATE(release, T)(void *obj)
{
	TEMPLATE(Vector, T) *t;
	
	t = (TEMPLATE(Vector, T) *)obj;
	free(t->mem);
	free(t);
}

int	TEMPLATE(size, T)(TEMPLATE(Vector, T) *vector)
{
	return (vector->size);
}

# ifndef Destructor
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

#endif

#undef T
#endif