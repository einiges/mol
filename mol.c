#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "arg.h"

#define SIGN(str) ((str[0] == '-') ? '-' : '+')
#define SIGNED(str) ((str[0] == '-') || (str[0] == '+'))
#define NEGATIVE(str) (str[0] == '-')

char base_char_limit(int base, bool uppercase);
void usage(int status);
void die(const char *errstr, ...);

char *argv0;

int
main(int argc, char *argv[])
{
	int width;
	int base = 10;
	bool base_uppercase = false;
	bool display_return = true;
	bool display_indicator = true;
	bool include_indicator = true;
	bool indicator_first = false;
	char *more_indicator = "+";
	char *less_indicator = "-";


	ARGBEGIN {
	case 'B':
		base_uppercase = true;
	case 'b':
		errno = 0;
		base = strtol(EARGF(usage(EXIT_FAILURE)), NULL, 10);
		if (errno == ERANGE || 2 > base || base > 36)
			die("base out of range.\n");
		break;
	case 'f':
		indicator_first = true;
		break;
	case 'l':
		less_indicator = EARGF(usage(EXIT_FAILURE));
		break;
	case 'm':
		more_indicator = EARGF(usage(EXIT_FAILURE));
		break;
	case 'n':
		display_return = false;
		break;
	case 's':
		display_indicator = false;
		break;
	case 'x':
		include_indicator = false;
		break;
	case 'h':
		usage(EXIT_SUCCESS);
		break;
	case 'v':
		fprintf(stderr, VERSION "\n");
		exit(EXIT_SUCCESS);
		break;
	} ARGEND;

	if (argc < 1)
		usage(EXIT_FAILURE);

	char *endptr;
	errno = 0;
	width = strtol(argv[0], &endptr, 10);
	if (errno == ERANGE)
		die("width is out of range.\n");
	if (endptr == argv[0])
		die("width is not a number.\n");

	if (include_indicator && display_indicator && width > 0 &&
	   (width < strlen(more_indicator) || width < strlen(less_indicator)))
		die("width is smaller than the length of at least one indicator\n");

	int cin_buf_siz = width + 2;
	char sin[cin_buf_siz];
	char *cnum;

	if (argc > 1)
	{
		cnum = argv[1];
	}
	else
	{
		if (!fgets(sin, cin_buf_siz, stdin))
			die("Input Required\n");
		sin[strcspn(sin, "\r\n")] = '\0';
		cnum = sin;
	}

	if (width <= 0)
		goto end;


	if (strlen(cnum) <= width)
	{
		printf("%s", cnum);
		goto end;
	}


	int w = width;
	char *indicator = NEGATIVE(cnum) ? less_indicator : more_indicator;

	if (include_indicator && display_indicator)
		w -= strlen(indicator);

	if (display_indicator && indicator_first)
		printf("%s", indicator);

	if (w > 0 && SIGNED(cnum))
	{
		putchar(SIGN(cnum));
		--w;
	}

	for (; w > 0; --w)
		putchar(base_char_limit(base, base_uppercase));

	if (display_indicator && !indicator_first)
		printf("%s", indicator);


end:
	if (display_return)
		putchar('\n');

	return EXIT_SUCCESS;
}


/*
 * Get the highest possible digit of a base.
 * example: 2->1, 16->f
 *
 * returns: 0 if base is not in the range of [2, 36].
 *
 * base: base to specify the highest digit.
 * uppercase: if true char will be returned in uppercase,
 *            otherwise lowercase.
 */
char
base_char_limit(int base, bool uppercase)
{
	if (2 > base || base > 36)
		return 0;

	char c = "123456789abcdefghijklmnopqrstuvwxyz"[base - 2];
	if (uppercase && c > '9')
		c -= 32;

	return c;
}


/*
 * Prints usage to stderr and exists.
 *
 * status: exit status
 */
void
usage(int status)
{
	fprintf(stderr, "usage: %s [-fnsxhv] [-bB base] [-l less_indicator] "
	                "[-m more_indicator] width [number]\n", argv0);
	exit(status);
}


/*
 * Prints to stderr and exits with EXIT_FAILURE.
 */
void
die(const char *errstr, ...)
{
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

