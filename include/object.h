#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>

#define CAT(X,Y) X##_##Y
#define CONCAT(X,Y) CAT(X,Y)

// alloc
#define new(type, ...) \
			CONCAT(constructor, type)(calloc(1, sizeof(type)), ## __VA_ARGS__)

#define $(type, ...) \
			CONCAT(constructor, type)((void *)(char[sizeof(type)]){}, ##  __VA_ARGS__)

// dealloc
void	delete(void *obj);
void	destructor(void *_self);
int		equal(void *_self, void *_other);
void	*clone(void *_self);

typedef struct Class
{
	void	(*destructor)(void *self);
	int		(*equal)(void *self, void *other);
	void	*(*clone)(void *self);
}	Class;

#endif
