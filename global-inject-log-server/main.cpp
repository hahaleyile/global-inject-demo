#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

HANDLE hSlot, hLog;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\{58EAAA50-B423-4AE6-8D4D-577380847A7F}");

BOOL ReadSlot()
{
	DWORD cbMessage, cMessage, cbRead, cbWrite;
	BOOL fResult;
	LPWSTR lpszBuffer;
	DWORD cAllMessages;
	HANDLE hEvent;
	OVERLAPPED ov;

	cbMessage = cMessage = cbRead = cbWrite = 0;

	hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("MonitorSlot"));
	if (NULL == hEvent)
		return FALSE;
	ov.Offset = 0;
	ov.OffsetHigh = 0;
	ov.hEvent = hEvent;

	fResult = GetMailslotInfo(hSlot, // mailslot handle 
		(LPDWORD)NULL,               // no maximum message size 
		&cbMessage,                   // size of next message 
		&cMessage,                    // number of messages 
		(LPDWORD)NULL);              // no read time-out 

	if (!fResult)
	{
		printf("GetMailslotInfo failed with %d.\n", GetLastError());
		return FALSE;
	}

	// no new messages arrived
	if (cbMessage == MAILSLOT_NO_MESSAGE)
	{
		printf("Waiting for a message...\n");
		return TRUE;
	}

	cAllMessages = cMessage;

	while (cMessage != 0)  // retrieve all messages
	{
		// Allocate memory for the message. 

		lpszBuffer = (LPWSTR)GlobalAlloc(GPTR, cbMessage);
		if (NULL == lpszBuffer)
			return FALSE;
		lpszBuffer[0] = '\0';

		fResult = ReadFile(hSlot,
			lpszBuffer,
			cbMessage,
			&cbRead,
			&ov);

		if (!fResult)
		{
			printf("ReadFile failed with %d.\n", GetLastError());
			GlobalFree((HGLOBAL)lpszBuffer);
			return FALSE;
		}

		// Display the message. 

		if (!WriteFile(hLog, lpszBuffer, cbRead - 2, &cbWrite, NULL))
		{
			printf("WriteFile failed with %d.\n", GetLastError());
		}

		GlobalFree((HGLOBAL)lpszBuffer);

		fResult = GetMailslotInfo(hSlot,  // mailslot handle 
			(LPDWORD)NULL,               // no maximum message size 
			&cbMessage,                   // size of next message 
			&cMessage,                    // number of messages 
			(LPDWORD)NULL);              // no read time-out 

		if (!fResult)
		{
			printf("GetMailslotInfo failed (%d)\n", GetLastError());
			return FALSE;
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName)
{
	hSlot = CreateMailslot(lpszSlotName,
		0,                             // no maximum message size 
		MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
		(LPSECURITY_ATTRIBUTES)NULL); // default security

	if (hSlot == INVALID_HANDLE_VALUE)
	{
		printf("CreateMailslot failed with %d\n", GetLastError());
		return FALSE;
	}
	else printf("Mailslot created successfully.\n");
	return TRUE;
}

int main()
{
	hLog = CreateFileW(L"log.txt",               // open log.txt
		FILE_APPEND_DATA | FILE_GENERIC_READ,    // open for appending and locking
		FILE_SHARE_READ,                         // allow multiple readers
		NULL,                                    // no security
		OPEN_ALWAYS,                             // open or create
		FILE_ATTRIBUTE_NORMAL,                   // normal file
		NULL);                                   // no attr. template

	if (hLog == INVALID_HANDLE_VALUE)
	{
		printf("Could not open log.txt\n");
		return 1;
	}

	MakeSlot(SlotName);
	while (TRUE)
	{
		ReadSlot();
		Sleep(3000);
	}
	return 0;
}