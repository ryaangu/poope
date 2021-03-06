#include <compiler/compiler.hpp>
#include <compiler/scanner.hpp>
#include <core/config.hpp>
#include <core/string.hpp>
#include <core/string_builder.hpp>
#include <core/hash_map.hpp>
#include <core/filesystem.hpp>

// The global Compiler
Compiler g_compiler;

// Hash Map with labels address
static HashMap<const char *, int> labels_map;

// String Builder for less allocation
static StringBuilder builder;

// Is inside a section?
static int is_data;
static int is_code;

// Initialize Compiler
void compiler_init()
{
	// Prepare Compiler State
	g_compiler.entry_point_address = 0;
	g_compiler.had_error           = false;

	g_compiler.bss_section.init();
	g_compiler.data_section.init();
	g_compiler.code_section.init();

	// Initialize labels map and builder
	labels_map.init();
	builder.init();
}

// Error
static void error(const char *message)
{
	// Error?
	if (g_compiler.had_error)
		return;

	// Output the error
	printf(stream_error, "%s -> Error: %s\n", g_compiler.path, message);

    // Set had error
    g_compiler.had_error = true;
}

// Error at Token
static void error_at(const Token &token, const char *message)
{
	// Error?
	if (g_compiler.had_error)
		return;

	// Output the error
	printf(stream_error, "%s (%d:%d) -> Error", g_compiler.path, token.line, token.column);

	if (token.type == TOKEN_END)
		printf(stream_error, " at end:");
	else if (token.type == TOKEN_ERROR)
		printf(stream_error, "");
	else
		printf(stream_error, " at '%.*s':", token.length, token.start);

	printf(stream_error, " %s\n", message);

    // Set had error
    g_compiler.had_error = true;
}

// Error at previous token
static inline void error_previous(const char *message)
{
    error_at(g_compiler.previous, message);
}

// Error at current token
static inline void error_current(const char *message)
{
    error_at(g_compiler.current, message);
}

// Advance scanner
static void advance()
{
    // Set previous token to the current token
    g_compiler.previous = g_compiler.current;

    // Advance the scanner and check for a token error_previous
    for (;;)
    {
        g_compiler.current = scan_token();
        if (g_compiler.current.type != TOKEN_ERROR) break;

        // Error
        error_current(g_compiler.current.start);
    }
}

// Consume token
static void consume(int type, const char *message)
{
    // Consume
    if (g_compiler.current.type == type)
    {
        advance();
        return;
    }

    // Error
    error_current(message);
}

// Check current type
static inline int check(int type)
{
    return (g_compiler.current.type == type);
}

// Match current type?
static int match(int type)
{
    // Not same type?
    if (!check(type))
    	return false;

    // Same type, advance
    advance();
    return true;
}

// Is token a register?
static int token_is_register(int type)
{
	return (type >= TOKEN_REG0 && type <= TOKEN_REG7);
}

// Get register value from token
static int token_get_register_value(int type)
{
	// Check type
	switch (type)
	{
		case TOKEN_REG0: return 0;
		case TOKEN_REG1: return 1;
		case TOKEN_REG2: return 2;
		case TOKEN_REG3: return 3;
		case TOKEN_REG4: return 4;
		case TOKEN_REG5: return 5;
		case TOKEN_REG6: return 6;
		case TOKEN_REG7: return 7;
	}

	// Nothing
	return 0;
}

// Get register mod from token
static int token_get_register_mod(int a, int b)
{
	// Get register values (as binary)
	a = (token_get_register_value(a) << 0);
	b = (token_get_register_value(b) << 0);

	// Calculate and return
	return (((0b11000 | b) << 3) | a);
}

// Emit byte
static void bss_emit_byte(unsigned char value)
{
	g_compiler.bss_section.write_byte(value);
}

static void data_emit_byte(unsigned char value)
{
	g_compiler.data_section.write_byte(value);
}

static void code_emit_byte(unsigned char value)
{
	g_compiler.code_section.write_byte(value);
}

// Emit dword
static void code_emit_dword(int value)
{
	g_compiler.code_section.write_byte((value >> 0)  & 0xff);
	g_compiler.code_section.write_byte((value >> 8)  & 0xff);
	g_compiler.code_section.write_byte((value >> 16) & 0xff);
	g_compiler.code_section.write_byte((value >> 24) & 0xff);
}

// Label
static void label(const Token &identifier_token)
{
	// Get the label name
	builder.clear();
	builder.push(identifier_token.start, 0, identifier_token.length);

	// Not inside code section?
	if (!is_code)
	{
		error_at(identifier_token, "Labels can't be outside code section.");
		return;
	}

	// Label already exists?
	if (labels_map.exists(builder))
	{
		error_at(identifier_token, "This label already exists.");
		return;
	}

	// Add label to map
	labels_map.set(builder, (g_compiler.code_section.size + 1));
}

// Identifier
static void identifier()
{
	// Get identifier token
	Token identifier_token = g_compiler.previous;

	// Label?
	if (match(TOKEN_COLON))
		label(identifier_token);
	else
		error_current("Expect label definition.");
}

