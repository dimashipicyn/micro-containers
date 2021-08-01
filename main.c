#include <stdio.h>
#define T int
#include "vector.h"

int main()
{
	Vector(int) *vec = new(int);

	m_push_back(vec, 10);
	m_push_back(vec, 32);
	m_push_back(vec, 12);
	m_push_back(vec, 76);
	m_push_back(vec, 3);

	for (int i = 0; i < 5; i++)
		printf("vec[%d] = %d\n", i, m_at(vec, i));
	
	Iterator(int) iter = m_iterator(vec), *iterPtr = &iter;
	while (m_has_next(iterPtr))
	{
		int num = m_next(iterPtr);
		printf("test iterator %d\n", num);
	}
}
