#include "gameclient.h"

//=======================================
//		      Constructors
//=======================================

gameclient::gameclient() {
	processId = 0;
}

gameclient::gameclient(DWORD PID)
{
	processId = PID;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, PID);
	hwnd = FindWindowFromProcessId(processId);
	baseAddr = GetModuleBaseAddress(PID, "zdkrc.exe");
	hydrateValues();
	// Check if it's valid process, if it's not then return
	if (maxHealth == 0) return;
	setWindowName();
}



//=======================================
//	        Helper functions
//=======================================

BOOL gameclient::isClientValid()
{
	if (maxHealth == 0) {
		return false;
	}
	else {
		return true;
	}
}

DWORD gameclient::getPID()
{
	return processId;
}

VOID gameclient::hydrateValues()
{
	getCharName(charName);
	maxHealth = getMaxHealth();
	curHealth = getCurHealth();
	maxMana = getMaxMana();
	curMana = getCurMana();
	curSelectedTarget = getCurSelectedTarget();
	charId = getSelfCharId();
}

VOID gameclient::setWindowName()
{
	SetWindowTextA(hwnd, (LPSTR)charName);
}

VOID gameclient::sendKeyStroke(int keycode, int interval) {
	PostMessage(hwnd, WM_KEYDOWN, keycode, 1);
	Sleep(interval);
	PostMessage(hwnd, WM_IME_KEYUP, keycode, 1);
}

VOID gameclient::sendMultipleKeyStrokes(std::vector<int> keys, int interval)
{
	for (size_t i = 0; i < keys.size(); i++){
		PostMessage(hwnd, WM_IME_KEYDOWN, keys[i], 1);
	}
	Sleep(interval);
	for (size_t i = 0; i < keys.size(); i++) {
		PostMessage(hwnd, WM_IME_KEYUP, keys[i], 1);
	}
}



//=======================================
//	     Values Getter Functions
//=======================================

VOID gameclient::getCharName(CHAR* outStr)
{
	char charName[16];
	uintptr_t charNameAddr = FindDMAAddy(hProcess, baseAddr + 0x87A8D4, { 0x4,0x8,0x100,0x4,0x18c });
	ReadProcessMemory(hProcess, (BYTE*)charNameAddr, &charName, sizeof(charName), nullptr);
	for (size_t i = 0; i < sizeof(charName); i++)
	{
		outStr[i] = charName[i];
	}
}

FLOAT gameclient::getMaxHealth() {
	FLOAT result = 0;
	uintptr_t addr = FindDMAAddy(hProcess, baseAddr + offsets.charInfoBaseAddr, offsets.maxHealth);
	ReadProcessMemory(hProcess, (BYTE*)addr, &result, sizeof(result), nullptr);
	return result;
}

FLOAT gameclient::getCurHealth()
{
	FLOAT result = 0;
	uintptr_t addr = FindDMAAddy(hProcess, baseAddr + offsets.charInfoBaseAddr, offsets.curHealth);
	ReadProcessMemory(hProcess, (BYTE*)addr, &result, sizeof(result), nullptr);
	return result;
}

FLOAT gameclient::getMaxMana()
{
	FLOAT result = 0;
	uintptr_t addr = FindDMAAddy(hProcess, baseAddr + offsets.charInfoBaseAddr, offsets.maxMana);
	ReadProcessMemory(hProcess, (BYTE*)addr, &result, sizeof(result), nullptr);
	return result;
}

FLOAT gameclient::getCurMana()
{
	FLOAT result = 0;
	uintptr_t addr = FindDMAAddy(hProcess, baseAddr + offsets.charInfoBaseAddr, offsets.curMana);
	ReadProcessMemory(hProcess, (BYTE*)addr, &result, sizeof(result), nullptr);
	return result;
}

int gameclient::getCurSelectedTarget() {
	int result = 0;
	uintptr_t addr = FindDMAAddy(hProcess, baseAddr + offsets.selectedPlayerBaseAddr, offsets.selecterPlayer);
	ReadProcessMemory(hProcess, (BYTE*)addr, &result, sizeof(result), nullptr);
	return result;
}

int gameclient::getSelfCharId()
{
	int Id = 0;
	sendKeyStroke(69); // "E" key code
	Id = getCurSelectedTarget();
	sendKeyStroke(27);
	return Id;
}



//=======================================
//			Process Functions
//=======================================

void gameclient::healthAutoPot()
{
	if (!settings.autoPotHpEnabled) return;

	float maxHealth = getMaxHealth();
	float curHealth = getCurHealth();
	int percentageHealth = floor(curHealth / maxHealth * 100);
	if (percentageHealth < settings.toleratedHpPercentage) {
		sendKeyStroke(settings.potHpKey);
	}
}

void gameclient::manaAutoPot()
{
	if (!settings.autoPotMpEnabled) return;

	float maxMana = getMaxMana();
	float curMana = getCurMana();
	int percentageMana = floor(curMana / maxMana * 100);
	if (percentageMana < settings.toleratedMpPercentage) {
		sendKeyStroke(settings.potMpKey);
	}
}

void gameclient::doAutoAttack()
{
	if (!settings.autoAttackEnabled) return;
	sendMultipleKeyStrokes(settings.keycodeNextTarget);

	for (unsigned int i = 0; i < settings.skillKeys.size(); i++)
	{
		sendKeyStroke(settings.skillKeys[i], 10);
		Sleep(1);
	}
}

void gameclient::doFollowUser(int userId)
{
	// Point the selected user first to self
	sendMultipleKeyStrokes(settings.keycodeSelfTargeting);
	
	// Set the user that's going to be followed
	uintptr_t selectedPlayerAddr = FindDMAAddy(hProcess, baseAddr + 0x87BD74, { 0xc,0x88 });
	WriteProcessMemory(hProcess,(BYTE*)selectedPlayerAddr, &userId, sizeof(userId), nullptr);

	// Send key stroke to do the follow
	sendMultipleKeyStrokes(settings.keycodeFollow);
}

