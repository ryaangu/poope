#include <core/filesystem.hpp>
#include <windows.h>

// Open File
int file_open(File &file, const char *path)
{
	// Convert UTF-8 to UTF-16
	TCHAR tpath[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, path, -1, tpath, MAX_PATH);

	// Open a handle to the file
	file.handle = CreateFileW(
		tpath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	// Return
	return (file.handle != INVALID_HANDLE_VALUE);
}

// Close File
void file_close(File &file)
{
	// Close the file handle
	CloseHandle(file.handle);
}