#include <core/string.hpp>

// Reverse buffer
static void reverse(char *buffer, int size)
{
	int  i, j;
	char c;

	for (i = 0, j = size - 1; i < j; ++i, --j)
	{
		c         = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = c;
	}
}

// String to integer
int string_to_integer(const char *string)
{
    int result = 0;
    int sign   = 1;

    // Check for sign
    if (*string == '-')
    {
        sign = -1;
        ++string;
    }

    // Calculate integer
    while (*string)
    {
        // Is digit?
        if (is_digit(*string))
        {
            // Calculate
            result = result * 10 + (*string - '0');
        }
        else
        {
            // Stop calculating and return the result we have so far
            return (result * sign);
        }

        // Advance
        ++string;
    }

    // Return the result
    return (result * sign);
}

// Integer to string
int integer_to_string(char *buffer, int value)
{
    int index = 0;
    int sign  = false;

    // Check for sign
    if (value < 0)
    {
        value = -value;
        sign  = true;
    }

    // Write integers
    do
    {
        buffer[index++] = (value % 10) + '0';
    }
    while ((value /= 10) > 0);

    // Write sign?
    if (sign)
    {
        buffer[index++] = '-';
    }

    // Terminate buffer
    buffer[index] = '\0';

    // Reverse buffer
    reverse(buffer, index);

    // Return the index
    return index;
}

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

// Compare strings
int string_equals(const char *a, const char *b)
{
	// Get both strings length
	int a_length = string_length(a);
	int b_length = string_length(b);

	// Length not the same?
	if (a_length != b_length)
		return false;

	// Loop and check for same characters
	for (int i = 0; i < a_length; ++i)
	{
		// Not the same character?
		if (a[i] != b[i])
			return false;
	}

	// Both strings are equals!
	return true;
}