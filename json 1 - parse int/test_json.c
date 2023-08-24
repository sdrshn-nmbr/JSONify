#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "miniunit.h"

// bool parse_int(int *a_value, const char **a_pos) -> declaration of parse_int

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
	mu_check(!is_success);	// because the input is valid
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
	mu_check(!is_success);		// because the input is valid
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
	mu_check(!is_success);	// because the input is valid
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
	mu_check(!is_success);		// because the input is valid
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
	mu_check(is_success);		// because the input is valid
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
	mu_check(is_success);		// because the input is valid
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
	mu_check(is_success);		// because the input is valid
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
	mu_check(is_success);		// because the input is valid
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
	mu_check(is_success);		// because the input is valid
	mu_check(pos == input + 4); 
	mu_check(result == -123);
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
	return EXIT_SUCCESS;
}
