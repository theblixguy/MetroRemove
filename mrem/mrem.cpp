/*

 mrem.cpp
 
 Metro Remove: Remove start button and "Metro" UI
 
 || Written by: Suyash Srijan
 || suyashsrijan@outlook.com
 || Tested on: Windows 8.1 Preview Build 9431, Windows 8 RTM
 
 || You are free to use the code anywhere you want but this comment block must not be removed
 
 */

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <SDKDDKVer.h>
#include <VersionHelpers.h>
#include <assert.h>
#define VERSIONHELPERAPI FORCEINLINE BOOL

// Miscellaneous variables //

LONG ret = ERROR_SUCCESS; // Value of ERROR_SUCCESS
BYTE buf = MAX_PATH; // Set value of buffer
int _temp = 0; // Temp variable
KBDLLHOOKSTRUCT kbdStruct; // Keyboard Hook structure
HHOOK _keyHook; // Keyboard Hook handle
MSG _msg; // Temp message
HKEY key = NULL; // Handle to registry key we want to modify

// Window Handles //

HWND _iconHwnd; // Window handle of start button
HWND _iconParentHwnd; // Window handle of start button parent
HWND _tBarListHwnd; // Window handle of taskbar app container
HWND _explorerSysTray; // Window handle of the system tray
HWND _mUIHwnd; // Window handle of the window which has loaded windows.immersiveshell.serviceprovider.dll into memory

// DWORD variables //

DWORD _mUIThreadId; // Id of the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
DWORD _expPID; // PID of explorer process
DWORD dwBufSize = sizeof(buf); // Set size of buffer
DWORD ENABLE_APPS_VIEW = 1;  // Enable apps view value (0x00000001)
DWORD DISABLE_APPS_VIEW = 0;  // Disable apps view value (0x00000000)

// HANDLE variables //

HANDLE _mUI; // Handle of the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
HANDLE _eSThandle; // Handle to explorer.exe


