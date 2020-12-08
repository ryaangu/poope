#ifndef CORE_STRING_BUILDER_HPP
#define CORE_STRING_BUILDER_HPP

#include <core/system.hpp>
#include <core/string.hpp>

// String Builder
struct StringBuilder
{
	int size;
	int room;

	char *data;

	// Initialize StringBuilder
	inline void init(int capacity = 255)
	{
		// Set size and room
		size = 0;
		room = capacity;

		// Allocate data
		data = reinterpret_cast<char *>(memory_allocate(room + 1));
	}

	// Push character
	inline void push(char character)
	{
		// Check for room
		if (size >= room)
		{
			// Add more room
			room <<= 1;

			// Reallocate
			data = reinterpret_cast<char *>(memory_reallocate(data, room + 1));
		}

		// Add character to data
		data[size++] = character;
	}

	// Push string
	inline void push(const char *string)
	{
		// Get string length
		int length = (string_length(string) - 1);

		// Check for room
		if ((size + length) >= room)
		{
			// Add more room
			room <<= 1;

			// Reallocate
			data = reinterpret_cast<char *>(memory_reallocate(data, room + 1));
		}

		// Add string to data
		for (int i = 0; i < length; ++i)
			data[size++] = string[i];
	}

	// Push substring
	inline void push(const char *string, int start, int end)
	{
		// Check for room
		if ((size + end) >= room)
		{
			// Add more room
			room <<= 1;

			// Reallocate
			data = reinterpret_cast<char *>(memory_reallocate(data, room + 1));
		}

		// Loop from start to end
		for (int i = start; i < end; ++i)
		{
			// Add character to data
			data[size++] = string[i];
		}
	}

	// Pull character from data
	inline char pull(int index)
	{
		return data[index];
	}

	// Free StringBuilder
	inline void free()
	{
		// Free data
		memory_free(data);
	}

	// Clear StringBuilder
	inline void clear()
	{
		size = 0;
	}

	// Get data as cstring
	inline operator const char *()
	{
		// Add null terminator
		data[size] = '\0';

		// Return the cstring
		return reinterpret_cast<const char *>(data);
	}
};

#endif