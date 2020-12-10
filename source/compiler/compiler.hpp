#ifndef COMPILER_COMPILER_HPP
#define COMPILER_COMPILER_HPP

#include <compiler/token.hpp>
#include <compiler/PE.hpp>

// Compiler
struct Compiler
{
	Token previous;
	Token current;

	const char *path;

	int has_bss;
	int has_data;
	int has_code;
	int had_error;

	SectionBody bss_section;
	SectionBody data_section;
	SectionBody code_section;

	unsigned int entry_point_address;
};

extern Compiler g_compiler;

extern void compiler_init();
extern void compile(const char *path, const char *source);

#endif