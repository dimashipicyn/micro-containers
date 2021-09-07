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
		try
		{
			int *p = 0;
			int a = *p; // segmentation fault
		}
		catchAll
		{
			printf("Segmentation fault handler\n");
		}
		end
		div(0); // throw zero division

		// no print
		printf("no print\n");
	}
	catch(ZERO_DIVISION_EXCEPTION)
	{
		printf("error %s\n", strexc());
	}
	catchAll
	{
		printf("error %s\n", strexc());
	}
	end

	printf("end program\n");

	return 0;
}
