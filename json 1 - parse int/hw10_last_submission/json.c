#include "json.h"
#include "miniunit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool parse_int(int *a_value, const char **a_pos)
{
	bool is_beginning_int = false;
	if (**a_pos == '-')
	{
		(*a_pos)++;
		if (**a_pos >= '0' && **a_pos <= '9')
		{
			is_beginning_int = true;
			*a_value = -1 * (**a_pos - '0');
			(*a_pos)++;
			while (**a_pos >= '0' && **a_pos <= '9')
			{
				*a_value = (*a_value * 10) - (**a_pos - '0');
				(*a_pos)++;
			}
		}
	}
	else if (**a_pos >= '0' && **a_pos <= '9')
	{
		is_beginning_int = true;
		*a_value = **a_pos - '0';
		(*a_pos)++;
		while (**a_pos >= '0' && **a_pos <= '9')
		{
			*a_value = (*a_value * 10) + (**a_pos - '0');
			(*a_pos)++;
		}
	}
	return is_beginning_int;
}

