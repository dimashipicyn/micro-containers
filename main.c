#include <stdio.h>
#include "object.h"
#include "vector.h"

typedef Vector(char)* VectorPtr; // чтобы убрать * из типа

// темплейтинг
#define template // генерация методов, ставим в одном файле
#define T VectorPtr // во всех файлах где используется
#include "vector.h" // тоже во всех

typedef struct s_int
{
	Class	*class; // первым элементом класс
	int		*content;
} t_int; // user "class"

int	user_equal(void *_self, void *_other); // сигнатура функции сравнения
void *user_clone(void *_self); // сигнатура копирующего конструктора
void user_destructor(void *_self); // сигнатура деструктора: void name(void *self)

void *constructor_t_int(void *_self) // сигнатура конструктора:  void *constructor_ + имя структуры(void *self, [other arg])
{									// может иметь разное количество параметров, первым принимает указатель на структуру
	t_int *self = _self;

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
	t_int *self = _self;
	
	free(self->content);
	printf("User destructor call!\n");
}

void *user_clone(void *_self) // сигнатура копирующего конструктора
{
	t_int *self = _self;
	t_int *clone = new(t_int);
	
	if (!clone)
		return 0;
	memcpy(clone->content, self->content, 5 * sizeof(int));
	return clone;
}

int	user_equal(void *_self, void *_other) // сигнатура функции сравнения
{
	t_int *self = _self;
	t_int *other = _other;
	// может быть любая проверка
	return self == other;
}

int main()
{
	Vector(int) *vec = new(Vector(int)); // новый массив интов в куче
	Vector(char) *vec_char = new(Vector(char)); // новый массив чаров в куче
    Vector(VectorPtr) *vec_char_ptr = new(Vector(VectorPtr)); // новый массив векторов в куче

	// добавляем числа
    m_push_back(vec, 10);
    m_push_back(vec, 4);
    m_push_back(vec, 56);

	// печатаем
    printf("vec pos 0 %d\n", m_at(vec, 0));
    printf("vec pos 1 %d\n", m_at(vec, 1));
    printf("vec pos 2 %d\n", m_at(vec, 2));

	// добавляем символы
    m_push_back(vec_char, '#');
    m_push_back(vec_char, 'v');
    m_push_back(vec_char, 'A');

    printf("vec_char pos 0 %c\n", m_at(vec_char, 0));
    printf("vec_char pos 1 %c\n", m_at(vec_char, 1));
    printf("vec_char pos 2 %c\n", m_at(vec_char, 2));

	// добавляем вектора
    m_push_back(vec_char_ptr, vec_char);
    m_push_back(vec_char_ptr, vec_char);

	// печатаем
    printf("vec_char_ptr pos 0: vector_char pos 0 %c\n", m_at(m_at(vec_char_ptr, 0), 0));
    printf("vec_char_ptr pos 1: vector_char pos 0 %c\n", m_at(m_at(vec_char_ptr, 1), 0));

    
	// новый итератор по вектору интов на стеке, delete не нужен
	Iterator(int) *i = $(Iterator(int), vec);
	while (m_has_next(i))
	{
		int p = m_next(i);
		printf("vec  %d\n", p);
	}
    
	// call destructor and free
	delete(vec);
    delete(vec_char);
    delete(vec_char_ptr);
	

	// новые экземляры пользовательского класса
	t_int *ptr = new(t_int);
	t_int *ptr1 = $(t_int); // на стеке
	t_int *ptr2 = $(t_int); // на стеке
	
	t_int *ptr3 = clone(ptr); // всегда в куче

	if (equal(ptr, ptr3))
		printf("ptr == ptr3\n");
	else
		printf("ptr != ptr3\n");

	// удаляем
	delete(ptr);
	delete(ptr3);
	
	// вызываем если внутри класса выделяется память
	destructor(ptr1);
	destructor(ptr2);
}
