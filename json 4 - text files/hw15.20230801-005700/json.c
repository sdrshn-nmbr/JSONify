#include "json.h"
#include "miniunit.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

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

void _print_ds(const Node *head, FILE *file)
{
    // account for nested lists and print them recursively as well
    if (head != NULL)
    {
        print_element_to_file(head->element, file);

        if (head->next != NULL)
        {
            fprintf(file, ", ");
            _print_ds(head->next, file);
        }
    }
}

void print_element_to_file(Element element, FILE *file)
{
    if (element.type == ELEMENT_INT)
    {
        fprintf(file, "%d", element.as_int);
    }
    else if (element.type == ELEMENT_STRING)
    {
        fprintf(file, "\"%s\"", element.as_string);
    }
    else if (element.type == ELEMENT_LIST)
    {
        if (element.as_list != NULL)
        {
            fprintf(file, "[");
            _print_ds(element.as_list, file);
            fprintf(file, "]");
        }
        else
        {
            fprintf(file, "[]"); // Empty list
        }
    }
    else if (element.type == ELEMENT_BOOL)
    {
        if (element.as_bool)
        {
            fprintf(file, "true");
        }
        else
        {
            fprintf(file, "false");
        }
    }
    else if (element.type == ELEMENT_NULL)
    {
        fprintf(file, "null");
    }
}

void print_element(Element element)
{
    print_element_to_file(element, stdout);
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

    // Check for empty list
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
    else if (**a_pos == 't' || **a_pos == 'f')
    {
        a_element->type = ELEMENT_BOOL;
        is_element_valid = parse_bool(&(a_element->as_bool), a_pos);
    }
    else if (**a_pos == 'n')
    {
        a_element->type = ELEMENT_NULL;
        is_element_valid = parse_null(a_pos);
    }
    // else
    // {
    //     // Initialize the as_list field to NULL for non-list elements
    //     a_element->type = ELEMENT_LIST;
    //     a_element->as_list = NULL;
    //     is_element_valid = false;
    // }

    return is_element_valid;
}

void write_json(char const *filename, Element element)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
        print_element_to_file(element, file);
        fclose(file);
    }
}

char *_read_chars_in_file(char const *filename)
{
    char *string = NULL;
    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        // Find the number of characters in the file
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        rewind(file);
        string = malloc(sizeof(*string) * (length + 1));
        if (string != NULL)
        {
            for (size_t i = 0; i < length; i++)
            {
                string[i] = fgetc(file);
            }
            string[length] = '\0';
        }
        fclose(file);
    }
    else
    {
        fprintf(stderr, "%s\n", strerror(errno));
    }
    return string;
}

bool read_json(char const *filename, Element *a_element)
{
    bool is_json = false;
    char *contents = _read_chars_in_file(filename);
    if (contents != NULL)
    {
        // Use a separate pointer to keep track of the parsing position, as otherwise, if you try to free contents later on after iterating over it, you will get an inalid free message as it then points to the null char at the end of the contents string
        const char *parse_position = contents;

        // Use parse_element to validate the JSON content.
        is_json = parse_element(a_element, &parse_position);

        if (is_json)
        {
            for (const char *ch = parse_position; *ch != '\0'; ch++)
            {
                if (!(isspace(*ch)) || (*ch != '\n') || (*ch != '\t'))
                {
                    fprintf(stderr, "Error: Unexpected %c in JSON file %s.\n", *ch, filename);
                    is_json = false;
                    break;
                }
            }
        }
        else
        {
            fprintf(stderr, "Error: %s does not contain a valid JSON element\n", filename);
        }
        free(contents);
    }
    return is_json;
}

bool parse_bool(bool *a_value, char const **a_pos)
{
    bool is_valid_bool = false;
    if (**a_pos == 't')
    {
        if (*(*a_pos + 1) == 'r' && *(*a_pos + 2) == 'u' && *(*a_pos + 3) == 'e')
        {
            *a_value = true;
            *a_pos += 4;
            is_valid_bool = true;
        }
    }
    else if (**a_pos == 'f')
    {
        if (*(*a_pos + 1) == 'a' && *(*a_pos + 2) == 'l' && *(*a_pos + 3) == 's' && *(*a_pos + 4) == 'e')
        {
            *a_value = false;
            *a_pos += 5;
            is_valid_bool = true;
        }
    }
    return is_valid_bool;
}

bool parse_null(char const **a_pos)
{
    bool is_valid_null = false;
    if (**a_pos == 'n')
    {
        if (*(*a_pos + 1) == 'u' && *(*a_pos + 2) == 'l' && *(*a_pos + 3) == 'l')
        {
            *a_pos += 4;
            is_valid_null = true;
        }
    }
    return is_valid_null;
}