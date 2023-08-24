#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "miniunit.h"
// #include "json.c"

static int _test_parse_int_single_zero()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "0";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 1);
    mu_check(result == 0);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_single_char()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "A";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(!is_success);  // because the input is valid
    mu_check(pos == input); // failure should be at the first character in the input
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_negative_zero()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-0";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 2);
    mu_check(result == 0);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_negative_single_char()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-A";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(!is_success);      // because the input is valid
    mu_check(pos == input + 1); // failure should be at the first character in the input, not the negative sign
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_full_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "AAABBBCCC";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(!is_success); // because the input is valid
    mu_check(pos == input);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_negative_string_with_numbers()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-ABC123";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(!is_success); // because the input is valid
    mu_check(pos == input + 1);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_full_number()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "123456789";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 9);
    mu_check(result == 123456789);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_number_with_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "123ABC";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 3);
    mu_check(result == 123);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_negative_number_with_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-123ABC";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 4);
    mu_check(result == -123);
    // ──────────────────────────────────────────────────────
    mu_end();
}
static int _test_parse_int_negative_number_with_string_and_number()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-123ABC456";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 4);
    mu_check(result == -123);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_int_negative_number_with_string_and_number_and_negative_number()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    int result; // will be initialized in parse_int(…)
    char *input = "-123ABC-456";
    char const *pos = input;
    bool is_success = parse_int(&result, &pos);
    mu_check(is_success); // because the input is valid
    mu_check(pos == input + 4);
    mu_check(result == -123);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_int_plain()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char *input = "1";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_INT);
    mu_check(element.as_int == 1);
    mu_check(pos == input + 1);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_int_negative()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char *input = "-2";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_INT);
    mu_check(element.as_int == -2);
    mu_check(pos == input + 2);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_int_with_leading_whitespace()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char *input = "  1"; // two spaces before the 1
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_INT);
    mu_check(element.as_int == 1);
    mu_check(pos == input + 3);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_int_oddballs()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char *input = " 4 A";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_INT);
    mu_check(element.as_int == 4);
    mu_check(pos == input + 2);
    mu_check(*pos == ' ');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_int_invalid()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char *input = "--4";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(!is_success);
    mu_check(pos == input + 1);
    mu_check(*pos == '-');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_valid_empty()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("", result);
    mu_check(pos == input + 2);
    mu_check(*pos == '\0');
    free(result);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_valid_one_char()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"A\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("A", result);
    mu_check(pos == input + 3);
    mu_check(*pos == '\0');
    free(result);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_valid_multiple_chars()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"ABC\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("ABC", result);
    mu_check(pos == input + 5);
    mu_check(*pos == '\0');
    free(result);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_invalid()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"A";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(!is_success);
    mu_check(pos == input);
    mu_check(*pos == '\"');

    input = "\"A\nB\"";
    pos = input;
    is_success = parse_string(&result, &pos);
    mu_check(!is_success);
    mu_check(pos == input + 2);
    mu_check(*pos == '\n');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_quote_in_middle()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"A\"B\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("A", result);
    mu_check(pos == input + 3);
    mu_check(*pos == 'B');
    free(result);
    // ──────────────────────────────────────────────────────
    mu_end();
}

// More test for parse_string to get 100% coverage:

