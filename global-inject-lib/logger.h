#pragma once

extern LPCTSTR slotName;
extern HANDLE logSlot;

namespace Logger
{
	void VLogLine(PCWSTR format, va_list args);
	void LogLine(PCWSTR format, ...);
	// Less than 424 bytes
	void LogMailSlot(LPCWSTR lpszMessage);
}

#define PRINT(message)		  Logger::LogMailSlot((message))
#define LOG(message, ...)     Logger::LogLine(L"[GLOBAL-INJECT-LOG]     [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
#define VERBOSE(message, ...) Logger::LogLine(L"[GLOBAL-INJECT-VERBOSE] [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
