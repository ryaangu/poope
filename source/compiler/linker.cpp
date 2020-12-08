#include <compiler/linker.hpp>
#include <compiler/compiler.hpp>
#include <core/filesystem.hpp>
#include <core/string_builder.hpp>
#include <core/system.hpp>

// PE File Format Macros
#define IMAGE_DOS_SIGNATURE 0x5a4d
#define IMAGE_NT_SIGNATURE  0x00004550

#define IMAGE_FILE_MACHINE_AMD64 0x8664

#define PE32  0x10b
#define PE32P 0x20b

#define IMAGE_FILE_EXECUTABLE_IMAGE    0x0002
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 0x0020

#define IMAGE_SCN_MEM_WRITE   0x80000000
#define IMAGE_SCN_MEM_READ    0x40000000
#define IMAGE_SCN_MEM_EXECUTE 0x20000000
#define IMAGE_SCN_MEM_SHARED  0x10000000

#define IMAGE_SCN_CNT_CODE				 0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA	 0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA 0x00000080

#define SUBSYSTEM_CONSOLE 0x03

#define IMAGE_BASE 0x00400000

#define FILE_ALIGNMENT    1 // 512
#define SECTION_ALIGNMENT 1 // 4096

// Assembly-like Functions Short Names
#define db(value) g_linker.executable.write_byte(value);
#define dw(value) g_linker.executable.write_word(value);
#define dd(value) g_linker.executable.write_dword(value);
#define dq(value) g_linker.executable.write_qword(value);

#define tdb(value, times) g_linker.executable.write_bytes(value, times);

// The global Linker
Linker g_linker;

// Initialize the Linker
void linker_init()
{
	// Initialize Linker Buffer
	g_linker.executable.init();
}

// Write PE file
static void write_pe_file()
{
	// MS-DOS Header
	dw (IMAGE_DOS_SIGNATURE); // DOS Signature ('MZ')
	tdb (0, 29 * 2);          // UNUSED
	dd (192);                 // PE File Header Address

	// MS-DOS Stub Program
	dq (0xCD09B4000EBA1F0E);
	dq (0x685421CD4C01B821);
	dq (0x72676F7270207369);
	dq (0x6F6E6E6163206D61);
	dq (0x6E75722065622074);
	dq (2329292586301155616);
	dq (724249607997255533);
	dq (36);
	dq (0x946A3DD9C7045C9D);
	dq (0x946A3DD9946A3DD9);
	dq (0x946A3DDA956B5582);
	dq (0x946A3DD8946B3DD9);
	dq (0x946A3DD8956E4C61);
	dq (0x946A3DD895684C61);
	dq (0x946A3DD968636952);
	dq (0x0000000000000000);

	// PE File Signature
	dd (IMAGE_NT_SIGNATURE); // 'PE\0\0'

	// PE File Header
	dw (IMAGE_FILE_MACHINE_AMD64);                                     // Machine
	dw (0);      			                                           // Number of Sections
	dd (1606536130);                                                   // Time Date Stamp
	dq (0);			                                                   // UNUSED
	dw (240);                                                          // Optional Header Size
	dw (IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE); // Characteristics

	// PE File Optional Header
	dw (PE32P);                // Magic
	dw (0);                    // Linker Version
	dd (g_compiler.code.size); // Code Size
	dd (0);                    // Data Size
	dd (0);                    // Bss Size
	dd (456);                  // Entry Point Address
	dd (456);                  // Base of Code
	dd (0);                    // Base of Data

	dd (IMAGE_BASE);        // Image Base
	dd (SECTION_ALIGNMENT); // Section Alignment
	dd (FILE_ALIGNMENT);    // File Alignment
	dd (0);                 // Operating System
	dd (0);                 // Image Version
	dw (5);                 // Major Subsystem Version
	dw (0);                 // Minor Subsystem Version
	dd (0);                 // Windows Version
	dd (1024);              // Image Size
	dd (0);                 // Header Size
	dd (0);                 // Checksum
	dw (SUBSYSTEM_CONSOLE); // Subsystem
	dw (0);                 // DLL Characteristics
	dq (0x1000);            // Size Of Stack Reserve
    dq (0x1000);            // Size Of Stack Commit
    dq (0x10000);           // Size Of Heap Reserve
    dq (0);                 // Size Of Heap Commit
    dd (0);                 // Loader Flags
    dd (16);                // Number of Data Directories

    // Data Directories
    tdb (0, 16 * 8);

    // Machine Code
    for (int i = 0; i < g_compiler.code.size; ++i)
    	db (g_compiler.code.data[i]);

	// Write the file
	StringBuilder path_builder;
	path_builder.init();

	path_builder.push(g_linker.path);
	path_builder.push(".exe");

	printl(g_linker.executable.size);
	write_file(path_builder, reinterpret_cast<const char *>(g_linker.executable.data), g_linker.executable.size);
}

// Link
void link(const char *path)
{
	// Initialize Linker
	linker_init();
	g_linker.path = path;

	// Write the executable
	write_pe_file();
}