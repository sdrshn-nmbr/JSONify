#include <stdbool.h>
#include <string.h>
#include "clog.h"

#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#define mu_start() int __mu_first_failure_line_num = 0

#define mu_check(condition)                             \
	do                                                  \
	{                                                   \
		if (__mu_first_failure_line_num == 0)           \
		{                                               \
			if ((condition) == 0)                       \
			{                                           \
				__mu_first_failure_line_num = __LINE__; \
			}                                           \
		}                                               \
	} while (false)

#define mu_check_strings_equal(s1, s2) mu_check((strcmp(s1, s2)) == 0)

#define mu_end() return __mu_first_failure_line_num

#define mu_run(function)                                                               \
	do                                                                                 \
	{                                                                                  \
		int func_result = function();                                                  \
		if (func_result == 0)                                                          \
		{                                                                              \
			printf_color(GREEN, "Test passed: %s\n", #function);                       \
		}                                                                              \
		else                                                                           \
		{                                                                              \
			printf_color(RED, "Test failed: %s at line %d\n", #function, func_result); \
		}                                                                              \
	} while (false)

#endif /* end of include guard: __MINIUNIT_H__ */
