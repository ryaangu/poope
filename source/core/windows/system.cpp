#include <core/config.hpp>
#include <core/system.hpp>
#include <core/string.hpp>
#include <windows.h>
#include <cstdarg>

// Heap handle
static HANDLE heap_handle;

// Output handle
static HANDLE output_handle;

// Error handle
static HANDLE error_handle;

// The linker just wants to see this symbol since we use floating-point.
extern "C" int _fltused = 0;

// Main Windows Function
extern "C" void WinMainCRTStartup()
{
    // Get heap handle
    heap_handle = GetProcessHeap();

    // Get output handle
    output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get error handle
    error_handle = GetStdHandle(STD_ERROR_HANDLE);

    // Initialize program
    init();

    // Success
    ExitProcess(EXIT_SUCCESS);
}

// Outputs character to console
void print(char character, int stream)
{
	char data[2] { character, '\0' };
	print(reinterpret_cast<const char *>(data), stream);
}

// Outputs character to console with '\n'
void printl(char character, int stream)
{
	print(character, stream);
	print("\n",	     stream);
}

// Outputs message to console
void print(const char *message, int stream)
{
    WriteConsoleA((stream == stream_output) ? output_handle : error_handle, message, string_length(message) - 1, nullptr, nullptr);
}

// Outputs message to console with '\n'
void printl(const char *message, int stream)
{
    print(message, stream);
    print("\n",    stream);
}

// Outputs number to console
void print(int integer, int stream)
{
	// Convert integer to a string
	char value[255];
	integer_to_string(value, integer);

	// Output number string to console
	print(reinterpret_cast<const char *>(value), stream);
}

// Outputs number to console with '\n'
void printl(int integer, int stream)
{
	print(integer, stream);
	print("\n",    stream);
}

// Outputs formatted message to console
void printf(int stream, const char *format, ...)
{
	// Start va
	va_list args;
	va_start(args, format);

	// Loop until a null terminator
	while (*format != '\0')
	{
		// Check characters
		switch (*format)
		{
			// Formatting
			case '%':
			{
				// Check formatting type
				switch (*(++format))
				{
					// Character (%c)
					case 'c':
					{
						print(static_cast<char>(va_arg(args, int)), stream);
						break;
					}

					// Decimal (%d)
					case 'd':
					{
						// Convert decimal to string
						char buffer[10];
						integer_to_string(buffer, va_arg(args, int));

						// Print
						print(reinterpret_cast<const char *>(buffer), stream);
						break;
					}

					// String with length (%.*s)
					case '.':
					{
						if (*(++format) == '*' && *(++format) == 's')
						{
							int length         = va_arg(args, int);
							const char *string = va_arg(args, const char *);

							// Print each character until length
							for (int i = 0; i < length; i++)
								print(string[i], stream);
						}
						break;
					}

					// String (%s)
					case 's':
					{
						print(va_arg(args, const char *), stream);
						break;
					}
				}

				break;
			}

			// Character
			default:
			{
				print(*format, stream);
				break;
			}
		}

		// Advance
		++format;
	}

	// End va
	va_end(args);
}

// Allocate memory
void *memory_allocate(int size)
{
	void *memory;

	// Allocate memory
	memory = HeapAlloc(heap_handle, 0, size);

	// Validate memory
	if (memory == nullptr)
	    ExitProcess(1);

	// Debug
	#ifdef DEBUG
	print("Debug: Allocation - Size: ");
	printl(size);
	#endif

	// Success
	return memory;
}

// Reallocate memory
void *memory_reallocate(void *block, int size)
{
	void *memory;

	// Reallocate memory
	memory = HeapReAlloc(heap_handle, 0, block, size);

	// Validate memory
	if (memory == nullptr)
	    ExitProcess(1);

	// Debug
	#ifdef DEBUG
	print("Debug: Reallocation - Size: ");
	printl(size);
	#endif

	// Success
	return memory;
}

// Free memory
void memory_free(void *block)
{
	HeapFree(heap_handle, 0, block);

	// Debug
	#ifdef DEBUG
	printl("Debug: Free!");
	#endif
}

// Compare memory blocks
int memory_equals(const void *a, const void *b, unsigned long long size)
{
	// Cast
	const unsigned char *c = reinterpret_cast<const unsigned char *>(a);
	const unsigned char *d = reinterpret_cast<const unsigned char *>(b);

	// Loop and check for same bytes
	for (int i = 0; i < size; ++i)
	{
		// Not the same byte?
		if (c[i] != d[i])
			return false;
	}

	// Both memory blocks are equals!
	return true;
}