#pragma once
#include <zmq.h>

extern void* zmqPusher, * zmqContext;

namespace Logger
{
	void VLogLine(PCWSTR format, va_list args);
	void LogLine(PCWSTR format, ...);
	void LogZeroMQ(LPCWSTR lpszMessage);
}

#define PRINT(message)		  Logger::LogZeroMQ((message))
#define LOG(message, ...)     Logger::LogLine(L"[GLOBAL-INJECT-LOG]     [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
#define VERBOSE(message, ...) Logger::LogLine(L"[GLOBAL-INJECT-VERBOSE] [%S]: " message L"\n", __FUNCTION__, __VA_ARGS__)
