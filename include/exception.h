#ifndef EXCEPTION_H
# define EXCEPTION_H
# include <setjmp.h>
# include <signal.h>
# include <assert.h>

// текст в си строку
# define str(s) #s

// max try blocks
# define MAX_CONTEXT 10

// исключения
typedef enum t_exception
{
	NO_EXCEPTION					= 0,
	ZERO_DIVISION_EXCEPTION			= 1,
	SEGMENTATION_FAULT_EXCEPTION	= 2,
	NUM_EXCEPTIONS
} t_exception;

void signalHandler(int sig);

extern t_exception	private_exc;
extern int			ctx_counter;
extern jmp_buf		private_env[MAX_CONTEXT];
extern char			*exception_name[NUM_EXCEPTIONS];

// регистратор исключений
#define try private_exc = setjmp(private_env[++ctx_counter]); if (private_exc == 0)

// обработчик исключения
#define catch(except_) else if (private_exc == except_)

// обработчик всех исключений
#define catchAll else if (private_exc != 0)

// сгенерировать исключение
#define throw(except_)\
			if (!ctx_counter) assert(!#except_); longjmp(private_env[ctx_counter], except_);

#define end ctx_counter--;

// возвращает строку с исключением
#define strexc() exception_name[private_exc]

// обертка над пользовательским main() для регистрации сигналов
#define main(...) usr_main(__VA_ARGS__);\
					int main(int ac, char **av) {\
						signal(SIGSEGV, signalHandler);\
						for(int i = 0; i < MAX_CONTEXT; i++) {\
							for (int j = 0; j < sizeof(jmp_buf); j++)\
								private_env[i][j] = 0;\
						}\
						return usr_main(ac, av);\
					}\
					int usr_main(__VA_ARGS__)


#endif
