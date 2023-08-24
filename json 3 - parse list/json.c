#include "json.h"
#include "miniunit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// typedef struct {
// 	enum  { ELEMENT_INT, ELEMENT_STRING, ELEMENT_LIST } type;
// 	union {
// 		int    as_int;
// 		char*  as_string;
// 		struct _Node* as_list; // for nested lists
// 	}; // ANONYMOUS union (C11).
// } Element;

// typedef struct _Node {
// 	struct _Node* next;
// 	Element element;
// } Node;

// both structs in header file

void _eat_whitespace(char const **a_pos)
{
    while (isspace(**a_pos))
    {
        (*a_pos)++;
    }
}

void _append(Node **a_head, Element element)
{
    Node *new_node = malloc(sizeof(*new_node));
    if (new_node != NULL)
    {
        new_node->element = element;
        new_node->next = NULL; // Initialize the 'next' pointer to NULL

        if (*a_head == NULL)
        {
            *a_head = new_node;
        }
        else
        {
            Node *node = *a_head;
            while (node->next != NULL)
            {
                node = node->next;
            }
            node->next = new_node;
        }
    }
}

bool _beginning_int(int *a_value, char const **a_pos)
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
    else
    {
        *a_pos = pos;
        (*a_pos)--; // to account for the increment in the for loop
        return is_valid_string;
    }
    (*a_pos)++;
    return is_valid_string;
}

void _free_list(Node *head)
{
    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        free_element(temp->element); // Free the current element
        free(temp);
    }
}

void free_element(Element element)
{
    if (element.type == ELEMENT_STRING)
    {
        free(element.as_string);
    }
    else if (element.type == ELEMENT_LIST)
    {
        _free_list(element.as_list); // Free nested lists recursively
    }
}

void _print_ds(const Node *head)
{
    // account for nested lists and print them recursively as well
    if (head != NULL)
    {
        print_element(head->element);

        if (head->next != NULL)
        {
            printf(", ");
            _print_ds(head->next);
        }
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
    else if (element.type == ELEMENT_LIST)
    {
        if (element.as_list != NULL)
        {
            printf("[");
            _print_ds(element.as_list);
            printf("]");
        }
        else
        {
            printf("[]"); // Empty list
        }
    }
}

bool parse_list(Node **a_head, char const **a_pos)
{
    bool is_valid_list = false;
    Element element = {.type = ELEMENT_INT, .as_int = 0}; // Initialize as an integer element

    if (**a_pos == '[')
    {
        (*a_pos)++;
        _eat_whitespace(a_pos);

        if (**a_pos == ']')
        {
            (*a_pos)++;
            *a_head = NULL; // Set the head of the list to NULL for an empty list
            return true;
        }

        while (parse_element(&element, a_pos))
        {
            _append(a_head, element);
            _eat_whitespace(a_pos);

            if (**a_pos == ',')
            {
                (*a_pos)++;
                _eat_whitespace(a_pos);
            }
            else if (**a_pos == ']')
            {
                (*a_pos)++;
                return true;
            }
            else
            {
                is_valid_list = false;
                break;
            }
        }
    }

    if (!is_valid_list)
    {
        // Free any previously parsed valid elements and nested lists
        _free_list(*a_head);
        *a_head = NULL;
    }
    
    return is_valid_list;
}

bool parse_element(Element *a_element, char const **a_pos)
{
    bool is_element_valid = false;
    _eat_whitespace(a_pos);

    // First, let's check for an empty list
    if (**a_pos == '[' && *(*a_pos + 1) == ']')
    {
        *a_element = (Element){.type = ELEMENT_LIST, .as_list = NULL};
        *a_pos += 2; // Skip the empty list "[]"
        return true;
    }

    if ((**a_pos == '-') || (isdigit(**a_pos)))
    {
        a_element->type = ELEMENT_INT;
        is_element_valid = parse_int(&(a_element->as_int), a_pos);
    }
    else if (**a_pos == '\"')
    {
        a_element->type = ELEMENT_STRING;
        is_element_valid = parse_string(&(a_element->as_string), a_pos);
    }
    else if (**a_pos == '[')
    {
        a_element->type = ELEMENT_LIST;
        a_element->as_list = NULL;                                   // Initialize as_list to NULL before parsing a new list
        is_element_valid = parse_list(&(a_element->as_list), a_pos); // Parse nested lists
    }
    else
    {
        // Initialize the as_list field to NULL for non-list elements
        a_element->type = ELEMENT_LIST;
        a_element->as_list = NULL;
        is_element_valid = false;
    }

    return is_element_valid;
}
