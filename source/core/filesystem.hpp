#ifndef CORE_FILESYSTEM_HPP
#define CORE_FILESYSTEM_HPP

#include <windows.h>

// File
struct File
{
	HANDLE handle;
};

extern int  file_open(File &file, const char *path);
extern void file_close(File &file);

// Write to File
template<typename T> void file_write(File &file, T content, int length)
{
	// Write content to the file
	WriteFile(file.handle, &content, length, nullptr, nullptr);
}

#endif