LRESULT WINAPI keyDownEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)) // Keydown detected
	{
		KBDLLHOOKSTRUCT kbdStruct = *(((KBDLLHOOKSTRUCT *) lParam)); // Load lParam data into kbdStruct structure
		if ((kbdStruct.vkCode == VK_LWIN) || (kbdStruct.vkCode == VK_RWIN))  // Windows key pressed
		{
			if (_temp == 0) {
				RegSetValueEx(key, TEXT("MakeAllAppsDefault"), NULL, REG_DWORD, (const BYTE*) &DISABLE_APPS_VIEW, sizeof(DISABLE_APPS_VIEW)); // Set value to 0 (OFF)
				_temp = 1;
			}

			else if (_temp == 1) {
				RegSetValueEx(key, TEXT("MakeAllAppsDefault"), NULL, REG_DWORD, (const BYTE*) &ENABLE_APPS_VIEW, sizeof(ENABLE_APPS_VIEW)); // Set value to 1 (ON)
				_temp = 0;
		}
			return CallNextHookEx(NULL, nCode, wParam, lParam); // Pass info to next hook procedure
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ret = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartPage"), 0, KEY_ALL_ACCESS, &key); // Open registry and get key handle
	
	if (!ret == ERROR_SUCCESS) // Something bad happened
	{
		std::wcout << "Error querying registry [" << GetLastError() << "]";
	}
	
	if (lstrcmpi(argv[1], _T("--killstart")) == 0) { // Kill start button? Sure

		// Checking if you are on 8.1 or not
		assert("\n\nStart Button cannot be killed/hidden since you're not running Windows 8.1", IsWindows8_1OrGreater());
		
		// You're on 8.1, good
		 std::wcout << "Trying to hide Start button"; // Tell user we are about to kill the start menu
		_iconParentHwnd = FindWindow(_T("Shell_TrayWnd"), NULL); // Get the taskbar window handle
		_iconHwnd = FindWindowEx(_iconParentHwnd, NULL, _T("Start"), NULL); // Get the start button handle

		LRESULT _iconHdl = SendMessage(_iconHwnd, WM_CLOSE, NULL, NULL); // Send a WM_CLOSE message to start button
		signed int _reVal = static_cast<int>(_iconHdl); // Store the return value

		if (_reVal == 0) // Yay, the button was successfully hidden
		{
			std::wcout << "\n\nStart button icon was successfully hidden"; 
		}
		else // Looks like the start button is already hidden
			{
				std::wcout << "\n\nFailed to hide the start button. (Error: " << GetLastError() << ")";
				std::getwchar();
			}
		}

	else if (lstrcmpi(argv[1], _T("--killmetro")) == 0) { // Kill the entire metro interface? Sure

		std::wcout << "Trying to kill Modern UI"; // Tell user we are about to kill the modern UI

		_mUIHwnd = FindWindow(_T("ApplicationManager_ImmersiveShellWindow"), NULL); // Get the handle of the immersive shell window
		_mUIThreadId = GetWindowThreadProcessId(_mUIHwnd, NULL); // Get the thread ID
		_mUI = OpenThread(THREAD_TERMINATE, FALSE, _mUIThreadId); // Open the thread and set the TERMINATE_THREAD flag and get its handle
		
		HRESULT _mUIHdl = TerminateThread(_mUI, 0); // Terminate the thread

		signed int _reVal = static_cast<int>(_mUIHdl); // Store the return value
		if (!_reVal == 0) // Yay, metro is completely gone now
		{
			std::wcout << "\n\nModern UI was successfully killed";
		}
		else // Something is wrong
		{
			std::wcout << "\n\nFailed to kill Modern UI (Error: " << GetLastError() << ")";
            std::getwchar();
		}
	}

	else if (lstrcmpi(argv[1], _T("--sbopenav")) == 0) { // Open the Start Screen only when Start button on the keyboard is pressed? Sure
		
		// Checking if you are on 8.1 or not
		assert("\n\nStart button trick cannot be applied since you're not running Windows 8.1", IsWindows8_1OrGreater());

		// You're on 8.1, good
		_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) keyDownEvent, GetModuleHandle(NULL), 0); // Set keyboard hook

		while (GetMessage(&_msg, NULL, 0, 0)) // Loop in order to dispatch and recieve system messages
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}

	else if (lstrcmpi(argv[1], _T("--resexplr")) == 0) {

		// Restart Explorer

		_explorerSysTray = FindWindow(TEXT("Shell_TrayWnd"), NULL); // Find the system tray window and store its handle
		GetWindowThreadProcessId(_explorerSysTray, &_expPID); // Get the PID of the process (explorer.exe) which created the window 
		_eSThandle = OpenProcess(PROCESS_TERMINATE, FALSE, _expPID); // Set the PROCESS_TERMINATE flag in explorer.exe process

		if (_eSThandle) // Valid handle

		{
			TerminateProcess(_eSThandle, 0); // Terminate the explorer.exe process
			Sleep(2000); // Take a nap
			ShellExecute(NULL, NULL, TEXT("cmd.exe /c explorer.exe"), NULL, NULL, SW_HIDE); // Launch a brand sparkling new Explorer process (thanks to Shantanu (@zeusk))
		}

		else // Invalid handle
		
		{
			std::wcout << "Invalid handle, looks like process has already been killed";
			std::getwchar(); 
		}
	}
	
	else { // Invalid command-line arguments specified 

		std::wcout << "Invalid command-line parameter \n\n Usage: \n --killstart : Remove start button \n --killmetro : Remove the Modern UI completely \n --sbopenav : Open the All Apps view only when the start button is pressed \n --resexplr : Restart the explorer process";
        std::getwchar();
		
	}

	RegCloseKey(key); // Close registry key handle
	UnhookWindowsHookEx(_keyHook); // Unhook the keyboard hook
	return 0; // Do I really need to explain what this does?

}



