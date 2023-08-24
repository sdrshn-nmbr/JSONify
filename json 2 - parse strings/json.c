#include "json.h"
#include "miniunit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

bool _beginning_int(int *a_value, char const **a_pos) // helper function that stores the valid int
{
    *a_value = (**a_pos - '0');
    (*a_pos)++;
    while (isdigit(**a_pos))
    {
        *a_value = (*a_value * 10) + (**a_pos - '0');
        (*a_pos)++;
    }
    return true; // only calling this function if the first character is a digit, and hence will always return true
}

bool parse_int(int *a_value, char const **a_pos)
{
    bool is_beginning_int = false;
    if (**a_pos == '-')
    {
        (*a_pos)++;
        if (isdigit(**a_pos))
        {
            is_beginning_int = _beginning_int(a_value, &(*a_pos));
        }
        *a_value = -(*a_value);
    }
    else if (isdigit(**a_pos))
    {
        is_beginning_int = _beginning_int(a_value, &(*a_pos));
    }
    return is_beginning_int;
}

bool parse_string(char **a_string, const char **a_pos)
{
    bool is_valid_string = false;
    const char *pos = *a_pos;
    if (*pos != '\"')
    {
        return is_valid_string;
    }
    (*a_pos)++;
    pos++;

    const char *end = strchr(pos, '\"');
    if (end != NULL)
    {
        char *newLinePtr = strchr(pos, '\n');
        // printf("%c\n", *newLinePtr);
        if (newLinePtr != NULL && newLinePtr < end)
        {
            (*a_pos)++;
            return is_valid_string;
        }
        char *tabPtr = strchr(pos, '\t');
        if (tabPtr != NULL && tabPtr < end)
        {
            (*a_pos)++;
            return is_valid_string;
        }
        int length = end - pos;
        char *string = malloc((length + 1) * sizeof(*string));
        strncpy(string, pos, length);
        string[length] = '\0';

        *a_string = string;
        *a_pos = end;
        is_valid_string = true;
    }
    (*a_pos)++;
    return is_valid_string;
}

bool parse_element(Element *a_element, char const **a_pos)
{
    bool is_beginning_valid = false;
    while (isspace(**a_pos)) // eats up any whitespace before the element
    {
        (*a_pos)++;
    }
    if ((**a_pos == '-') || (isdigit(**a_pos)))
    {
        (*a_element).type = ELEMENT_INT;
        is_beginning_valid = parse_int(&((*a_element).as_int), a_pos);
    }
    else if (**a_pos == '\"')
    {
        (*a_element).type = ELEMENT_STRING;
        is_beginning_valid = parse_string(&((*a_element).as_string), a_pos);
    }
    else
    {
        is_beginning_valid = false;
    }
    return is_beginning_valid;
}

void free_element(Element element)
{
    if (element.type == ELEMENT_STRING)
    {
        free(element.as_string);
    }
}

void print_element(Element element)
{
    if (element.type == ELEMENT_INT)
    {
        printf("%d", element.as_int);
    }
    else if (element.type == ELEMENT_STRING)
    {
        printf("\"%s\"", element.as_string);
    }
}


