#include <stdio.h>
#include <string.h>
#include "object.h"

typedef struct user_class
{
	Class	*class; // первым элементом класс
	int		*content;
} user_class; // user "class"

int	user_equal(void *_self, void *_other); // сигнатура функции сравнения
void *user_clone(void *_self); // сигнатура копирующего конструктора
void user_destructor(void *_self); // сигнатура деструктора: void name(void *self)

void *constructor_user_class(void *_self) // сигнатура конструктора:  void *constructor_ + имя структуры(void *self, [other arg])
{									// может иметь разное количество параметров, первым принимает указатель на структуру
	user_class *self = _self;

	static Class class_int_t =		// создаем экземляр предка
	{
		.destructor = user_destructor,
		.equal = user_equal,
		.clone = user_clone
	};
	self->class = &class_int_t;	// записываем указатель на предка
	
	self->content = calloc(5, sizeof(int));
	printf("User constructor call!\n");
	return self;
}

void user_destructor(void *_self) // сигнатура деструктора: void name(void *self)
{
	user_class *self = _self;
	
	free(self->content);
	printf("User destructor call!\n");
}

void *user_clone(void *_self) // сигнатура копирующего конструктора
{
	user_class *self = _self;
	user_class *clone = new(user_class);
	
	if (!clone)
		return 0;
	memcpy(clone->content, self->content, 5 * sizeof(int));
	return clone;
}

int	user_equal(void *_self, void *_other) // сигнатура функции сравнения
{
	user_class *self = _self;
	user_class *other = _other;
	// может быть любая проверка
	return self == other;
}

int main()
{
	// новые экземляры пользовательского класса
	user_class *ptr = new(user_class);
	user_class *ptr1 = $(user_class); // на стеке
	user_class *ptr2 = $(user_class); // на стеке
	
	// копия
	user_class *ptr3 = clone(ptr); // всегда в куче

	printf("equal: %s\n", equal(ptr, ptr) ? "true" : "false");
	printf("equal: %s\n", equal(ptr, ptr3) ? "true" : "false");
	printf("equal: %s\n", equal(ptr1, ptr2) ? "true" : "false");
	
	// удаляем
	delete(ptr);
	delete(ptr3);
	
	// вызываем если внутри класса выделяется память
	destructor(ptr1);
	destructor(ptr2);
}
