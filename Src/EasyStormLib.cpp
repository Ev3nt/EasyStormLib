#include "EasyStormLib.h"
#include "StormLib.h"

namespace Storm {
	Archive::Archive() {
		m_handle = NULL;
		m_owner = false;
	}

	Archive::~Archive() {
		if (m_owner) {
			Close();
		}
	}

	void Archive::Open(std::string name, DWORD priority, DWORD flags) {
		if (StormOpenArchive(name.c_str(), priority, flags, &m_handle)) {
			m_owner = true;
		}
	}

	void Archive::Connect(HANDLE handle) {
		m_handle = handle;
		m_owner = false;
	}

	void Archive::Close(bool force) {
		if (m_owner || (!m_owner && force)) {
			if (m_handle) {
				StormCloseArchive(m_handle);
			}
		}

		m_handle = NULL;
	}

	std::string Archive::GetArchiveName() {
		char name[MAX_PATH];
		FillMemory(name, sizeof(name), 0);

		if (m_handle) {
			StormGetArchiveName(m_handle, name, sizeof(name));
		}

		return name;
	}

	std::string Archive::GetArchiveName(std::string fileName) {
		char name[MAX_PATH];
		FillMemory(name, sizeof(name), 0);

		HANDLE _handle;
		if (StormOpenFile(fileName.c_str(), &_handle)) {
			HANDLE _archive;
			if (StormGetFileArchive(_handle, &_archive)) {
				StormGetArchiveName(_archive, name, sizeof(name));
			}

			StormCloseFile(_handle);
		}

		return name;
	}

	std::string Archive::operator[](std::string name) {
		std::string filedata;

		HANDLE _handle = NULL;

		if (m_handle) {
			StormOpenFileEx(m_handle, name.c_str(), 0, &_handle);
		}
		else {
			StormOpenFile(name.c_str(), &_handle);
		}

		if (_handle) {
			SIZE_T high; // Idk how i can use it on x32, so maximum size limit is 4gb
			SIZE_T size = StormGetFileSize(_handle, &high);

			SIZE_T readed; // Useless shit, cause we have checked file size
			filedata.resize(size);
			StormReadFile(_handle, (char*)filedata.data(), size, &readed, 0);
			StormCloseFile(_handle);
		}

		return filedata;
	}
}
