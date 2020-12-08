#include <core/filesystem.hpp>
#include <windows.h>

// Write content to path
void write_file(const char *path, const char *content, int content_length)
{
	// Convert UTF-8 to UTF-16
	TCHAR tpath[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, path, -1, tpath, MAX_PATH);

	// Open a handle to the file
	HANDLE file_handle = CreateFileW(
		tpath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	// Check for valid handle
	if (file_handle == INVALID_HANDLE_VALUE)
		return;

	// Write content to the file
	WriteFile(file_handle, content, content_length, nullptr, nullptr);

	// Close the file handle
	CloseHandle(file_handle);
}