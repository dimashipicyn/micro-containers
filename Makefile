VPATH = srcs:include

CC = clang
FLAGS = -Wall -Wextra -Werror -MMD # -fsanitize=address
SRCS = vector.c object.c exception.c
INC = vector.h object.h exception.h
INCLUDE = ./include
OBJ = $(SRCS:.c=.o)
DEPENDS = ${SRCS:.c=.d}
NAME = libminiooc.a

.PHONY: all clean fclean re git test

all: $(SRCS) $(NAME)

test: all
	$(CC) test.c -I$(INCLUDE) $(NAME)
	./a.out

examples: all
	$(CC) main_object_example.c -o object_example -I$(INCLUDE) $(NAME)
	$(CC) main_vector_example.c -o vector_example -I$(INCLUDE) $(NAME)
	$(CC) main_exception_example.c -o exception_example -I$(INCLUDE) $(NAME)
	./object_example
	./vector_example
	./exception_example

$(NAME): $(OBJ)
		ar rcs $(NAME) $(OBJ)

.c.o: $(SRCS)
		$(CC) $(FLAGS) -c -I$(INCLUDE) $<

clean:
		@rm -rf $(OBJ) $(DEPENDS)

fclean: clean
		@rm -rf $(NAME)

re: fclean all

git: $(SRCS) $(INC)
	git add $^
	git add Makefile main_object_example.c main_vector_example.c main_exception_example.c README.md

-include ${DEPENDS}
