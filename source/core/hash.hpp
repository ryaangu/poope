#ifndef CORE_HASH_HPP
#define CORE_HASH_HPP

#include <compiler/token.hpp>

// Hash integers
static inline unsigned int hash(int key)
{
	unsigned int hash = static_cast<unsigned int>(key);

	hash = (hash + 0x7ed55d16) + (hash << 12);
	hash = (hash ^ 0xc761c23c) ^ (hash >> 19);
	hash = (hash + 0x165667b1) + (hash << 5);
	hash = (hash + 0xd3a2646c) ^ (hash << 9);
	hash = (hash + 0xfd7046c5) + (hash << 3);
	hash = (hash ^ 0xb55a4f09) ^ (hash >> 16);

	return hash;
}

// Hash string
static inline unsigned int hash(const char *key)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

// Hash token
static inline unsigned int hash(const Token &key)
{
	unsigned int hash = 5381;
	int c;

	// The token start base
	const char *base = key.start;

	// The last token character
	char end = *(key.start + key.length);

	// Loop until last character
	while ((c = *(base++)) != end)
	{
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

#endif