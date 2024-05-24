#include "stdafx.h"
#include "logger.h"
#include "print.h"

handle_t g_hBinding = NULL;
RPC_CSTR pszStringBinding = NULL;

//DWORD WINAPI CompleteCall(LPVOID lpParam);

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
}

void LogRPC(PWSTR lpszMessage)
{
	RPC_STATUS status;

	if (!pszStringBinding) {
		unsigned char pszProtocolSequence[] = "ncalrpc";
		unsigned char pszEndpoint[] = "4CD673C2-36DB-4868-A14D-CE2CE1E0881F";
		status = RpcStringBindingComposeA(NULL,
			pszProtocolSequence,
			NULL,
			pszEndpoint,
			NULL,
			&pszStringBinding);
		if (status)
		{
			pszStringBinding = NULL;
			//MessageBoxA(NULL, "RpcStringBindingCompose failed", "Error", MB_OK);
			return;
		}
	}

	if (!g_hBinding)
	{
		status = RpcBindingFromStringBindingA(pszStringBinding, &g_hBinding);
		if (status)
		{
			g_hBinding = NULL;
			//MessageBoxA(NULL, "RpcBindingFromStringBinding failed", "Error", MB_OK);
			return;
		}
	}

	RPC_ASYNC_STATE async;
	status = RpcAsyncInitializeHandle(&async, sizeof(RPC_ASYNC_STATE));
	if (status)
	{
		//MessageBoxA(NULL, "RpcAsyncInitializeHandle failed", "Error", MB_OK);
		return;
	}
	async.UserInfo = NULL;
	async.NotificationType = RpcNotificationTypeEvent;
	async.u.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (async.u.hEvent == 0)
		async.NotificationType = RpcNotificationTypeNone;

	RpcTryExcept
	{
		PrintFile(&async, g_hBinding, lpszMessage);
	}
		RpcExcept(1)
	{
		//MessageBoxA(NULL, "PrintFile failed", "Error", MB_OK);
		return;
	}
	RpcEndExcept;	
	
	if (async.NotificationType == RpcNotificationTypeEvent)
	{
		WaitForSingleObject(async.u.hEvent, 1000 * 10);
		CloseHandle(async.u.hEvent);
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (RpcAsyncGetCallStatus(&async) == RPC_S_ASYNC_CALL_PENDING)
				Sleep(1000);
			else
				break;
		}
	}
	RpcAsyncCompleteCall(&async, NULL);

	//if (CreateThread(NULL, 0, CompleteCall, &async, 0, NULL) == NULL)
	//{
	//	//MessageBoxA(NULL, "CreateThread failed", "Error", MB_OK);
	//	return;
	//}
}

//DWORD WINAPI CompleteCall(LPVOID lpParam)
//{
//	RPC_ASYNC_STATE async = *(RPC_ASYNC_STATE*)lpParam;
//	if (async.NotificationType == RpcNotificationTypeEvent)
//	{
//		WaitForSingleObject(async.u.hEvent, 1000 * 10);
//		CloseHandle(async.u.hEvent);
//	}
//	else
//	{
//		for (int i = 0; i < 10; i++)
//		{
//			if (RpcAsyncGetCallStatus(&async) == RPC_S_ASYNC_CALL_PENDING)
//				Sleep(1000);
//			else
//				break;
//		}
//	}
//	RpcAsyncCompleteCall(&async, NULL);
//
//	return 0;
//}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr)
{
	free(ptr);
}
