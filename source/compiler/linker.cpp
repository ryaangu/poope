#include <compiler/linker.hpp>
#include <compiler/compiler.hpp>
#include <compiler/PE.hpp>
#include <core/filesystem.hpp>
#include <core/string_builder.hpp>
#include <core/system.hpp>

#define FILE_ALIGNMENT    1 // 512
#define SECTION_ALIGNMENT 1 // 4096

#define OPTIONAL_HEADER_SIZE 240
#define SECTION_COUNT        1

// The global Linker
Linker g_linker;

// Write PE file
static void write_pe_file()
{
	// Replace <file>.asm with <file>.exe
	StringBuilder path_builder;
	path_builder.init();

	path_builder.push(g_linker.path);
	path_builder.size -= 2;

	path_builder.push("exe");

	// Open file
	File pe_file;
	file_open(pe_file, path_builder);

	// Get sections address
	unsigned int code_address = (456 + (SECTION_COUNT * sizeof(SectionHeader)));

	// Write MS-DOS Header
	MSDOSHeader msdos_header
	{
		IMAGE_DOS_SIGNATURE, // DOS Signature
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // UNUSED
		192 // PE File Header Address
	};

	file_write(pe_file, msdos_header, sizeof(MSDOSHeader));

	// Write MS-DOS Stub Program
	MSDOSStubProgram msdos_stub_program
	{
		{
			0xCD09B4000EBA1F0E,
			0x685421CD4C01B821,
			0x72676F7270207369,
			0x6F6E6E6163206D61,
			0x6E75722065622074,
			2329292586301155616,
			724249607997255533,
			36,
			0x946A3DD9C7045C9D,
			0x946A3DD9946A3DD9,
			0x946A3DDA956B5582,
			0x946A3DD8946B3DD9,
			0x946A3DD8956E4C61,
			0x946A3DD895684C61,
			0x946A3DD968636952,
			0x0000000000000000
		}
	};

	file_write(pe_file, msdos_stub_program, sizeof(MSDOSStubProgram));

	// Write PE File Header
	PEFileHeader pe_file_header
	{
		IMAGE_NT_SIGNATURE,                                          // PE Signature
		IMAGE_FILE_MACHINE_I386,                                    // Machine
		SECTION_COUNT,                                               // Number of Sections
		1606536130,                                                  // Time Date Stamp
		0,                                                           // UNUSED
		0,															 // UNUSED
		OPTIONAL_HEADER_SIZE,                                        // Optional Header Size
		IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE // Characteristics
	};

	file_write(pe_file, pe_file_header, sizeof(PEFileHeader));

	// Write PE File Optional Header
	PEFileOptionalHeader pe_file_optional_header
	{
		PE32_EXECUTABLE,                 // Magic
		0,                                    // Major Linker Version
		0,                                    // Minor Linker Version
		g_compiler.code_section.size,         // Code Size
		g_compiler.data_section.size,         // Data Size
		g_compiler.bss_section.size,          // BSS Size
		code_address + g_compiler.entry_point_address, // Entry Point Address
		code_address,                                  // Code Base
		0,                                    // Data Base

		IMAGE_BASE,        // Image Base
		SECTION_ALIGNMENT, // Section Alignment
		FILE_ALIGNMENT,    // FIle Alignment
		0,                 // Major Operating System Version
		0,                 // Minor Operating System Version
		1,                 // Major Image Version
		0,                 // Minor Image Version
		5,                 // Major Subsystem Version
		0,                 // Minor Subsystem Version
		0,                 // Windows Version
		4096,              // Image Size
		0,                 // Header Size
		0,                 // Checksum
		SUBSYSTEM_CONSOLE, // Subsystem
		0,                 // DLL Characteristics
		0x1000,            // Stack Reserve
		0x1000,            // Stack Commit
		0x10000,           // Heap Reserve
		0,                 // Heap Commit
		0,                 // Loader Flags
		16,                // Number of Data Directories

		// Data Directories
		{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 2 }}
	};

	file_write(pe_file, pe_file_optional_header, sizeof(PEFileOptionalHeader));

	// Write .text Section Header
	SectionHeader code_section_header
	{
		{ '.', 't', 'e', 'x', 't', 0, 0, 0 },                           // Name
		g_compiler.code_section.size,		                            // Virtual Size
		code_address,						                            // Virtual Address
		g_compiler.code_section.size,                                   // Size of Raw Data
		code_address,                                                   // Pointer to Raw Data
		{ 0, 0, 0 },                                                    // UNUSED
		IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ // Characteristics
	};

	file_write(pe_file, code_section_header, sizeof(SectionHeader));

	// Write .text Section Body
	for (int i = 0; i < g_compiler.code_section.size; ++i)
		file_write(pe_file, g_compiler.code_section.data[i], sizeof(unsigned char));

	// Close file
	file_close(pe_file);
}

// Link
void link(const char *path)
{
	// Initialize Linker
	g_linker.path = path;

	// Write the executable
	write_pe_file();
}