/*
 * Copyright © 2009-2011 Bombizov Alexandr
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
#include "../Include/WorkWnd.h"
#include "../Include/Hook.h"

typedef struct tagRegEntryNames
{
	enHotKeyOperations m_enHotKeyOperation;
	LPCTSTR m_strAltEntryName;
	BOOL m_fAltDefault;
	LPCTSTR m_strCtrlEntryName;
	BOOL m_fCtrlDefault;
	LPCTSTR m_strShiftEntryName;
	BOOL m_fShiftDefault;
	LPCTSTR m_strWinEntryName;
	BOOL m_fWinDefault;
	LPCTSTR m_strMsg1EntryName;
	LPCTSTR m_strMsg2EntryName;
	LPCTSTR m_strMsg3EntryName;
	UINT m_uMsgDefault[3];
}REGENTRYNAMES,*LPREGENTRYNAMES;

static REGENTRYNAMES theRegEntryTable[] =
{
	{hkoTransp
		, _T("TranspAlt"), TRUE
		, _T("TranspCtrl"), FALSE
		, _T("TranspShift"), FALSE
		, _T("TranspWin"), FALSE
		, 0, 0, 0, {WM_MOUSEWHEEL, 0, 0}
	}
	,{hkoTopMost
		, _T("TopMostAlt"), TRUE
		, _T("TopMostCtrl"), TRUE
		, _T("TopMostShift"), FALSE
		, _T("TopMostWin"), FALSE
		, _T("TopMostMsg"), 0, 0, {WM_LBUTTONDOWN, 0, 0}
	}
	,{hkoMoveWnd
		, _T("MoveWndAlt"), TRUE
		, _T("MoveWndCtrl"), FALSE
		, _T("MoveWndShift"), FALSE
		, _T("MoveWndWin"), FALSE
		, _T("MoveWndMsg"), _T("MoveWndMsg1"), _T("MoveWndMsg2"), {WM_LBUTTONDOWN, WM_MOUSEMOVE, WM_LBUTTONUP}
	}
	,{hkoSizeWnd
		, _T("SizeWndAlt"), TRUE
		, _T("SizeWndCtrl"), FALSE
		, _T("SizeWndShift"), FALSE
		, _T("SizeWndWin"), FALSE
		, _T("SizeWndMsg"), _T("SizeWndMsg1"), _T("SizeWndMsg2"), {WM_RBUTTONDOWN, WM_MOUSEMOVE, WM_RBUTTONUP}
	}
	,{hkoToggleCaption
		, _T("ToggleCaptionAlt"), TRUE
		, _T("ToggleCaptionCtrl"), TRUE
		, _T("ToggleCaptionShift"), FALSE
		, _T("ToggleCaptionWin"), FALSE
		, _T("ToggleCaptionMsg"), 0, 0, {WM_RBUTTONDOWN, 0, 0}
	}
	,{hkoToggleCaption
		, _T("ToggleCaptionAlt"), TRUE
		, _T("ToggleCaptionCtrl"), TRUE
		, _T("ToggleCaptionShift"), FALSE
		, _T("ToggleCaptionWin"), FALSE
		, _T("ToggleCaptionMsg"), 0, 0, {WM_RBUTTONDOWN, 0, 0}
	}
};

static INT theRegEntryTableLength = sizeof(theRegEntryTable)/sizeof(theRegEntryTable[0]);

void CWorkWnd::LoadSettings()
{	

	DWORD dwVal=0;
	for (int curItem=0;curItem<theRegEntryTableLength;++curItem)
	{
		enHotKeyOperations curOp=theRegEntryTable[curItem].m_enHotKeyOperation;
		
		// Note! Here we use short-circuit evaluation
		if(theRegEntryTable[curItem].m_strAltEntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strAltEntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_fAltDefault;
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_fAlt=(BOOL)dwVal;

		if(theRegEntryTable[curItem].m_strCtrlEntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strCtrlEntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_fCtrlDefault;
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_fCtrl=(BOOL)dwVal;

		if(theRegEntryTable[curItem].m_strShiftEntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strShiftEntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_fShiftDefault;
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_fShift=(BOOL)dwVal;

		if(theRegEntryTable[curItem].m_strWinEntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strWinEntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_fWinDefault;
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_fWin=(BOOL)dwVal;

		if(theRegEntryTable[curItem].m_strMsg1EntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg1EntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_uMsgDefault[0];
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[0]=dwVal;

		if(theRegEntryTable[curItem].m_strMsg2EntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg2EntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_uMsgDefault[1];
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[1]=dwVal;

		if(theRegEntryTable[curItem].m_strMsg3EntryName == 0
			|| !m_ProfileReg.GetProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg3EntryName,&dwVal))
			dwVal=theRegEntryTable[curItem].m_uMsgDefault[2];
		CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[2]=dwVal;
	}

	//загрузка параметра метода перемещения окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("MoveMethod"),&dwVal))
		CHook::GetHook()->m_MoveMethod=(enMoveSizeMethod)dwVal;
	else
		CHook::GetHook()->m_MoveMethod=msmProgram;
	//загрузка параметра метода изменения размера окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("SizeMethod"),&dwVal))
		CHook::GetHook()->m_SizeMethod=(enMoveSizeMethod)dwVal;
	else
		CHook::GetHook()->m_SizeMethod=msmProgram;
	//загрузка параметра метода изменения размера окна
	if(m_ProfileReg.GetProfileInt(_T("MoveSize"),_T("SizeMethodBy"),&dwVal))
		CHook::GetHook()->m_SizeMethodBy=(enSizeMethodBy)dwVal;
	else
		CHook::GetHook()->m_SizeMethodBy=smbCorner;	
	//загрузка параметров изменения прозрачности
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("MinTranspVal"),&dwVal))
		CHook::GetHook()->m_bMinTranspVal=(BOOL)dwVal;
	else
		CHook::GetHook()->m_bMinTranspVal=100;
	if(m_ProfileReg.GetProfileInt(_T("TranspVals"),_T("TranspStep"),&dwVal))
		CHook::GetHook()->m_bTranspStep=(BYTE)dwVal;
	else
		CHook::GetHook()->m_bTranspStep=10;
	//если вдруг m_bTranspStep стала равна 0
	if(CHook::GetHook()->m_bTranspStep==0)
		CHook::GetHook()->m_bTranspStep=10;
	//загрузка параметров позиционирования OSD окна
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("Align"),&dwVal))
		m_osdWnd.SetPos((COSDWnd::enOSDPos)dwVal);
	else
		m_osdWnd.SetPos(COSDWnd::osdpCenter);
	//загрузка цветов OSD окна
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("TextColor"),&dwVal))
		m_osdWnd.SetTextColor((COLORREF)dwVal);
	else
		m_osdWnd.SetTextColor(0xff0000);
	if(m_ProfileReg.GetProfileInt(_T("OSD"),_T("TextShadowColor"),&dwVal))
		m_osdWnd.SetTextShadowColor((COLORREF)dwVal);
	else
		m_osdWnd.SetTextShadowColor(0xaaaaaa);	
	//загрузка шрифта OSD окна
	LOGFONT lf;
	DWORD dwSizeOfLf=sizeof(lf);
	if(m_ProfileReg.GetProfileBinary(_T("OSD"),_T("Font"),(void*)&lf,&dwSizeOfLf))
		m_osdWnd.SetFont(lf);
	//загрузка параметра обновления
	if(m_ProfileReg.GetProfileInt(_T("Update"),_T("UpdateType"),&dwVal))
		m_Updater.m_UpdateType=(CUpdater::enUpdateType)dwVal;
	else
		m_Updater.m_UpdateType=CUpdater::utAtStartEvery24;
}

void CWorkWnd::SaveSettings()
{
	for (int curItem=0;curItem<theRegEntryTableLength;++curItem)
	{
		enHotKeyOperations curOp=theRegEntryTable[curItem].m_enHotKeyOperation;

		if (theRegEntryTable[curItem].m_strAltEntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strAltEntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_fAlt);

		if (theRegEntryTable[curItem].m_strCtrlEntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strCtrlEntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_fCtrl);

		if (theRegEntryTable[curItem].m_strShiftEntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strShiftEntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_fShift);

		if (theRegEntryTable[curItem].m_strWinEntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strWinEntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_fWin);

		if (theRegEntryTable[curItem].m_strMsg1EntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg1EntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[0]);

		if (theRegEntryTable[curItem].m_strMsg2EntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg2EntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[1]);

		if (theRegEntryTable[curItem].m_strMsg3EntryName != 0)
			m_ProfileReg.WriteProfileInt(_T("HotKeys"),theRegEntryTable[curItem].m_strMsg3EntryName,
				CHook::GetHook()->m_arHotKeyInfo[curOp].m_uMsg[2]);
	}

	//сохранение параметра метода перемещения окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("MoveMethod"),
		CHook::GetHook()->m_MoveMethod);

	//сохранение параметра метода изменения размера окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("SizeMethod"),
		CHook::GetHook()->m_SizeMethod);

	//сохранение параметра метода изменения размера окна
	m_ProfileReg.WriteProfileInt(_T("MoveSize"),_T("SizeMethodBy"),
		CHook::GetHook()->m_SizeMethodBy);

	m_ProfileReg.WriteProfileInt(_T("TranspVals"),_T("MinTranspVal"),
		CHook::GetHook()->m_bMinTranspVal);
	m_ProfileReg.WriteProfileInt(_T("TranspVals"),_T("TranspStep"),
		CHook::GetHook()->m_bTranspStep);
	//сохранение параметров позиционирования OSD окна
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("Align"),(DWORD)m_osdWnd.GetPos());
	//сохранение цветов OSD окна
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("TextColor"),(DWORD)m_osdWnd.GetTextColor());
	m_ProfileReg.WriteProfileInt(_T("OSD"),_T("TextShadowColor"),(DWORD)m_osdWnd.GetTextShadowColor());
	//сохранение шрифта OSD окна
	LOGFONT lf;
	m_osdWnd.GetFont(&lf);
	DWORD dwSizeOfLf=sizeof(lf);
	m_ProfileReg.WriteProfileBinary(_T("OSD"),_T("Font"),(void*)&lf,dwSizeOfLf);
	//сохранение параметра обновления
	m_ProfileReg.WriteProfileInt(_T("Update"),_T("UpdateType"),(DWORD)m_Updater.m_UpdateType);
}