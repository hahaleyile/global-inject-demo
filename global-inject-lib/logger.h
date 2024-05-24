#pragma once
#include <queue>

extern handle_t g_hBinding;
extern RPC_CSTR pszStringBinding;

namespace Logger
{
	void VLogLine(PCWSTR format, va_list args);
	void LogLine(PCWSTR format, ...);
}
void LogRPC(PWSTR lpszMessage);

#define PRINT(message)		  LogRPC((message))
#define LOG(message, ...)     Logger::LogLine(L"[GLOBAL-INJECT-LOG]     [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
#define VERBOSE(message, ...) Logger::LogLine(L"[GLOBAL-INJECT-VERBOSE] [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
