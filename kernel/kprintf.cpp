#include <limits.h>
#include <stdarg.h>
#include <kernel/kstdio.h>
#include <string.h>
#include <stdlib.h>
 
bool kprint(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (kputchar(bytes[i]) == EOF)
			return false;
	return true;
}
 
int kprintf(const char* __restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(str, len))
				return -1;
			written += len;
		} else if(*format == 'd') {
			format++;
			char str[20];
			itoa((int)va_arg(parameters, int), str);
			size_t len = strlen(str);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if(!kprint(str, len))
				return -1;
			written += len;
		} else if(*format == 'x') {
			format++;
			char str[32];
			__itoa((int)va_arg(parameters, unsigned int), 16, str);
			size_t len = strlen(str);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if(!kprint(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}

bool soutprint(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (soutchar(bytes[i]) == EOF)
			return false;
	return true;
}
 
int sout(const char* __restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!soutprint(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!soutprint(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!soutprint(str, len))
				return -1;
			written += len;
		} else if(*format == 'd') {
			format++;
			char str[20];
			itoa((int)va_arg(parameters, int), str);
			size_t len = strlen(str);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if(!soutprint(str, len))
				return -1;
			written += len;
		} else if(*format == 'x') {
			format++;
			char str[32];
			__itoa((int)va_arg(parameters, unsigned int), 16, str);
			size_t len = strlen(str);
			if(maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if(!soutprint(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!soutprint(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}