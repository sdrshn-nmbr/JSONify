#include "json.h"
#include "miniunit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool beginning_int(int *a_value, char const **a_pos) {
    *a_value = (**a_pos - '0');
    (*a_pos)++;
    while (isdigit(**a_pos))
    {
        *a_value = (*a_value * 10) + (**a_pos - '0');
        (*a_pos)++;
    }
    return true;
}

bool parse_int(int *a_value, char const **a_pos)
{
    bool is_beginning_int = false;
    if (**a_pos == '-')
    {
        (*a_pos)++;
        if (isdigit(**a_pos))
        {
            is_beginning_int = beginning_int(a_value, &(*a_pos));
        }
        *a_value = -(*a_value);
    }
    else if (isdigit(**a_pos))
    {
        is_beginning_int = beginning_int(a_value, &(*a_pos));
    }
    return is_beginning_int;
}


