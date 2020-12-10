#ifndef COMPILER_LINKER_HPP
#define COMPILER_LINKER_HPP

// Linker
struct Linker
{
	const char *path;
};

extern Linker g_linker;

extern void link(const char *path);

#endif