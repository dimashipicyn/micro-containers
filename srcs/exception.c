#include "exception.h"

int			ctx_counter = 0;

// буфер для setjmp, longjmp
jmp_buf private_env[MAX_CONTEXT];

// текущее исключение
t_exception private_exc = 0;

// строковые представления исключений
char *exception_name[NUM_EXCEPTIONS] = {

    str(NO_EXCEPTION),
    str(ZERO_DIVISION_EXCEPTION),
	str(SEGMENTATION_FAULT_EXCEPTION)
};

// обработчик сигналов
void signalHandler(int sig)
{
	if (sig == SIGSEGV)
		throw(SEGMENTATION_FAULT_EXCEPTION);
}
