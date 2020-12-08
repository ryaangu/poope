#ifndef CORE_COMPARE_HPP
#define CORE_COMPARE_HPP

#include <core/string.hpp>
#include <core/hash.hpp>

// All types
template <typename T> static inline int compare(T a, T b)
{
	return (a == b);
}

// Strings
template <> inline int compare(const char *a, const char *b)
{
	return ((a == b) || string_equals(a, b));
}

// Tokens
template <> inline int compare(Token a, Token b)
{
	// Compare length
	if (a.length != b.length)
		return false;

	// Hash tokens
	unsigned int a_hash = hash(a);
	unsigned int b_hash = hash(b);

	// Compare hashs
	if (a_hash != b_hash)
		return false;

	return true;
}

#endif