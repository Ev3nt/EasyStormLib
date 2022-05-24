#include "EasyStormLib.h"
#include "StormLib.h"

namespace Storm {
	Archive::Archive(std::string name, DWORD priority, DWORD flags): owner(true) {
		StormOpenArchive(name.c_str(), priority, flags, &m_handle);
	}

	Archive::Archive(HANDLE handle): m_handle(handle) {

	}

	Archive::~Archive() {
		if (owner) {
			Close();
		}
	}

	void Archive::Close() {
		if (m_handle) {
			StormCloseArchive(m_handle);
			m_handle = NULL;
		}
	}

	std::string Archive::GetArchiveName() {
		char name[MAX_PATH];
		FillMemory(name, sizeof(name), 0);

		if (m_handle) {
			StormGetArchiveName(m_handle, name, sizeof(name));
		}

		return name;
	}

	std::string Archive::operator[](std::string name) {
		std::string filedata;

		if (m_handle) {
			HANDLE _handle;
			if (StormOpenFileEx(m_handle, name.c_str(), 0, &_handle)) {
				SIZE_T high; // Idk how i can use it on x32, so maximum size limit is 4gb
				SIZE_T size = StormGetFileSize(_handle, &high); 

				SIZE_T readed; // Useless shit, cause we have checked file size
				char* buffer = new char[size];
				FillMemory(buffer, size, 0);
				StormReadFile(_handle, buffer, size, &readed, 0);
				StormCloseFile(_handle);

				filedata.append(buffer, size);
				delete[] buffer;
			}
		}

		return filedata;
	}
}