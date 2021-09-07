# Object oriented c

## Classes
```c
#include <stdio.h>
#include <string.h>
#include "object.h"

typedef struct user_class
{
    Class    *class; // первым элементом класс
    int        *content;
} user_class; // user "class"

int    user_equal(void *_self, void *_other); // сигнатура функции сравнения
void *user_clone(void *_self); // сигнатура копирующего конструктора
void user_destructor(void *_self); // сигнатура деструктора: void name(void *self)

void *constructor_user_class(void *_self) // сигнатура конструктора:  void *constructor_ + имя структуры(void *self, [other arg])
{                                    // может иметь разное количество параметров, первым принимает указатель на структуру
    user_class *self = _self;

    static Class class_int_t =        // создаем экземляр предка
    {
        .destructor = user_destructor,
        .equal = user_equal,
        .clone = user_clone
    };
    self->class = &class_int_t;    // записываем указатель на предка
    
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

int    user_equal(void *_self, void *_other) // сигнатура функции сравнения
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
```
###### Если запустить код то увидим:
```sh
User constructor call!
User constructor call!
User constructor call!
User constructor call!
equal: true
equal: false
equal: false
User destructor call!
User destructor call!
User destructor call!
User destructor call!
```
### Exceptions
```c
#include <stdio.h>
#include "exception.h"

void div(int n)
{
    if (n == 0)
        throw(ZERO_DIVISION_EXCEPTION);
    // не выполнится
    int e = 3 / n;
}

int main(int ac, char **av)
{
    try
    {
        div(0); // throw zero division

        // no print
        printf("no print\n");
    }
    catch(ZERO_DIVISION_EXCEPTION)
    {
        printf("error %s\n", strexception);
    }
    catchAll
    {
        printf("error %s\n", strexception);
    }

    printf("end program\n");

    return 0;
}
```
#### Vector

```c
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


    // новый итератор по вектору чаров на стеке, delete не нужен
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
```

### Сборка библиотеки
```sh
git clone git@github.com:dimashipicyn/miniobjectc.git
cd miniobjectc
make
```
#### Юнит тесты
```sh
make test
```
