#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include <core/system.hpp>

extern int string_length(const char *string);
extern int string_equals(const char *a, const char *b);

extern int string_to_integer(const char *string);
extern int integer_to_string(char *buffer, int value);

// Is character a digit?
static inline int is_digit(char character)
{
	return (character >= '0' && character <= '9');
}

// Is character an alpha?
static inline int is_alpha(char character)
{
	return (character >= 'A' && character <= 'Z') ||
		   (character >= 'a' && character <= 'z') ||
		   (character == '_');
}

#endif