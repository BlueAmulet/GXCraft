#pragma once

bool netcat_init;
void netcat_console();
void netcat_close();
void netcat_log(const char* data);
void netcat_logf(const char* format, ...);
