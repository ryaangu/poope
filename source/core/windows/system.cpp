#include <core/system.hpp>
#include <core/string.hpp>

#include <windows.h>

// Heap handle
static HANDLE heap_handle;

// Output handle
static HANDLE output_handle;

// The linker just wants to see this symbol since we use floating-point.
extern "C" int _fltused = 0;

// Main Windows Function
extern "C" void WinMainCRTStartup()
{
    // Get heap handle
    heap_handle = GetProcessHeap();

    // Get output handle
    output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Initialize program
    init();
    
    // Success
    ExitProcess(EXIT_SUCCESS);
}

// Outputs message to console
void print(const char *message)
{
    WriteConsoleA(output_handle, message, string_length(message) - 1, nullptr, nullptr);
}

// Outputs message to console with '\n'
void printl(const char *message)
{
    print(message);
    print("\n");
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

	// Success
	return memory;
}

// Free memory
void memory_free(void *block)
{
	HeapFree(heap_handle, 0, block);
}