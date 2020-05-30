#pragma once
#include "stdafx.h"

uintptr_t GetModuleBaseAddress(DWORD procId, const WCHAR* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

HWND FindWindowFromProcessId(DWORD dwProcessId);