// Section
static void section()
{
	// Expect section name
	consume(TOKEN_IDENTIFIER, "Expect section name after dot.");

	// Get section name token
	Token section_token = g_compiler.previous;

	// Get the section name
	builder.clear();
	builder.push(section_token.start, 0, section_token.length);

	// Check for valid section name
	if (string_equals(builder, "data"))
	{
		is_data = true;
		is_code = false;

		g_compiler.has_data = true;
	}
	else if (string_equals(builder, "code"))
	{
		is_code = true;
		is_data = false;

		g_compiler.has_code = true;
	}
	else
		error_at(section_token, "Expect data or code section.");
}

// Copy (cpy)
static void cpy()
{
	// Set copy instruction
	unsigned char copy_instruction = 0xb8;

	// Expect register
	advance();

	// Check for valid register
	if (!token_is_register(g_compiler.previous.type))
	{
		error_previous("Expect register after 'cpy'.");
		return;
	}

	// Increase copy instruction with register value
	copy_instruction += token_get_register_value(g_compiler.previous.type);

	// Store the register type (maybe the value will be also a register!)
	int register_type = g_compiler.previous.type;

	// Expect comma after register
	consume(TOKEN_COMMA, "Expect comma after register.");

	// Number value?
	if (match(TOKEN_NUMBER))
	{
		// Get the number value from token
		int value = string_to_integer(g_compiler.previous.start);

		// Emit bytes
		code_emit_byte(copy_instruction);
		code_emit_dword(value);
	}
	else
	{
		// Register?
		advance();

		if (token_is_register(g_compiler.previous.type))
		{
			// Get register mod from token
			int mod = token_get_register_mod(register_type, g_compiler.previous.type);

			// Emit bytes
			code_emit_byte(0x89);
			code_emit_byte(mod);
		}
		else
		{
			error_previous("Expect register or number value.");
		}
	}
}

// Jump (jmp)
static void jmp()
{
	// Expect label
	consume(TOKEN_IDENTIFIER, "Expect label after 'jmp'.");

	// Get label name
	builder.clear();
	builder.push(g_compiler.previous.start, 0, g_compiler.previous.length);

	// Label exists?
	if (!labels_map.exists(builder))
	{
		error_previous("This label does not exists.");
		return;
	}

	// Get label address
	int address = labels_map.get(builder);

	// Calculate jump address and encode to byte
	unsigned char jump_address = (address - (g_compiler.code_section.size + 1)) & 0xff;

	// Emit bytes
	code_emit_byte(0xeb);
	code_emit_byte(jump_address);
}

// Return (ret)
static void ret()
{
	// Emit byte
	code_emit_byte(0xc3);
}

// Increment (inc)
static void inc()
{
	// Set increment instruction
	unsigned char inc_instruction = 0x40;

	// Expect register
	advance();

	// Check for valid register
	if (!token_is_register(g_compiler.previous.type))
	{
		error_previous("Expect register after 'inc'.");
		return;
	}

	// Increase increment instruction with register value
	inc_instruction += token_get_register_value(g_compiler.previous.type);

	// Emit byte
	code_emit_byte(inc_instruction);
}

// Decrement (dec)
static void dec()
{
	// Set decrement instruction
	unsigned char dec_instruction = 0x48;

	// Expect register
	advance();

	// Check for valid register
	if (!token_is_register(g_compiler.previous.type))
	{
		error_previous("Expect register after 'dec'.");
		return;
	}

	// Increase decrement instruction with register value
	dec_instruction += token_get_register_value(g_compiler.previous.type);

	// Emit byte
	code_emit_byte(dec_instruction);
}

// Statement
static void statement()
{
	// Advance
	advance();

	// Check previous token type
	switch (g_compiler.previous.type)
	{
		// Identifier
		case TOKEN_IDENTIFIER:
		{
			identifier();
			break;
		}

		// Section
		case TOKEN_DOT:
		{
			section();
			break;
		}

		// Copy (cpy)
		case TOKEN_CPY:
		{
			cpy();
			break;
		}

		// Jump (jmp)
		case TOKEN_JMP:
		{
			jmp();
			break;
		}

		// Return (ret)
		case TOKEN_RET:
		{
			ret();
			break;
		}

		// Increment (inc)
		case TOKEN_INC:
		{
			inc();
			break;
		}

		// Decrement (dec)
		case TOKEN_DEC:
		{
			dec();
			break;
		}
	}
}

// Compile source
void compile(const char *path, const char *source)
{
	// Initialize Scanner
	scanner_init(source);

	// Initialize Compiler
	compiler_init();
	g_compiler.path = path;

	// Advance
    advance();

    // Statements
    while (!match(TOKEN_END) && !g_compiler.had_error)
        statement();

    // Expect code section
    if (!g_compiler.has_code)
    {
    	error("Expect code section.");
    	return;
    }

    // Expect entry point
    if (!labels_map.exists("init"))
    {
    	error("No entry point 'init' found.");
    	return;
    }

    // Get Entry Point Address
    g_compiler.entry_point_address = static_cast<unsigned int>(labels_map.get("init")) - 1;
}