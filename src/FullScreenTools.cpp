/*
 * Copyright © 2014 Iljin Alexandr
 * This file is part of TranspWnds.
 *
 * TranspWnds is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TranspWnds is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with TranspWnds.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _WIN32_WINNT 0x0500

#include "../Include/FullScreenTools.h"

#include <cassert>

namespace FullScreenTools
{

void fillMonitorInfo(HWND hWnd, MONITORINFO * lpMonInfo)
{
	HMONITOR hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	GetMonitorInfo(hMon, lpMonInfo);
}

/// \brief Функция определяет область, которую занимает экран монитора.
void getMonitorDesktopRect(HWND hWnd, RECT * lpMonitorRect)
{
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(monInfo);
	fillMonitorInfo(hWnd, &monInfo);
	*lpMonitorRect = monInfo.rcMonitor;
}

bool isFullScreen(HWND hWnd)
{
	WINDOWINFO windowInfo;
	::GetWindowInfo(hWnd, &windowInfo);
	RECT rcDesktop;
	getMonitorDesktopRect(hWnd, &rcDesktop);
	::AdjustWindowRectEx(&rcDesktop, ::GetWindowLong(hWnd,GWL_STYLE), FALSE,
		::GetWindowLong(hWnd,GWL_EXSTYLE));

	return (rcDesktop.bottom == windowInfo.rcWindow.bottom) && 
		(rcDesktop.top == windowInfo.rcWindow.top) && 
		(rcDesktop.left == windowInfo.rcWindow.left) && 
		(rcDesktop.right == windowInfo.rcWindow.right);
}

// Assume that current size and position is a normal ones.
// Notice that often a window unable to be higher than desktop, so we have to hide a caption of the window
// Store current size, position and window style
void setFullScreen(HWND hWnd, DWORD * lpdwNormalStyle, RECT * lprcNormalPosition)
{
	assert(lpdwNormalStyle != 0);
	assert(lprcNormalPosition != 0);
	*lpdwNormalStyle = GetWindowLong(hWnd,GWL_STYLE);
	GetWindowRect(hWnd, lprcNormalPosition);

	::SetWindowLong(hWnd,GWL_STYLE,
		::GetWindowLong(hWnd,GWL_STYLE)&~(WS_CAPTION|WS_THICKFRAME|WS_BORDER));
	::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	{
		int fullScreenLeft = -1;
		int fullScreenTop = -1;
		int fullScreenWidth = -1;
		int fullScreenHeight = -1;
			
		// Let's determine a window size which client rectangle will be placed at fullscreen.
		RECT rcDesktop;
		getMonitorDesktopRect(hWnd, &rcDesktop);
		::AdjustWindowRectEx(&rcDesktop, ::GetWindowLong(hWnd,GWL_STYLE), FALSE,
			::GetWindowLong(hWnd,GWL_EXSTYLE));

		fullScreenLeft = rcDesktop.left;
		fullScreenTop = rcDesktop.top;
		fullScreenWidth = rcDesktop.right - rcDesktop.left;
		fullScreenHeight = rcDesktop.bottom - rcDesktop.top;
		// OK. Now we know the window size, so let's fullscreen this window
		::SetWindowPos(hWnd,0,fullScreenLeft,fullScreenTop,fullScreenWidth,fullScreenHeight,SWP_NOZORDER);
	}
}

void restoreNormalSize(HWND hWnd, DWORD * lpdwNormalStyle, RECT * lprcNormalPosition)
{
	::SetWindowLong(hWnd,GWL_STYLE,
		::GetWindowLong(hWnd,GWL_STYLE)|*lpdwNormalStyle);
	*lpdwNormalStyle=0;

	SetWindowPos(hWnd, HWND_NOTOPMOST, lprcNormalPosition->left, lprcNormalPosition->top, 
		lprcNormalPosition->right - lprcNormalPosition->left ,lprcNormalPosition->bottom - lprcNormalPosition->top,
		0);
	ZeroMemory(lprcNormalPosition, sizeof(*lprcNormalPosition));
}

}; // namespace FullScreenTools