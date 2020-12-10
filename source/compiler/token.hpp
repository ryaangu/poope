#ifndef COMPILER_TOKEN_HPP
#define COMPILER_TOKEN_HPP

#include <core/string.hpp>

// Token Type
enum TokenType : int
{
	// Sentinel
	TOKEN_ERROR,
    TOKEN_END,   // \0

	// Literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,

	// Single Character
    TOKEN_COMMA,    // ,
    TOKEN_DOT,      // .
    TOKEN_COLON,    // :
    TOKEN_QUESTION, // ?

	// Keywords
    TOKEN_CPY, // cpy
    TOKEN_ADD, // add
    TOKEN_SUB, // sub
    TOKEN_MUL, // mul
    TOKEN_DIV, // div
    TOKEN_CMP, // cmp
    TOKEN_JMP, // jmp
    TOKEN_RET, // ret
    TOKEN_INC, // inc
    TOKEN_DEC, // dec

    // Registers
    TOKEN_REG0, // reg0
    TOKEN_REG1, // reg1
    TOKEN_REG2, // reg2
    TOKEN_REG3, // reg3
    TOKEN_REG4, // reg4
    TOKEN_REG5, // reg5
    TOKEN_REG6, // reg6
    TOKEN_REG7, // reg7
};

// Token Structure
struct Token
{
    int type;
    int length;
    int line;
    int column;

    const char *start;

    // Default Constructor
    inline Token() {}

    // Make a token with content
    inline Token(const char *content)
    {
        start  = content;
        length = string_length(content);
    }
};

#endif