static int _test_parse_string_valid_with_escaped_characters()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"A\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("A", result);
    mu_check(pos == input + 3);
    mu_check(*pos == '\0');
    free(result);

    input = "\"A\\nB\""; // same as: {'\"', 'A', '\\', 'n', 'B', '\"'}
    pos = input;
    is_success = parse_string(&result, &pos);
    mu_check(is_success);
    mu_check_strings_equal("A\\nB", result);
    mu_check(pos == input + 6);
    mu_check(*pos == '\0');
    free(result);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_tab_escape_char()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"A\tB\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(!is_success);
    mu_check(pos == input + 2);
    mu_check(*pos == '\t');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_first_char_not_doubtle_quote()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "A\"";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(!is_success);
    mu_check(pos == input);
    mu_check(*pos == 'A');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_string_only_one_double_quote_with_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    char *result;
    char *input = "\"ABC";
    char const *pos = input;
    bool is_success = parse_string(&result, &pos);
    mu_check(!is_success);
    mu_check(pos == input);
    mu_check(*pos == '\"');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"ABC\"";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", element.as_string);
    mu_check(pos == input + 5);
    mu_check(*pos == '\0');
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_string_long()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"Hello, World!\"";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check_strings_equal("Hello, World!", element.as_string);
    mu_check(pos == input + 15);
    mu_check(*pos == '\0');
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_string_num_str()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"12345\"";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check_strings_equal("12345", element.as_string);
    mu_check(pos == input + 7);
    mu_check(*pos == '\0');
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_string_empty()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"\"";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check_strings_equal("", element.as_string);
    mu_check(pos == input + 2);
    mu_check(*pos == '\0');
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_string_more_escape_chars()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"Escape \\\"characters\\\"\"";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check_strings_equal("Escape \\", element.as_string);
    mu_check(pos == input + 10);
    mu_check(*pos == 'c');
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_element_does_not_call_any_parse_func()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "ABC";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(!is_success);
    mu_check(pos == input);
    mu_check(*pos == 'A');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static void _test_print_element()
{
    Element element;
    char const *input = "123";
    bool is_success = parse_element(&element, &input);
    if (is_success)
    {
        printf_color(GREEN, "_test_print_element_2: %s\n", is_success ? "true" : "false");
    }
    else
    {
        printf_color(RED, "_test_print_element_2: %s\n", is_success ? "true" : "false");
    }
    printf("Testing parse element(&element, \"123\")\n");
    printf(" - Expected: 123\n");
    printf(" - Actual:   ");
    print_element(element);
    fputc('\n', stdout);
    free_element(element);
}

static void _test_print_element_2()
{
    Element element;
    char const *input = "\"ABC\"";
    bool is_success = parse_element(&element, &input);
    if (is_success)
    {
        printf_color(GREEN, "_test_print_element_2: %s\n", is_success ? "true" : "false");
    }
    else
    {
        printf_color(RED, "_test_print_element_2: %s\n", is_success ? "true" : "false");
    }
    printf("Testing parse element(&element, \"\\\"ABC\\\"\")\n");
    printf(" - Expected: \"ABC\"\n");
    printf(" - Actual:   ");
    print_element(element);
    fputc('\n', stdout);
    free_element(element);
}

