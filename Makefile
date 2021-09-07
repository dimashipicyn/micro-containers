VPATH = srcs:include

CC = clang
FLAGS = -Wall -Wextra -Werror -MMD # -fsanitize=address
SRCS = vector.c object.c
INC = vector.h object.h
INCLUDE = ./include
OBJ = $(SRCS:.c=.o)
DEPENDS = ${SRCS:.c=.d}
NAME = libminiooc.a

.PHONY: all clean fclean re

all: $(SRCS) $(NAME)

$(NAME): $(OBJ)
		ar rcs $(NAME) $(OBJ)

.c.o: $(SRCS)
		$(CC) $(FLAGS) -c -I$(INCLUDE) $<

clean:
		@rm -rf $(OBJ) $(DEPENDS)

fclean: clean
		@rm -rf $(NAME)

re: fclean all

gitadd: $(SRCS) $(INC)
	git add $^
	git add Makefile

-include ${DEPENDS}
