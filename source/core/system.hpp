#ifndef CORE_SYSTEM_HPP
#define CORE_SYSTEM_HPP

extern void init();

extern void print(const char  *message);
extern void printl(const char *message);

extern void *memory_allocate(int size);
extern void *memory_reallocate(void *block, int size);
extern void  memory_free(void *block);

#endif