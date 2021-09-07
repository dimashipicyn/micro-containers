#include "object.h"

/**
 * dealloc
 */
void	delete(void *obj)
{
	Class	*m;

	if (!obj)
		return ;
	m = *(Class **)obj;
	if (m && m->destructor)
		m->destructor(obj);
	free(obj);
}

void	destructor(void *_self)
{
	Class	*class;

	if (_self)
		class = *(Class **)_self;
	if (_self && class && class->destructor)
		class->destructor(_self);
}

void	*clone(void *_self)
{
	Class	*class;

	if (_self)
		class = *(Class **)_self;
	if (_self && class && class->clone)
		return class->clone(_self);
	return 0;
}

int		equal(void *_self, void *other)
{
	Class	*class;

	if (_self)
		class = *(Class **)_self;
	if (_self && class && class->equal)
		return class->equal(_self, other);
	if (other)
		return 0;
	return 1;
}

unsigned int	hash(void *_self)
{
	Class	*class;

	if (_self)
		class = *(Class **)_self;
	if (_self && class && class->hash)
		return class->hash(_self);
	return (unsigned int)_self;
}
