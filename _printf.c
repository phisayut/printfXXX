#include "main.h"

void cleanup(va_list args, buffer_t *output);
int run_printf(const char *format, va_list args, buffer_t *output);
int printf(const char *format, ...);

/**
 * cleanup -cleans up ops for _printf
 * @args: va_list of args for _printf
 * @output: buffer_t struct
 */

void cleanup(va_list args, buffer_t *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 * run_printf - Reads through the format string for _printf
 * @format: character string to be printed
 * @output: A buffer_t struct
 * @args: A va_list of arguments
 * Return: the number of characters stored to output
 */
int run_printf(const char *format, va_list args, buffer_t *output)
{
	int i, wdth, presn, ret = 0;
	char tmp;
	unsigned char flgs, len;
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);

	for (i = 0; *(format + i); i++)
	{
		len = 0;
		if (*(format + i) == '%')
		{
			tmp = 0;
			flgs = handle_flags(format + i + 1, &tmp);
			wdth = handle_width(args, format + i + tmp + 1, &tmp);
			presn = handle_precision(args, format + i + tmp + 1, &tmp);
			f = handle_specifiers(format + i + tmp + 1);
			if (f != NULL)
			{
				i += tmp + 1;
				ret += f(args, output, flgs, wdth, presn, len);
				continue;
			}
			else if (*(format + i + tmp + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + i), 1);
		i += (len != 0) ? 1 : 0;
	}
	cleanup(args, output);
	return (ret);
}

/**
 * _printf - Outputs a formatted string.
 * @format: Character string to print - may contain directives.
 * Return: The number of characters printed.
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list args;
	int ret;

	if (format == NULL)
		return (-1);
	output = init_buffer();
	if (output == NULL)
		return (-1);

	va_start(args, format);

	ret = run_printf(format, args, output);

	return (ret);
}
