#include <compiler/scanner.hpp>

#include <core/string.hpp>
#include <core/string_builder.hpp>
#include <core/hash_map.hpp>

// The global Scanner
Scanner g_scanner;

// Hash Map with keyword tokens
static HashMap<const char *, int> keywords_map;

// String Builder for keyword
static StringBuilder keyword_builder;

// Initialize the Scanner
void scanner_init(const char *source)
{
	// Prepare Scanner State
	g_scanner.start   = source;
	g_scanner.current = source;
	g_scanner.line    = 1;
	g_scanner.column  = 1;

	// Initialize keywords HashMap
	keywords_map.init();

	keywords_map.set("cpy", TOKEN_CPY);
	keywords_map.set("add", TOKEN_ADD);
	keywords_map.set("sub", TOKEN_SUB);
	keywords_map.set("mul", TOKEN_MUL);
	keywords_map.set("div", TOKEN_DIV);
	keywords_map.set("cmp", TOKEN_CMP);
	keywords_map.set("jmp", TOKEN_JMP);
	keywords_map.set("ret", TOKEN_RET);
	keywords_map.set("inc", TOKEN_INC);
	keywords_map.set("dec", TOKEN_DEC);

	keywords_map.set("reg0", TOKEN_REG0);
	keywords_map.set("reg1", TOKEN_REG1);
	keywords_map.set("reg2", TOKEN_REG2);
	keywords_map.set("reg3", TOKEN_REG3);
	keywords_map.set("reg4", TOKEN_REG4);
	keywords_map.set("reg5", TOKEN_REG5);
	keywords_map.set("reg6", TOKEN_REG6);
	keywords_map.set("reg7", TOKEN_REG7);

	// Initialize keyword StringBuilder
	keyword_builder.init();
}

// Advance the Scanner
static char advance()
{
	// Increase current pointer
	++g_scanner.current;

	// Return previous character
	return (g_scanner.current[-1]);
}

// Peek current character
static char peek()
{
	return (*g_scanner.current);
}

// Match current character?
static int match(char expected)
{
	// Check
	if (peek() != expected)
		return false;

	// Advance
	advance();
	return true;
}

// Is Scanner end?
static int is_end()
{
	return (peek() == '\0');
}

// Skip Whitespace
static void skip_whitespace()
{
	// Loop while current character is a whitespace
	for (;;)
	{
		// Get the current character
		char character = peek();

		// Check for whitespaces
		switch (character)
		{
			// Space
			case ' ':
			case '\r':
			case '\t':
			{
				// Advance
				advance();
				break;
			}

			// New line
			case '\n':
			{
				// Increase scanner line and reset column
				++g_scanner.line;
				g_scanner.column = 0;

				// Advance
				advance();
				break;
			}

			// Comment
			case ';':
			{
				// Advance until a new line
				while (!match('\n') && !is_end())
					advance();

				break;
			}

			// Not a space!
			default:
				return;
		}
	}
}

// Make Token
static Token make_token(int type)
{
	Token token;

	token.type   = type;
	token.start  = g_scanner.start;
	token.length = static_cast<int>(g_scanner.current - g_scanner.start);
	token.line   = g_scanner.line;
	token.column = g_scanner.column;

	return token;
}

// Make Error Token
static Token make_error_token(const char *message)
{
	Token token;

	token.type   = TOKEN_ERROR;
	token.start  = message;
	token.length = string_length(message);
	token.line   = g_scanner.line;
	token.column = g_scanner.column;

	return token;
}

// Get Identifier Type
static int identifier_type()
{
	// Get the keyword
	keyword_builder.clear();
	keyword_builder.push(g_scanner.start, 0, static_cast<int>(g_scanner.current - g_scanner.start));

	// Try finding keyword
	if (keywords_map.exists(keyword_builder))
	{
		// Found!
		return keywords_map.get(keyword_builder);
	}

	// No keyword found, return an identifier type.
	return TOKEN_IDENTIFIER;
}

// Make identifier
static Token make_identifier()
{
	// Loop while an alpha or digit
	while (is_alpha(peek()) || is_digit(peek()))
		advance();

	// Return Token
	return make_token(identifier_type());
}

// Make number
static Token make_number()
{
	// Loop while a digit
	while (is_digit(peek()))
		advance();

	// Return Token
	return make_token(TOKEN_NUMBER);
}

// Scan Token
Token scan_token()
{
	// Skip Whitespace
	skip_whitespace();

	// Set Scanner start pointer
	g_scanner.start = g_scanner.current;

	// End?
	if (is_end())
		return make_token(TOKEN_END);

	// Advance and get previous character
	char character = advance();

	// Literals
	if (is_digit(character))
		return make_number();

	if (is_alpha(character))
		return make_identifier();

	// Check character
	switch (character)
	{
		// Single Character
		case ',':
			return make_token(TOKEN_COMMA);

		case '.':
			return make_token(TOKEN_DOT);

		case ':':
			return make_token(TOKEN_COLON);

		case '?':
			return make_token(TOKEN_QUESTION);
	}

	// Unexpected Character Error!
	return make_error_token("Unexpected character.");
}