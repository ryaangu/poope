#ifndef COMPILER_COMPILER_HPP
#define COMPILER_COMPILER_HPP

#include <compiler/token.hpp>
#include <core/buffer.hpp>

// Compiler
struct Compiler
{
	Token previous;
	Token current;

	const char *path;

	int had_error;

	Buffer code;
};

extern Compiler g_compiler;

extern void compiler_init();
extern void compile(const char *path, const char *source);

#endif