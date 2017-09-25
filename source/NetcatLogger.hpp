#pragma once

namespace Netcat {
	extern bool init;
	void console();
	void accept();
	void close();
	void log(const char* data);
	void logf(const char* format, ...);
};
