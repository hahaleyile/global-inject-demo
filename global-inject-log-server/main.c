#include <stdio.h>
#include <Windows.h>
#include "print.h"

// https://devblogs.microsoft.com/oldnewthing/20050912-14/?p=34253
HANDLE hLog, ghMutex;
void* context, * server;

void PrintFile(PRPC_ASYNC_STATE async, handle_t handle, wchar_t* content)
{
	int cbWrite;
	if (!content)
	{
		printf("Message is empty\n");
		return;
	}
	int size = wcslen(content) * sizeof(wchar_t);
	if (size <= 0)
	{
		printf("Message is empty\n");
		RpcAsyncCompleteCall(async, NULL);
		return;
	}

	WaitForSingleObject(ghMutex, INFINITE);
	if (!WriteFile(hLog, content, size, &cbWrite, NULL))
	{
		printf("WriteFile failed with %d.\n", GetLastError());
	}
	RpcAsyncCompleteCall(async, NULL);
	ReleaseMutex(ghMutex);
}

void rpcInit()
{
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	if (ghMutex == NULL)
	{
		exit(GetLastError());
	}

	RPC_STATUS status;
	CHAR* pszProtocolSequence = "ncalrpc";
	CHAR* pszSecurity = NULL;
	CHAR* pszEndpoint = "4CD673C2-36DB-4868-A14D-CE2CE1E0881F";
	unsigned int    cMinCalls = 1;
	unsigned int    fDontWait = FALSE;

	status = RpcServerUseProtseqEpA(pszProtocolSequence,
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		pszEndpoint,
		pszSecurity);

	if (status) exit(status);

	status = RpcServerRegisterIf(print_v1_0_s_ifspec,
		NULL,
		NULL);

	if (status) exit(status);

	status = RpcServerListen(cMinCalls,
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		fDontWait);

	if (status) exit(status);
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

	rpcInit();

	return 0;
}


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