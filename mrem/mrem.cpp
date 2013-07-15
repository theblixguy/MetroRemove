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

HWND _iconHwnd; // Window handle to the start button
HWND _iconParentHwnd; // Window handle to the start button parent
HWND _tBarListHwnd; // Window handle to the taskbar app container
HWND _mUIHwnd; // Window handle which has loaded windows.immersiveshell.serviceprovider.dll into memory
RECT _tBarList; // Taskbar app container's RECT structure 
RECT _icon; // Start button's RECT structure 
DWORD _mUIThreadId; // Id of the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
HANDLE _mUI; // Handle of the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
long _newLength; // New length of the taskbar
DWORD _expPID; // PID of explorer process
HANDLE _eSThandle; // Handle to system tray window

int _tmain(int argc, _TCHAR* argv[])
{
	if (lstrcmpi(argv[1], _T("--killstart")) == 0) { // Kill start button? Sure

		// Checking if you are on 8.1 or not
		assert("\n\nStart Button cannot be killed/hidden since you're not running Windows 8.1", IsWindows8_1OrGreater());
		
		// You're on 8.1, good
		 std::wcout << "Trying to hide Start button"; // Tell user we are about to kill the start menu
		_iconParentHwnd = FindWindow(_T("Shell_TrayWnd"), NULL); // Get the taskbar window handle
		_iconHwnd = FindWindowEx(_iconParentHwnd, NULL, _T("Start"), NULL); // Get the start button handle

		/*

		SUPER BUGGY: I was trying to resize the taskbar so that the empty space left after removing the
		start button gets utilized but the code below is really buggy and crashes the explorer process most of the time 
		so it has been commented out while I am working on a fix.

		_tBarListHwnd = FindWindowEx(_iconParentHwnd, NULL, _T("ReBarWindow32"), NULL);
		GetWindowRect(_tBarListHwnd, &_tBarList);
		GetWindowRect(_iconHwnd, &_icon);
		_newLength = _icon.left + _icon.right;
		MoveWindow(_tBarListHwnd, (_tBarList.left - _newLength), _tBarList.top, ((_tBarList.left + _tBarList.right) + _newLength), ((_tBarList.top + _tBarList.bottom) + _newLength), TRUE);

		*/

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

	else if (lstrcmpi(argv[1], _T("--sbopenav")) == 0) { // Open the All apps view when Start button on the keyboard is pressed? Sure

		 /*
		 
		 Open the all apps view when start button on the keyboard is pressed
		 To do that, we have to hook into the keyboard and capture the WM_KEYDOWN event and
		 call CImmersiveLauncher::OnStartButtonPressed() which is found in twinui.dll and
		 pass IMMERSIVELAUNCHERSHOWMETHOD and IMMERSIVELAUNCHERDISMISSMETHOD arguments. The function is 
		 undocumented so I am studying the library to find out more details about the above parameters.
		 
		 */

		std::wcout << "\n\nWIP"; // Still working
        std::getwchar();
	}

	else if (lstrcmpi(argv[1], _T("--resexplr")) == 0) {

		// Restart Explorer

		HWND _explorerSysTray = FindWindow(TEXT("Shell_TrayWnd"), NULL); // Find the system tray window
		GetWindowThreadProcessId(_explorerSysTray, &_expPID); // Get the explorer.exe PID
		_eSThandle = OpenProcess(PROCESS_TERMINATE, FALSE, _expPID); // Set the PROCESS_TERMINATE flag

		if (_eSThandle)

		{
			TerminateProcess(_eSThandle, 0); // Terminate the explorer.exe process
		}

		Sleep(2000); // Take a nap
		ShellExecute(NULL, NULL, TEXT("cmd.exe /c explorer.exe"), NULL, NULL, SW_HIDE); // Launch a brand sparkling new Explorer process (thanks to Shantanu (@zeusk)
	}
	
	else { // Invalid command-line arguments specified 

		std::wcout << "Invalid command-line parameter \n\n Usage: \n --killstart : Remove start button \n --killmetro : Remove the Modern UI completely \n --sbopenav : Open the All Apps view only when the start button is pressed \n --resexplr : Restart the explorer process";
                std::getwchar();
		
	}
                return 0;
}

