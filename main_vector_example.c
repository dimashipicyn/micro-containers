#include <stdio.h>
#include "object.h"
#include "vector.h"

typedef Vector(char)* VectorPtr; // чтобы убрать * из типа

// темплейтинг
#define template // генерация методов, ставим в одном файле
#define T VectorPtr // во всех файлах где используется
#include "vector.h" // тоже во всех

int main()
{
	Vector(int) *vec_int = new(Vector(int)); // новый массив интов в куче
	Vector(char) *vec_char = new(Vector(char)); // новый массив чаров в куче
    Vector(VectorPtr) *vec_char_ptr = new(Vector(VectorPtr)); // новый массив векторов в куче

	// добавляем числа
    m_push_back(vec_int, 10);
    m_push_back(vec_int, 4);
    m_push_back(vec_int, 56);

	// печатаем
    printf("vec_int pos 0 %d\n", m_at(vec_int, 0));
    printf("vec_int pos 1 %d\n", m_at(vec_int, 1));
    printf("vec_int pos 2 %d\n", m_at(vec_int, 2));

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

	printf("vec_int hash: %u\n", hash(vec_int));
	printf("vec_char hash: %u\n", hash(vec_char));
	printf("vec_char_ptr hash: %u\n", hash(vec_char_ptr));
   
	// сравниваем вектора: если равны 1, если нет 0
	printf("equal vec_int, vec_char: %s\n", equal(vec_int, vec_char) ? "true" : "false");
	printf("equal vec_int, vec_int: %s\n", equal(vec_int, vec_int) ? "true" : "false");


	// новый итератор по вектору интов на стеке, delete не нужен
	Iterator(char) *i = $(Iterator(char), vec_char);
	while (m_has_next(i))
	{
		char p = m_next(i);

		printf("vec_char ch: %c\n", p);
	}
    
	// call destructor and free
	delete(vec_int);
    delete(vec_char);
    delete(vec_char_ptr);
		
}
