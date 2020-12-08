#ifndef CORE_BUFFER_HPP
#define CORE_BUFFER_HPP

#include <core/system.hpp>
#include <core/math.hpp>

// Buffer
struct Buffer
{
	int offset;

	int size;
	int room;

	unsigned char *data;

	// Initialize Buffer
	inline void init()
	{
		offset = 0;

		size = 0;
		room = 256;

		// Allocate data
		data = reinterpret_cast<unsigned char *>(memory_allocate(room));
	}

	// Write bytes to the buffer
	inline void write_bytes(unsigned long long value, int amount)
	{
		// Check for room
		if ((size + amount) >= room)
		{
			// Get new room size
			room <<= 1;

			// Reallocate data
			data = reinterpret_cast<unsigned char *>(memory_reallocate(data, room));
		}

		// Encode first byte
		data[size++] = (value >> 0) & 0xff;

		// Encode the other bytes
		for (int i = 1; i < amount; ++i)
			data[size++] = (value >> (8 * i)) & 0xff;
	}

	// Read bytes from the buffer
	inline unsigned long long read_bytes(int amount)
	{
		// Increase offset
		offset += amount;

		// Decode first byte
		unsigned long long value = (data[offset - amount] << 0);

		// Decode the other bytes
		for (int i = 1; i < amount; ++i)
			value |= (data[offset - (amount - i)]) << (8 * i);

		// Return the value
		return value;
	}

	// Write byte
	inline void write_byte(int value)
	{
		write_bytes(value, 1);
	}

	// Read byte
	inline int read_byte()
	{
		return read_bytes(1);
	}

	// Write word
	inline void write_word(int value)
	{
		write_bytes(value, 2);
	}

	// Read word
	inline int read_word()
	{
		return read_bytes(2);
	}

	// Write dword
	inline void write_dword(int value)
	{
		write_bytes(value, 4);
	}

	// Read dword
	inline int read_dword()
	{
		return read_bytes(4);
	}

	// Write qword
	inline void write_qword(unsigned long long value)
	{
		write_bytes(value, 8);
	}

	// Read qword
	inline int read_qword()
	{
		return read_bytes(8);
	}

	// Free Buffer
	inline void free()
	{
		memory_free(data);
	}
};

#endif