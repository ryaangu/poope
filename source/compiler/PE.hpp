#ifndef COMPILER_PE_HPP
#define COMPILER_PE_HPP

#include <core/system.hpp>

// PE File Format Macros
#define IMAGE_DOS_SIGNATURE 0x5a4d
#define IMAGE_NT_SIGNATURE  0x00004550

#define IMAGE_FILE_MACHINE_UNKNOWN 0x0
#define IMAGE_FILE_MACHINE_AMD64   0x8664
#define IMAGE_FILE_MACHINE_I386    0x014c

#define PE32_EXECUTABLE      0x10b
#define PE32_PLUS_EXECUTABLE 0x20b

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

// MS-DOS Header
struct MSDOSHeader
{
	unsigned short signature;
	unsigned short unused[29];
	unsigned int   header_address;
};

// MS-DOS Stub Program
struct MSDOSStubProgram
{
	unsigned long long data[16];
};

// PE File Header
struct PEFileHeader
{
	unsigned int       signature;
	unsigned short     machine;
	unsigned short     section_count;
	unsigned int       time_date_stamp;
	unsigned int       unused;
	unsigned int       unused2;
	unsigned short     optional_header_size;
	unsigned short     characteristics;
};

// Data Directory
struct DataDirectory
{
	unsigned int address;
	unsigned int size;
};

// PE File Optional Header
struct PEFileOptionalHeader
{
	unsigned short magic;
	unsigned char  major_linker_version;
	unsigned char  minor_linker_version;
	unsigned int   code_size;
	unsigned int   data_size;
	unsigned int   bss_size;
	unsigned int   entry_point_address;
	unsigned int   code_base;
	unsigned int   data_base;

	unsigned int       image_base;
	unsigned int       section_alignment;
	unsigned int       file_alignment;
	unsigned short     major_operating_system;
	unsigned short     minor_operating_system;
	unsigned short     major_image_version;
	unsigned short     minor_image_version;
	unsigned short     major_subsystem_version;
	unsigned short     minor_subsystem_version;
	unsigned int       windows_version;
	unsigned int       image_size;
	unsigned int       header_size;
	unsigned int       checksum;
	unsigned short     subsystem;
	unsigned short     dll_characteristics;
	unsigned long long stack_reserve;
	unsigned long long stack_commit;
	unsigned long long heap_reserve;
	unsigned long long heap_commit;
	unsigned int       loader_flags;
	unsigned int       data_directories_count;

	DataDirectory data_directories[16];
};

// Section Header
struct SectionHeader
{
	unsigned char name[8];
	unsigned int  virtual_size;
	unsigned int  virtual_address;
	unsigned int  raw_size;
	unsigned int  raw_address;
	unsigned int  unused[3];
	unsigned int  characteristics;
};

// Section Body
struct SectionBody
{
	unsigned char *data;

	unsigned int size;
	unsigned int room;

	// Initialize
	inline void init()
	{
		size = 0;
		room = 255;
		data = reinterpret_cast<unsigned char *>(memory_allocate(room));
	}

	// Write unsigned char
	inline void write_byte(unsigned char value)
	{
		// Check for room
		if (size >= room)
		{
			// Reallocate
			room <<= 1;
			data   = reinterpret_cast<unsigned char *>(memory_reallocate(data, room));
		}

		// Add unsigned char to data
		data[size++] = value;
	}
};

#endif