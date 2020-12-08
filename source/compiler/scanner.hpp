#ifndef COMPILER_SCANNER_HPP
#define COMPILER_SCANNER_HPP

#include <compiler/token.hpp>

// Scanner
struct Scanner
{
	const char *start;
	const char *current;

	int line;
	int column;
};

extern Scanner g_scanner;

extern void  scanner_init(const char *source);
extern Token scan_token();

#endif