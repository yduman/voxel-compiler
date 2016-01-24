#pragma once

#include <ostream>

struct FileGuard {
	FILE *fp;

	inline FileGuard(FILE *_fp) : fp(_fp)
	{
	}

	inline ~FileGuard()
	{
		if (fp)
			fclose(fp);
	}

	inline FileGuard(const FileGuard &other) = delete;
	inline FileGuard &operator=(FileGuard other) = delete;
};

struct Writer {
	std::ostream &os;

	inline Writer(std::ostream &_os) : os(_os)
	{
	}

	inline Writer &operator<<(uint32_t val)
	{
		os.write((const char*)&val, sizeof(uint32_t));
		return *this;
	}
};