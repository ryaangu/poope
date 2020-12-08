#ifndef CORE_SYSTEM_HPP
#define CORE_SYSTEM_HPP

// Streams
enum Stream : int
{
	stream_output,
	stream_error,
};

extern void init();

extern void print(char character, int stream = stream_output);
extern void printl(char character, int stream = stream_output);

extern void print(const char  *message, int stream = stream_output);
extern void printl(const char *message, int stream = stream_output);

extern void print(int integer, int stream = stream_output);
extern void printl(int integer, int stream = stream_output);

extern void printf(int stream, const char *format, ...);

extern void *memory_allocate(int size);
extern void *memory_reallocate(void *block, int size);
extern void  memory_free(void *block);

extern int memory_equals(const void *a, const void *b, unsigned long long size);

#endif