static int _test_parse_list_empty()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head == NULL);
    mu_check(pos == input + 2);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_single_int()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[1]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == 1);
    mu_check(pos == input + 3);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_multiple_ints()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[1, 2, 3]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == 1);
    mu_check(head->next != NULL);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 2);
    mu_check(head->next->next->element.as_int == 3);
    mu_check(pos == input + 9);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_multiple_ints_with_whitespace()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[ 1 , 2 , 3 ]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == 1);
    mu_check(head->next != NULL);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 2);
    mu_check(head->next->next->element.as_int == 3);
    mu_check(pos == input + 13);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_invalid_ints()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(!is_success);
    mu_check(head == NULL);
    mu_check(pos == input + 30);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_valid_ints_weird_whitespace()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[  0, 2,       3,4, 5, 6           , -7, 8,   9   , 10]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == 0);
    mu_check(head->next != NULL);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 2);
    mu_check(pos == input + strlen(input));
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_valid_one_string()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[\"ABC\"]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", head->element.as_string);
    mu_check(pos == input + 7);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_valid_multiple_strings()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[\"ABC\", \"DEF\", \"GHI\"]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", head->element.as_string);
    mu_check(head->next != NULL);
    mu_check(head->next->element.type == ELEMENT_STRING);
    mu_check_strings_equal("DEF", head->next->element.as_string);
    mu_check(head->next->next->element.type == ELEMENT_STRING);
    mu_check_strings_equal("GHI", head->next->next->element.as_string);
    mu_check(pos == input + 21);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_strings_and_ints_with_whitespace()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[\"ABC\", 1, \"DEF\", 2, \"GHI\", 3]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", head->element.as_string);
    mu_check(head->next != NULL);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 1);
    mu_check(head->next->next->element.type == ELEMENT_STRING);
    mu_check_strings_equal("DEF", head->next->next->element.as_string);
    mu_check(head->next->next->next->element.type == ELEMENT_INT);
    mu_check(head->next->next->next->element.as_int == 2);
    mu_check(head->next->next->next->next->element.type == ELEMENT_STRING);
    mu_check_strings_equal("GHI", head->next->next->next->next->element.as_string);
    mu_check(head->next->next->next->next->next->element.type == ELEMENT_INT);
    mu_check(head->next->next->next->next->next->element.as_int == 3);
    mu_check(pos == input + 30);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_invalid_int_and_no_bracket_after_comma()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10,";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(!is_success);
    mu_check(head == NULL);
    mu_check(pos == input + 31);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_valid_with_trailing_characters()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[\"ABC\", 1, \"DEF\", 2, \"GHI\", 3] 123";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(pos == input + 30);
    mu_check(*pos == ' ');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_empty_nested_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[]]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list == NULL);
    mu_check(pos == input + 4);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    head = NULL;
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_single_int_nested_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL; // Initialize head to NULL
    char const *input = "[[1]]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list != NULL);
    mu_check(head->element.as_list->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->element.as_int == 1);
    mu_check(pos == input + 5);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_multiple_ints_nested_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[1, 2, 3]]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list != NULL);
    mu_check(head->element.as_list->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->element.as_int == 1);
    mu_check(head->element.as_list->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->element.as_int == 2);
    mu_check(head->element.as_list->next->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->next->element.as_int == 3);
    mu_check(pos == input + 11);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_multiple_ints_nested_list_with_int()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[1, 2, 3], 4]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head != NULL);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list != NULL);
    mu_check(head->element.as_list->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->element.as_int == 1);
    mu_check(head->element.as_list->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->element.as_int == 2);
    mu_check(head->element.as_list->next->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->next->element.as_int == 3);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 4);
    mu_check(pos == input + 14);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_every_type_of_thing()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[1, 2, 3], \"ABC\", 4]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list != NULL);
    mu_check(head->element.as_list->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->element.as_int == 1);
    mu_check(head->element.as_list->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->element.as_int == 2);
    mu_check(head->element.as_list->next->next->element.type == ELEMENT_INT);
    mu_check(head->element.as_list->next->next->element.as_int == 3);
    mu_check(head->next->element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", head->next->element.as_string);
    mu_check(head->next->next->element.type == ELEMENT_INT);
    mu_check(head->next->next->element.as_int == 4);
    mu_check(pos == input + 21);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_empty_nested_list_before_int()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[], 1]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head->element.type == ELEMENT_LIST);
    mu_check(head->element.as_list == NULL);
    mu_check(head->next->element.type == ELEMENT_INT);
    mu_check(head->next->element.as_int == 1);
    mu_check(pos == input + 7);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_int_before_empty_nested_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[1, []]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == 1);
    mu_check(head->next->element.type == ELEMENT_LIST);
    mu_check(head->next->element.as_list == NULL);
    mu_check(pos == input + 7);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    head = NULL;
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_invalid_ints_nested_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(!is_success);
    mu_check(head == NULL);
    mu_check(pos == input + 32);
    mu_check(*pos == '\0');
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_close_bracket_str_literal()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[\"]\"]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head->element.type == ELEMENT_STRING);
    mu_check_strings_equal("]", head->element.as_string);
    mu_check(pos == input + 5);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    head = NULL;
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_list_negative_int_before_nested_str_list()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Node *head = NULL;
    char const *input = "[-1, [\"ABC\"]]";
    char const *pos = input;
    bool is_success = parse_list(&head, &pos);
    mu_check(is_success);
    mu_check(head->element.type == ELEMENT_INT);
    mu_check(head->element.as_int == -1);
    mu_check(head->next->element.type == ELEMENT_LIST);
    mu_check(head->next->element.as_list != NULL);
    mu_check(head->next->element.as_list->element.type == ELEMENT_STRING);
    mu_check_strings_equal("ABC", head->next->element.as_list->element.as_string);
    mu_check(pos == input + 13);
    mu_check(*pos == '\0');
    free_element((Element){.type = ELEMENT_LIST, .as_list = head});
    head = NULL;
    // ──────────────────────────────────────────────────────
    mu_end();
}

static void _test_print_element_empty_list()
{
    Element element;
    char const *input = "[]";
    bool is_success = parse_element(&element, &input);
    if (is_success)
    {
        printf_color(GREEN, "_test_print_element_empty_list: %s\n", is_success ? "true" : "false");
    }
    else
    {
        printf_color(RED, "_test_print_element_empty_list: %s\n", is_success ? "true" : "false");
    }
    printf("Testing parse element(&element, \"[]\")\n");
    printf(" - Expected: []\n");
    printf(" - Actual:   ");
    print_element(element);
    fputc('\n', stdout);
    free_element((Element){.type = ELEMENT_LIST, .as_list = element.as_list});
}

