/* 
mrem.cpp

Metro Remove: Remove start menu and "Metro" UI

|| Written by: Suyash Srijan
|| suyashsrijan@outlook.com
|| Tested on: Windows 8.1 Preview Build 9431 (should work on Windows 8 as well, but I have not tested it)

|| You are free to use the code anywhere you want but this comment block must not be removed

*/

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <SDKDDKVer.h>

    HWND _iconHwnd; // Handle to the start button
	HWND _iconParentHwnd; // Handle to the start button parent
	HWND _tBarListHwnd; // Handle to the taskbar app container
	HWND _mUIHwnd; // Handle to the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
	RECT _tBarList; // Taskbar app container's RECT structure 
	RECT _icon; // Start button's RECT structure 
	DWORD _mUIThreadId; // Id of the thread which has loaded windows.immersiveshell.serviceprovider.dll into memory
	long _newLength; // New length of the taskbar

int _tmain(int argc, _TCHAR* argv[])
{
	if (lstrcmpi(argv[1], _T("--killstart")) == 0) { // Kill start button? Sure

		 std::wcout << "Trying to hide Start button"; // Tell user we are about to kill the start menu
		_iconParentHwnd = FindWindow(_T("Shell_TrayWnd"), NULL); // Get the taskbar window handle
		_iconHwnd = FindWindowEx(_iconParentHwnd, NULL, _T("Start"), NULL); // Get the start button handle

		/*

		SUPER BUGGY: The following code might not work correctly and crash the explorer process 
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
			std::getwchar(); 
		}
		else // Looks like the start button is already hidden
		{
			std::wcout << "\n\nFailed to hide the start button. (The start button may already be hidden)"; 
			std::getwchar();
		}
	}
	else if (lstrcmpi(argv[1], _T("--killmetro")) == 0) { // Kill the entire metro interface? Sure

		std::wcout << "Trying to kill Modern UI"; // Tell user we are about to kill the modern UI

		_mUIHwnd = FindWindow(_T("ApplicationManager_DesktopShellWindow"), NULL); // Get the handle of the thread which controls the UI
		GetWindowThreadProcessId(_mUIHwnd, &_mUIThreadId); // Get the thread ID
		OpenThread(0x0001, 0, _mUIThreadId); // Open the thread and set the TERMINATE_THREAD flag
		
		HRESULT _mUIHdl = TerminateThread(_mUIHwnd, 0); // Terminate the thread

		signed int _reVal = static_cast<int>(_mUIHdl); // Store the return value
		if (!_reVal == 0) // Yay, metro is completely gone now
		{
			std::wcout << "\n\nModern UI was successfully killed";
			std::getwchar();
		}
		else // Looks like you have already killed metro before
		{
			std::wcout << "\n\nFailed to kill Modern UI";
			std::getwchar();
		}
	}

	else if (lstrcmpi(argv[1], _T("--sbopenav")) == 0) { // Open the All apps view when Start button on the keyboard is pressed? Sure

		 /*
		 
		 Open the all apps view when start button on the keyboard is pressed
		 To do that, we have to hook into the keyboard and capture the WM_KEYDOWN event and
		 call CImmersiveLauncher::OnStartButtonPressed() which is found in twinui.dll and
		 pass IMMERSIVELAUNCHERSHOWMETHOD and IMMERSIVELAUNCHERDISMISSMETHOD arguments. The function is undocumented so 
		 I am studying the library to find out more details about the above parameters.
		 
		 */

		std::wcout << "\n\nWIP"; // Still working
		std::getwchar();
	}

	else {
		std::wcout << "Invalid command-line parameter \n\n Usage: \n --killstart : Remove start button \n --killmetro : Remove the Modern UI completely \n --sbopenav : Open the All Apps view only when the start button is pressed \n";
		std::getwchar();
	}

}

