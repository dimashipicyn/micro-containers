# micro-containers
```c
// Example
#include <stdio.h>

#define T int
#include "vector.h"
#define T char
#include "vector.h"
typedef Vector(char) * VectorPtr;
#define T VectorPtr
#include "vector.h"

int main()
{
    Vector(int) *vec = new(int);
    Vector(char) *vec_char = new(char);
    Vector(VectorPtr) *vec_char_ptr = new(VectorPtr);

    m_push_back(vec, 10);
    m_push_back(vec, 4);
    m_push_back(vec, 56);

    printf("vec pos 0 %d\n", m_at(vec, 0));
    printf("vec pos 1 %d\n", m_at(vec, 1));
    printf("vec pos 2 %d\n", m_at(vec, 2));

    m_push_back(vec_char, '#');
    m_push_back(vec_char, 'v');
    m_push_back(vec_char, 'A');

    printf("vec char pos 0 %c\n", m_at(vec_char, 0));
    printf("vec char pos 1 %c\n", m_at(vec_char, 1));
    printf("vec char pos 2 %c\n", m_at(vec_char, 2));

    m_push_back(vec_char_ptr, vec_char);
    m_push_back(vec_char_ptr, vec_char);
    m_push_back(vec_char_ptr, vec_char);

    printf("vec char ptr pos 0 %s\n", m_at(vec_char_ptr, 0)->mem);
    printf("vec char ptr pos 1 %s\n", m_at(vec_char_ptr, 1)->mem);
    printf("vec char ptr pos 2 %s\n", m_at(vec_char_ptr, 2)->mem);

    
    delete(vec);
    delete(vec_char);
    delete(vec_char_ptr);
}
```
