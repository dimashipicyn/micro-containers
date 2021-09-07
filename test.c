#include <stdio.h>
#include "vector.h"
#include "acutest.h"

#define TRUE 1
#define FALSE 0

void test_new(void)
{
	Vector(char) *vector_char = new(Vector(char));
	Vector(int) *vector_int = new(Vector(int));
	Vector(double) *vector_double = new(Vector(double));
	Vector(void_ptr_t) *vector_ptr = new(Vector(void_ptr_t));

	TEST_CHECK(vector_char != 0);
	TEST_CHECK(vector_int != 0);
	TEST_CHECK(vector_double != 0);
	TEST_CHECK(vector_ptr != 0);
	TEST_CHECK(vector_char->mem != 0);
	TEST_CHECK(vector_int->mem != 0);
	TEST_CHECK(vector_double->mem != 0);
	TEST_CHECK(vector_ptr->mem != 0);
	delete(vector_char);
	delete(vector_int);
	delete(vector_double);
	delete(vector_ptr);
}

void test_insert(void)
{
	Vector(char) *vector_char = new(Vector(char));
	
	TEST_CHECK(m_insert(vector_char, 'x', 0) == TRUE);
	TEST_CHECK(m_insert(vector_char, 'y', 1) == TRUE);
	TEST_CHECK(m_insert(vector_char, 'z', 2) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "xyz", 3) == 0);
	TEST_CHECK(m_insert(vector_char, 'z', 4) != TRUE);
	TEST_CHECK(m_insert(vector_char, 'A', 1) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "xAyz", 4) == 0);
	delete(vector_char);
}

void test_push_back(void)
{
	Vector(char) *vector_char = new(Vector(char));
	
	TEST_CHECK(m_push_back(vector_char, 'x') == TRUE);
	TEST_CHECK(m_push_back(vector_char, 'y') == TRUE);
	TEST_CHECK(m_push_back(vector_char, 'z') == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "xyz", 3) == 0);
	delete(vector_char);
}

void test_push_front(void)
{
	Vector(char) *vector_char = new(Vector(char));
	
	TEST_CHECK(m_push_front(vector_char, 'x') == TRUE);
	TEST_CHECK(m_push_front(vector_char, 'y') == TRUE);
	TEST_CHECK(m_push_front(vector_char, 'z') == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "zyx", 3) == 0);
	delete(vector_char);
}

void test_load(void)
{
	Vector(char) *vector_char = new(Vector(char));

	TEST_CHECK(m_load(vector_char, "hello world", 11) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "hello world", 11) == 0);
	delete(vector_char);
}

void test_size(void)
{
	Vector(char) *vector_char = new(Vector(char));

	TEST_CHECK(m_size(vector_char) == 0);
	TEST_CHECK(m_load(vector_char, "hello world", 11) == TRUE);
	TEST_CHECK(m_size(vector_char) == 11);
	delete(vector_char);
}

void test_erase(void)
{
	Vector(char) *vector_char = new(Vector(char));

	TEST_CHECK(m_load(vector_char, "hello world", 11) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "hello world", 11) == 0);
	TEST_CHECK(m_erase(vector_char, 0) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "ello world", 10) == 0);
	TEST_CHECK(m_erase(vector_char, 9) == TRUE);
	TEST_CHECK(memcmp(vector_char->mem, "ello worl", 9) == 0);
	TEST_CHECK(m_erase(vector_char, 9) != TRUE);
	delete(vector_char);
}

void test_clear(void)
{
	Vector(char) *vector_char = new(Vector(char));

	TEST_CHECK(m_load(vector_char, "hello world", 11) == TRUE);
	void *m = calloc(1, vector_char->capacity);
	m_clear(vector_char);
	TEST_CHECK(memcmp(vector_char->mem, m, vector_char->capacity) == 0);
	delete(vector_char);
	free(m);
}

void test_at(void)
{
	char p = 0;
	Vector(char) *vector_char = new(Vector(char));

	TEST_CHECK(m_load(vector_char, "hello world", 11) == TRUE);
	p = m_at(vector_char, 0);
	TEST_CHECK(p != 0);
	TEST_CHECK(p == 'h');
	p = m_at(vector_char, 10);
	TEST_CHECK(p != 0);
	TEST_CHECK(p == 'd');
//	p = m_at(vector_char, 11); // interrupt assert
//	TEST_CHECK(p == 0);
//	p = m_at(vector_char, -1); // interrupt assert
//	TEST_CHECK(p == 0);       
	delete(vector_char);
}

TEST_LIST =
{
	{"new", test_new},
	{"vector_insert", test_insert},
	{"vector_push_back", test_push_back},
	{"vector_push_front", test_push_front},
	{"vector_load", test_load},
	{"vector_erase", test_erase},
	{"vector_clear", test_clear},
	{"vector_at", test_at},
	{"vector_size", test_size},
	{0, 0}
};
