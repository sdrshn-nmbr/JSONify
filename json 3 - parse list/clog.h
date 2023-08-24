#ifndef __CLOG_H__
#define __CLOG_H__

#define debugf(...)                               \
	do                                            \
	{                                             \
		printf("\n[%s:%d] ", __FILE__, __LINE__); \
		printf(__VA_ARGS__);                      \
	} while (false)

#define ANSI_RESET "\x1B[0m"
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36

#define printf_color(ansi_color_code, ...)   \
	do                                       \
	{                                        \
		printf("\x1B[%dm", ansi_color_code); \
		printf(__VA_ARGS__);                 \
		printf(ANSI_RESET);                  \
	} while (false)

#define log_int(n) printf("%s == %d\n", (#n), (n))

#define log_char(ch) printf("%s == '%c'\n", (#ch), (ch))

#define log_str(st) printf("%s == \"%s\"\n", (#st), (st))

#define log_addr(ad) printf("%s == %p\n", (#ad), (void *)(ad))

#define log_float(fl) printf("%s == %.016f\n", (#fl), (fl))

#define log_bool(bl) printf("%s == %s\n", #bl, (bl ? "true" : "false"))

#endif /* end of include guarid: __CLOG_H__ */
