#pragma once
#include "stdafx.h"

class processlist
{
private:
	DWORD procIDs[1024];
	WCHAR procNames[1024][100];
	INT totalProc = 0;
	BOOL error = false;
public:
	std::vector<int> dekaronPIDs;
	processlist();
	void printProcesses();
	void printDekaronProcs();
	void hydrateList();
};