static void _test_print_element_simple_list()
{
    Element element;
    char const *input = "[1, 2, 3]";
    bool is_success = parse_element(&element, &input);
    if (is_success)
    {
        printf_color(GREEN, "_test_print_element_simple_list: %s\n", is_success ? "true" : "false");
    }
    else
    {
        printf_color(RED, "_test_print_element_simple_list: %s\n", is_success ? "true" : "false");
    }
    printf("Testing parse element(&element, \"[1, 2, 3]\")\n");
    printf(" - Expected: [1, 2, 3]\n");
    printf(" - Actual:   ");
    print_element(element);
    fputc('\n', stdout);
    free_element((Element){.type = ELEMENT_LIST, .as_list = element.as_list});
}

static void _test_print_element_nested_list()
{
    Element element;
    char const *input = "[[1, 2, 3], 4]";
    bool is_success = parse_element(&element, &input);
    if (is_success)
    {
        printf_color(GREEN, "_test_print_element_nested_list: %s\n", is_success ? "true" : "false");
    }
    else
    {
        printf_color(RED, "_test_print_element_nested_list: %s\n", is_success ? "true" : "false");
    }
    printf("Testing parse element(&element, \"[[1, 2, 3], 4]\")\n");
    printf(" - Expected: [[1, 2, 3], 4]\n");
    printf(" - Actual:   ");
    print_element(element);
    fputc('\n', stdout);
    free_element((Element){.type = ELEMENT_LIST, .as_list = element.as_list});
}

static int _test_print_element_to_file_stdout()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "123";
    bool is_success = parse_element(&element, &input);
    mu_check(is_success);
    printf("Testing parse element to file (&element, \"123\")\n");
    printf(" - Expected: 123\n");
    printf(" - Actual:   ");
    print_element_to_file(element, stdout);
    printf("\n");
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_print_element_to_file_seperate_txt_file()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "123";
    bool is_success = parse_element(&element, &input);
    mu_check(is_success);
    FILE *file = fopen("test_file.txt", "a");
    fprintf(file, "Testing parse element to file (&element, \"123\")\n");
    fprintf(file, " - Expected: 123\n");
    fprintf(file, " - Actual:   ");
    print_element_to_file(element, file);
    fprintf(file, "\n\n");
    fclose(file);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_write_json_simple()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "\"ABC\"";
    bool is_success = parse_element(&element, &input);
    mu_check(is_success);
    write_json("test_file.txt", element);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_read_json_str()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    bool is_success = read_json("test_file.txt", &element);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_STRING);
    mu_check(element.as_string != NULL);
    mu_check_strings_equal(element.as_string, "ABC");
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_write_json_and_read_json_int()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "123";
    bool is_success = parse_element(&element, &input);
    mu_check(is_success);
    write_json("test_file.txt", element);
    free_element(element);
    is_success = read_json("test_file.txt", &element);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_INT);
    mu_check(element.as_int == 123);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_bool_true()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "true huehuehue";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_BOOL);
    mu_check(element.as_bool == true);
    mu_check(pos = input + 4);
    mu_check(*pos == ' ');
    print_element(element);
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_bool_false()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "false hehehehe";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_BOOL);
    mu_check(element.as_bool == false);
    mu_check(pos = input + 5);
    mu_check(*pos == ' ');
    print_element(element);
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_bool_false_invalid()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "falshuehuehue";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(!is_success);
    mu_check(pos = input + 4);
    mu_check(*pos == 'h');
    debugf("pos: %c\n", *(pos));
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_bool_false_invalid_2()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "farue";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(!is_success);
    mu_check(pos = input + 2);
    mu_check(*pos == 'r');
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_bool_true_invalid()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "truhuehuehue";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(!is_success);
    mu_check(pos = input + 3);
    mu_check(*pos == 'h');
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_parse_null()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    char const *input = "nulliifyy";
    char const *pos = input;
    bool is_success = parse_element(&element, &pos);
    mu_check(is_success);
    mu_check(element.type == ELEMENT_NULL);
    mu_check(pos = input + 4);
    mu_check(*pos == 'i');
    print_element(element);
    fputc('\n', stdout);
    free_element(element);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_read_json_first_error_file_not_found()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    bool is_success = read_json("file_not_found.txt", &element);
    mu_check(!is_success);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_read_json_second_error_invalid_json()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    bool is_success = read_json("invalid_2.txt", &element);
    mu_check(!is_success);
    // ──────────────────────────────────────────────────────
    mu_end();
}

