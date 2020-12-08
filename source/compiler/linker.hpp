#ifndef COMPILER_LINKER_HPP
#define COMPILER_LINKER_HPP

#include <core/buffer.hpp>

// Linker
struct Linker
{
	const char *path;

	Buffer executable;
};

extern Linker g_linker;

extern void linker_init();
extern void link(const char *path);

#endif