#include <stdio.h>
#include <zmq.h>
#include <Windows.h>

HANDLE hLog;
void* context, * server;

void zmqRecv()
{
	zmq_msg_t msg;
	DWORD cbWrite;

	do
	{
		zmq_msg_init(&msg);
		int size = zmq_msg_recv(&msg, server, 0);
		if (size == -1)
		{
			printf("zmq_msg_recv failed with %d.\n", zmq_errno());
			return;
		}
		wchar_t* buffer = (wchar_t*)zmq_msg_data(&msg);
		size = zmq_msg_size(&msg);
		if (!WriteFile(hLog,buffer,size,&cbWrite,NULL))
		{
			printf("WriteFile failed with %d.\n", GetLastError());
		}
		zmq_msg_close(&msg);
	} while (true);
}

void zmqInit()
{
	context = zmq_ctx_new();
	server = zmq_socket(context, ZMQ_PULL);
	int rc = zmq_bind(server, "tcp://127.0.0.1:54124");
	if (rc != 0)
		exit(1);
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

	zmqInit();
	zmqRecv();

	CloseHandle(hLog);
	zmq_close(server);
	zmq_ctx_destroy(context);
	return 0;
}