static int _test_read_json_third_error_non_whitespace_trailing_chars()
{
    mu_start();
    // ──────────────────────────────────────────────────────
    Element element;
    bool is_success = read_json("invalid_3.txt", &element);
    mu_check(!is_success);
    // ──────────────────────────────────────────────────────
    mu_end();
}

int main(int argc, char *argv[])
{
    mu_run(_test_parse_int_single_zero);
    mu_run(_test_parse_int_single_char);
    mu_run(_test_parse_int_negative_zero);
    mu_run(_test_parse_int_negative_single_char);
    mu_run(_test_parse_int_full_string);
    mu_run(_test_parse_int_negative_string_with_numbers);
    mu_run(_test_parse_int_full_number);
    mu_run(_test_parse_int_number_with_string);
    mu_run(_test_parse_int_negative_number_with_string);
    mu_run(_test_parse_int_negative_number_with_string_and_number);
    mu_run(_test_parse_int_negative_number_with_string_and_number_and_negative_number);

    mu_run(_test_parse_element_int_plain);
    mu_run(_test_parse_element_int_negative);
    mu_run(_test_parse_element_int_with_leading_whitespace);
    mu_run(_test_parse_element_int_oddballs);
    mu_run(_test_parse_element_int_invalid);

    mu_run(_test_parse_string_valid_empty);
    mu_run(_test_parse_string_valid_one_char);
    mu_run(_test_parse_string_valid_multiple_chars);
    mu_run(_test_parse_string_invalid);
    mu_run(_test_parse_string_quote_in_middle);
    mu_run(_test_parse_string_valid_with_escaped_characters);
    mu_run(_test_parse_string_tab_escape_char);
    mu_run(_test_parse_string_first_char_not_doubtle_quote);
    mu_run(_test_parse_string_only_one_double_quote_with_string);

    mu_run(_test_parse_element_string);
    mu_run(_test_parse_element_string_long);
    mu_run(_test_parse_element_string_num_str);
    mu_run(_test_parse_element_string_empty);
    mu_run(_test_parse_element_string_more_escape_chars);
    mu_run(_test_parse_element_does_not_call_any_parse_func);

    _test_print_element();
    _test_print_element_2();

    mu_run(_test_parse_list_empty);
    mu_run(_test_parse_list_single_int);
    mu_run(_test_parse_list_multiple_ints);
    mu_run(_test_parse_list_multiple_ints_with_whitespace);
    mu_run(_test_parse_list_invalid_ints);
    mu_run(_test_parse_list_valid_ints_weird_whitespace);
    mu_run(_test_parse_list_valid_one_string);
    mu_run(_test_parse_list_valid_multiple_strings);
    mu_run(_test_parse_list_strings_and_ints_with_whitespace);
    mu_run(_test_parse_list_invalid_int_and_no_bracket_after_comma);
    mu_run(_test_parse_list_valid_with_trailing_characters);

    mu_run(_test_parse_list_empty_nested_list);
    mu_run(_test_parse_list_single_int_nested_list);
    mu_run(_test_parse_list_multiple_ints_nested_list);
    mu_run(_test_parse_list_multiple_ints_nested_list_with_int);
    mu_run(_test_parse_list_every_type_of_thing);
    mu_run(_test_parse_list_empty_nested_list_before_int);
    mu_run(_test_parse_list_int_before_empty_nested_list);
    mu_run(_test_parse_list_invalid_ints_nested_list);
    mu_run(_test_parse_list_close_bracket_str_literal);
    mu_run(_test_parse_list_negative_int_before_nested_str_list);

    _test_print_element_empty_list();
    _test_print_element_simple_list();
    _test_print_element_nested_list();

    mu_run(_test_print_element_to_file_stdout);
    mu_run(_test_print_element_to_file_seperate_txt_file);

    mu_run(_test_write_json_simple);

    mu_run(_test_read_json_str);

    mu_run(_test_write_json_and_read_json_int);

    mu_run(_test_parse_bool_true);
    mu_run(_test_parse_bool_false);
    mu_run(_test_parse_bool_false_invalid);
    mu_run(_test_parse_bool_false_invalid_2);
    mu_run(_test_parse_bool_true_invalid);

    mu_run(_test_parse_null);

    mu_run(_test_read_json_first_error_file_not_found);
    mu_run(_test_read_json_second_error_invalid_json);
    mu_run(_test_read_json_third_error_non_whitespace_trailing_chars);

    return EXIT_SUCCESS;
}
