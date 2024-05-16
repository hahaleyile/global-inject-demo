#include "stdafx.h"
#include "logger.h"

HANDLE logSlot = NULL;
LPCTSTR slotName = TEXT("\\\\.\\mailslot\\{58EAAA50-B423-4AE6-8D4D-577380847A7F}");

namespace Logger
{
	void VLogLine(PCWSTR format, va_list args)
	{
		WCHAR buffer[1025];
		int len = _vsnwprintf_s(buffer, _TRUNCATE, format, args);
		if (len == -1) {
			// Truncation occurred.
			len = _countof(buffer) - 1;
		}

		while (--len >= 0 && buffer[len] == L'\n') {
			// Skip all newlines at the end.
		}

		// Leave only a single trailing newline.
		if (buffer[len + 1] == L'\n' && buffer[len + 2] == L'\n') {
			buffer[len + 2] = L'\0';
		}

		OutputDebugString(buffer);
	}

	void LogLine(PCWSTR format, ...)
	{
		va_list args;
		va_start(args, format);
		VLogLine(format, args);
		va_end(args);
	}

	void LogMailSlot(LPCWSTR lpszMessage)
	{
		if (!logSlot)
		{
			HANDLE hFile = CreateFile(slotName,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);

			if (hFile == INVALID_HANDLE_VALUE)
			{
				LOG("CreateFile failed with %d.\n", GetLastError());
				return;
			}
			else
			{
				logSlot = hFile;
			}
		}

		BOOL fResult;
		DWORD cbWritten;

		fResult = WriteFile(logSlot,
			lpszMessage,
			(DWORD)(lstrlenW(lpszMessage) + 1) * sizeof(WCHAR),
			&cbWritten,
			(LPOVERLAPPED)NULL);

		if (!fResult)
		{
			LOG("WriteFile failed with %d.\n", GetLastError());
			return;
		}

		LOG("Slot written to successfully.\n");
	}
}
