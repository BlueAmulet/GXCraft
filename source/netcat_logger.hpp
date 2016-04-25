#pragma once

class Netcat {
	public:
		static bool init;
		static void console();
		static void accept();
		static void close();
		static void log(const char* data);
		static void logf(const char* format, ...);
};
