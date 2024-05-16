#include "stdafx.h"
#include "customization_session.h"
#include "session_private_namespace.h"
#include "logger.h"

extern HINSTANCE g_hDllInst;

namespace
{
	typedef int (WINAPI* MESSAGEBOXW)(HWND, LPCWSTR, LPCWSTR, UINT);
	typedef int (WINAPI* MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);
	typedef BOOL(WINAPI* GETUSERNAMEA)(LPSTR, LPDWORD);
	typedef BOOL(WINAPI* GETCOMPUTERNAMEA)(LPSTR, LPDWORD);
	typedef BOOL(WINAPI* GETCOMPUTERNAMEW)(LPWSTR, LPDWORD);

	MESSAGEBOXW pOriginalMessageBoxW;
	MESSAGEBOXA pOriginalMessageBoxA;
	GETUSERNAMEA pOriginalGetUserNameA;
	GETCOMPUTERNAMEA pOriginalGetComputerNameA;
	GETCOMPUTERNAMEW pOriginalGetComputerNameW;

	int WINAPI MessageBoxWHook(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
	{
		int res = pOriginalMessageBoxW(hWnd, lpText, lpCaption, uType);

		WCHAR log[212];
		wsprintfW(log, L"MessageBoxW -- In -- lpCaption: %s @@ uType: %u @@ return: %d\n", lpCaption, uType, res);
		log[211] = 0;
		PRINT(log);

		return res;
	}

	int WINAPI MessageBoxAHook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
	{
		int res = pOriginalMessageBoxA(hWnd, lpText, lpCaption, uType);

		WCHAR log[212];
		wsprintfW(log, L"MessageBoxA -- In -- lpCaption: %hs @@ uType: %u @@ return: %d\n", lpCaption, uType, res);
		log[211] = 0;
		PRINT(log);

		return res;
	}

	BOOL WINAPI GetUserNameAHook(LPSTR lpBuffer, LPDWORD pcbBuffer)
	{
		BOOL res = pOriginalGetUserNameA(lpBuffer, pcbBuffer);

		WCHAR log[212];
		wsprintfW(log, L"GetUserNameA -- @@ Out -- lpBuffer: %hs @@ pcbBuffer: %u @@ return: %d\n", lpBuffer, *pcbBuffer, res);
		log[211] = 0;
		PRINT(log);

		return res;
	}

	BOOL WINAPI GetComputerNameAHook(LPSTR lpBuffer, LPDWORD nSize)
	{
		DWORD originalSize = *nSize;
		BOOL res = pOriginalGetComputerNameA(lpBuffer, nSize);

		WCHAR log[212];
		wsprintfW(log, L"GetComputerNameA -- In -- nSize: %u @@ Out -- lpBuffer: %hs @@ nSize: %u @@ return: %d\n", originalSize, lpBuffer, *nSize, res);
		log[211] = 0;
		PRINT(log);

		return res;
	}

	BOOL WINAPI GetComputerNameWHook(LPWSTR lpBuffer, LPDWORD nSize)
	{
		DWORD originalSize = *nSize;
		BOOL res = pOriginalGetComputerNameW(lpBuffer, nSize);

		WCHAR log[212];
		wsprintfW(log, L"GetComputerNameW -- In -- nSize: %u @@ Out -- lpBuffer: %s @@ nSize: %u @@ return: %d\n", originalSize, lpBuffer, *nSize, res);
		log[211] = 0;
		PRINT(log);

		return res;
	}

	MH_STATUS InitCustomizationHooks()
	{
		MH_STATUS status;
		WCHAR reason[212];

		status = MH_CreateHook(MessageBoxW, MessageBoxWHook, (void**)&pOriginalMessageBoxW);
		if (status == MH_OK) {
			status = MH_QueueEnableHook(MessageBoxW);
			PRINT(L"MessageBoxW Hooked\n");
		}
		else
		{
			wsprintfW(reason, L"MessageBoxW: %d\npid: %d\n", status, _getpid());
			PRINT(reason);
		}

		status = MH_CreateHook(MessageBoxA, MessageBoxAHook, (void**)&pOriginalMessageBoxA);
		if (status == MH_OK) {
			status = MH_QueueEnableHook(MessageBoxA);
			PRINT(L"MessageBoxA Hooked\n");
		}
		else
		{
			wsprintfW(reason, L"MessageBoxA: %d\npid: %d\n", status, _getpid());
			PRINT(reason);
		}

		status = MH_CreateHook(GetUserNameA, GetUserNameAHook, (void**)&pOriginalGetUserNameA);
		if (status == MH_OK) {
			status = MH_QueueEnableHook(GetUserNameA);
			PRINT(L"GetUserNameA Hooked\n");
		}
		else
		{
			wsprintfW(reason, L"GetUserNameA: %d\npid: %d\n", status, _getpid());
			PRINT(reason);
		}

		status = MH_CreateHook(GetComputerNameA, GetComputerNameAHook, (void**)&pOriginalGetComputerNameA);
		if (status == MH_OK) {
			status = MH_QueueEnableHook(GetComputerNameA);
			PRINT(L"GetComputerNameA Hooked\n");
		}
		else
		{
			wsprintfW(reason, L"GetComputerNameA: %d\npid: %d\n", status, _getpid());
			PRINT(reason);
		}

		status = MH_CreateHook(GetComputerNameW, GetComputerNameWHook, (void**)&pOriginalGetComputerNameW);
		if (status == MH_OK) {
			status = MH_QueueEnableHook(GetComputerNameW);
			PRINT(L"GetComputerNameW Hooked\n");
		}
		else
		{
			wsprintfW(reason, L"GetComputerNameW: %d\npid: %d\n", status, _getpid());
			PRINT(reason);
		}

		return status;
	}
}

bool CustomizationSession::Start(bool runningFromAPC, HANDLE sessionManagerProcess, HANDLE sessionMutex) noexcept
{
	auto instance = new (std::nothrow) CustomizationSession();
	if (!instance) {
		LOG(L"Allocation of CustomizationSession failed");
		return false;
	}

	if (!instance->StartAllocated(runningFromAPC, sessionManagerProcess, sessionMutex)) {
		delete instance;
		return false;
	}

	// Instance will free itself.
	return true;
}

bool CustomizationSession::StartAllocated(bool runningFromAPC, HANDLE sessionManagerProcess, HANDLE sessionMutex) noexcept
{
	// Create the session semaphore. This will block the library if another instance
	// (from another session manager process) is already injected and its customization session is active.
	WCHAR szSemaphoreName[sizeof("CustomizationSessionSemaphore-pid=1234567890")];
	swprintf_s(szSemaphoreName, L"CustomizationSessionSemaphore-pid=%u", GetCurrentProcessId());

	HRESULT hr = m_sessionSemaphore.create(1, 1, szSemaphoreName);
	if (FAILED(hr)) {
		LOG(L"Semaphore creation failed with error %08X", hr);
		return false;
	}

	m_sessionSemaphoreLock = m_sessionSemaphore.acquire();

	if (WaitForSingleObject(sessionManagerProcess, 0) != WAIT_TIMEOUT) {
		VERBOSE(L"Session manager process is no longer running");
		return false;
	}

	if (!InitSession(runningFromAPC, sessionManagerProcess)) {
		return false;
	}

	if (runningFromAPC) {
		// Create a new thread for us to allow the program's main thread to run.
		try {
			// Note: Before creating the thread, the CRT/STL bumps the
			// reference count of the module, something a plain CreateThread
			// doesn't do.
			std::thread thread(&CustomizationSession::RunAndDeleteThis, this,
				sessionManagerProcess, sessionMutex);
			thread.detach();
		}
		catch (const std::exception& e) {
			LOG(L"%S", e.what());
			UninitSession();
			return false;
		}
	}
	else {
		// No need to create a new thread, a dedicated thread was created for us
		// before injection.
		RunAndDeleteThis(sessionManagerProcess, sessionMutex);
	}

	return true;
}

bool CustomizationSession::InitSession(bool runningFromAPC, HANDLE sessionManagerProcess) noexcept
{
	MH_STATUS status = MH_Initialize();
	if (status != MH_OK) {
		LOG(L"MH_Initialize failed with %d", status);
		return false;
	}

	if (runningFromAPC) {
		// No other threads should be running, skip thread freeze.
		MH_SetThreadFreezeMethod(MH_FREEZE_METHOD_NONE_UNSAFE);
	}
	else {
		MH_SetThreadFreezeMethod(MH_FREEZE_METHOD_FAST_UNDOCUMENTED);
	}

	try {
		m_newProcessInjector.emplace(sessionManagerProcess);
	}
	catch (const std::exception& e) {
		LOG(L"InitSession failed: %S", e.what());
		m_newProcessInjector.reset();
		MH_Uninitialize();
		return false;
	}

	status = InitCustomizationHooks();
	if (status != MH_OK) {
		LOG(L"InitCustomizationHooks failed with %d", status);
	}

	status = MH_ApplyQueued();
	if (status != MH_OK) {
		LOG(L"MH_ApplyQueued failed with %d", status);
	}

	if (runningFromAPC) {
		MH_SetThreadFreezeMethod(MH_FREEZE_METHOD_FAST_UNDOCUMENTED);
	}

	return true;
}

void CustomizationSession::RunAndDeleteThis(HANDLE sessionManagerProcess, HANDLE sessionMutex) noexcept
{
	m_sessionManagerProcess.reset(sessionManagerProcess);

	if (sessionMutex) {
		m_sessionMutex.reset(sessionMutex);
	}

	// Prevent the system from displaying the critical-error-handler message box.
	// A message box like this was appearing while trying to load a dll in a
	// process with the ProcessSignaturePolicy mitigation, and it looked like this:
	// https://stackoverflow.com/q/38367847
	DWORD dwOldMode;
	SetThreadErrorMode(SEM_FAILCRITICALERRORS, &dwOldMode);

	Run();

	SetThreadErrorMode(dwOldMode, nullptr);

	delete this;
}

void CustomizationSession::Run() noexcept
{
	DWORD waitResult = WaitForSingleObject(m_sessionManagerProcess.get(), INFINITE);
	if (waitResult != WAIT_OBJECT_0) {
		LOG(L"WaitForSingleObject returned %u, last error %u", waitResult, GetLastError());
	}

	VERBOSE(L"Uninitializing and freeing library");

	UninitSession();
}

void CustomizationSession::UninitSession() noexcept
{
	MH_STATUS status = MH_Uninitialize();
	if (status != MH_OK) {
		LOG(L"MH_Uninitialize failed with status %d", status);
	}

	m_newProcessInjector.reset();
}
