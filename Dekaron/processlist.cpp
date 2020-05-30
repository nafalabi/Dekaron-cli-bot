#include "process.h"
#include "processlist.h"

// Constructor
processlist::processlist()
{
	hydrateList();
}

// Gather all the processes in the machine
// Store the list of PID and name of the processes to object property
void processlist::hydrateList()
{
	// Set initial value of object properties
	totalProc = 0;
	dekaronPIDs.clear();

	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		error = true;
	}
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);
	// Print the name and process identifier for each process.
	int listIndex = 0;
	for (i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0)
		{
			//Process Name declaration
			WCHAR procName[100] = L"<unknown>";
			// Get a handle to the process.
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
			// Get the process name.
			if (NULL != hProcess) {
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					GetModuleBaseNameW(hProcess, hMod, procName, sizeof(procName) / sizeof(WCHAR));
				}
			}

			//Copying the data to the public property
			procIDs[listIndex] = aProcesses[i];
			//lstrcpyW(procNames[listIndex], procName);
			wcscpy_s(procNames[listIndex], procName);
			//Counting total process
			totalProc += 1;
			listIndex += 1;
			// If it's dekaron procs the add to the list
			if (wcscmp(procName, L"zdkrc.exe") == 0) {
				dekaronPIDs.push_back(aProcesses[i]);
			}
			// Release the handle to the process.
			CloseHandle(hProcess);
		}
	}
}

// Print out all the processes to the console
void processlist::printProcesses()
{
	// Refresh process list
	hydrateList();

	int printIndex = 1;
	for (int i = 0; i < totalProc; i++)
	{
		WCHAR procName[100];
		lstrcpyW(procName, procNames[i]);
		if (wcscmp(procName, L"<unknown>")) {
			wprintf(L"\t%d. PID %d (%ls)\n",printIndex,procIDs[i],procName);
			printIndex += 1;
		}
	}
}

// Print out all the Dekaron processes to the console
void processlist::printDekaronProcs() {
	// Refresh process list
	hydrateList();

	int printIndex = 1;
	for (int i = 0; i < totalProc; i++)
	{
		WCHAR procName[100];
		lstrcpyW(procName, procNames[i]);
		if ( wcscmp(procName, L"zdkrc.exe") == 0) {
			wprintf(L"\t%d. PID %d (%ls)\n", printIndex, procIDs[i], procName);
			printIndex += 1;
		}
	}
}
