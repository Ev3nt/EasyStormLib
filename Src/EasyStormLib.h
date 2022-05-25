#pragma once

#include <Windows.h>
#include <string>

namespace Storm {
	class Archive {
	public:
		Archive(std::string name, DWORD priority = 13, DWORD flags = 0); // Open archive
		Archive(HANDLE handle); // Connect to opened archive
		Archive();
		~Archive(); // It's calling close if we're owner of archive

		void Close();
		std::string GetArchiveName();

		std::string operator[](std::string name); // Open file such as string
	private:
		HANDLE m_handle = NULL;
		bool owner = false; // We're owner, if we have opened archive
	};
}