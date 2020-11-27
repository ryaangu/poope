#include <core/string.hpp>

// Returns string length
int string_length(const char *string)
{
    // Get string base
    const char *base = string;

    // Traverse the string
    while (*base++);

    // Calculate the string length
    return static_cast<int>(base - string);
}