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
#include "../Include/PropHotKeys.h"
#include "../res/resource.h"

typedef struct tagDialogControls
{
	enHotKeyOperations m_enHotKeyOperation;
	int m_idcCheckEnabled;
	int m_idcCheckAlt;
	int m_idcCheckCtrl;
	int m_idcCheckShift;
	int m_idcCheckWin;
	int m_idcComboMsg;
	BOOL m_fUpdateMsg2;
}DIALOGCONTROLS, *LPDIALOGCONTROLS;

const static DIALOGCONTROLS theDialgControlsTable[] = 
{
	{hkoTransp,
		0, // checkbox enabled
		IDC_CHECK_TRANSP_ALT, // checkbox alt
		IDC_CHECK_TRANSP_CTRL, // checkbox ctrl
		IDC_CHECK_TRANSP_SHIFT, // checkbox shift
		IDC_CHECK_TRANSP_WIN, // checkbox win
		0, // combobox messages
		FALSE, // update m_uMsg[2]
	},
	{hkoTopMost,
		0, // checkbox enabled
		IDC_CHECK_TOPMOST_ALT, // checkbox alt
		IDC_CHECK_TOPMOST_CTRL, // checkbox ctrl
		IDC_CHECK_TOPMOST_SHIFT, // checkbox shift
		IDC_CHECK_TOPMOST_WIN, // checkbox win
		IDC_COMBO_TOPMOST_MSG, // combobox messages
		FALSE, // update m_uMsg[2]
	},
	{hkoMoveWnd,
		0, // checkbox enabled
		IDC_CHECK_MOVE_ALT, // checkbox alt
		IDC_CHECK_MOVE_CTRL, // checkbox ctrl
		IDC_CHECK_MOVE_SHIFT, // checkbox shift
		IDC_CHECK_MOVE_WIN, // checkbox win
		IDC_COMBO_MOVE_MSG, // combobox messages
		TRUE, // update m_uMsg[2]
	},
	{hkoSizeWnd,
		0, // checkbox enabled
		IDC_CHECK_SIZE_ALT, // checkbox alt
		IDC_CHECK_SIZE_CTRL, // checkbox ctrl
		IDC_CHECK_SIZE_SHIFT, // checkbox shift
		IDC_CHECK_SIZE_WIN, // checkbox win
		IDC_COMBO_SIZE_MSG, // combobox messages
		TRUE, // update m_uMsg[2]
	},
	{hkoToggleCaption,
		0, // checkbox enabled
		IDC_CHECK_TOGGLECAPTION_ALT, // checkbox alt
		IDC_CHECK_TOGGLECAPTION_CTRL, // checkbox ctrl
		IDC_CHECK_TOGGLECAPTION_SHIFT, // checkbox shift
		IDC_CHECK_TOGGLECAPTION_WIN, // checkbox win
		IDC_COMBO_TOGGLECAPTION_MSG, // combobox messages
		FALSE, // update m_uMsg[2]
	},
	{hkoToggleFullScreen,
		IDC_CHECK_TOGGLEFULLSCREEN_ENABLE, // checkbox enabled
		IDC_CHECK_TOGGLEFULLSCREEN_ALT, // checkbox alt
		IDC_CHECK_TOGGLEFULLSCREEN_CTRL, // checkbox ctrl
		IDC_CHECK_TOGGLEFULLSCREEN_SHIFT, // checkbox shift
		IDC_CHECK_TOGGLEFULLSCREEN_WIN, // checkbox win
		IDC_COMBO_TOGGLEFULLSCREEN_MSG, // combobox messages
		FALSE, // update m_uMsg[2]
	},
};

const static INT theDialgControlsTableLength = sizeof(theDialgControlsTable)/sizeof(theDialgControlsTable[0]);

CPropHotKeys::CPropHotKeys(void):
	CULPropPage()
{
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORSTATIC,&CPropHotKeys::OnCtlColor);
//	MessageMap.AddMessage<CPropHotKeys>(WM_CTLCOLORBTN,&CPropHotKeys::OnCtlColor);
}

CPropHotKeys::~CPropHotKeys(void)
{
}

LRESULT CPropHotKeys::OnInitDialog(WPARAM w,LPARAM l)
{
	for (int curItem=0;curItem<theDialgControlsTableLength;++curItem)
	{
		enHotKeyOperations curOp=theDialgControlsTable[curItem].m_enHotKeyOperation;
		if (theDialgControlsTable[curItem].m_idcCheckEnabled != 0)
			SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckEnabled,BM_SETCHECK,
				m_arHotkey[curOp].m_fEnabled,0);
		if (theDialgControlsTable[curItem].m_idcCheckAlt != 0)
			SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckAlt,BM_SETCHECK,
				m_arHotkey[curOp].m_fAlt,0);
		if (theDialgControlsTable[curItem].m_idcCheckCtrl != 0)
			SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckCtrl,BM_SETCHECK,
				m_arHotkey[curOp].m_fCtrl,0);
		if (theDialgControlsTable[curItem].m_idcCheckShift != 0)
			SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckShift,BM_SETCHECK,
				m_arHotkey[curOp].m_fShift,0);
		if (theDialgControlsTable[curItem].m_idcCheckWin != 0)
			SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckWin,BM_SETCHECK,
				m_arHotkey[curOp].m_fWin,0);

		if (theDialgControlsTable[curItem].m_idcComboMsg != 0)
		{
			CULComboBox cbItemMsg;
			cbItemMsg.Attach(GetDlgItem(theDialgControlsTable[curItem].m_idcComboMsg));
			int nItem=cbItemMsg.AddString(CULStrTable(IDS_OPTOINS_COMBO_LBTN));
			cbItemMsg.SetItemData(nItem,WM_LBUTTONDOWN);
			nItem=cbItemMsg.AddString(CULStrTable(IDS_OPTOINS_COMBO_RBTN));
			cbItemMsg.SetItemData(nItem,WM_RBUTTONDOWN);
			nItem=cbItemMsg.AddString(CULStrTable(IDS_OPTOINS_COMBO_MBTN));
			cbItemMsg.SetItemData(nItem,WM_MBUTTONDOWN);
			for(int i=0;i<cbItemMsg.GetCount();++i)
				if(m_arHotkey[curOp].IsMsg(0,cbItemMsg.GetItemData(i)))
				{
					cbItemMsg.SetCurSel(i);
					break;
				}
			cbItemMsg.Detach();
		}
	}

	return CULPropPage::OnInitDialog(w,l);
}

LRESULT CPropHotKeys::OnCtlColor(WPARAM w,LPARAM l)
{
	if(::GetDlgCtrlID((HWND)l)==(short)-1)
	{
		TCHAR szClass[50];
		if(::GetClassName((HWND)l,szClass,50))
			if(_tcsicmp(szClass,_T("BUTTON"))==0)
				return 0;
	}
	HDC hDC=(HDC)w;
	::SetBkMode(hDC,TRANSPARENT);
	return (LRESULT)::GetStockObject(NULL_BRUSH);
}

LRESULT CPropHotKeys::OnApply(BYTE nReturn)
{
	for (int curItem=0;curItem<theDialgControlsTableLength;++curItem)
	{
		enHotKeyOperations curOp=theDialgControlsTable[curItem].m_enHotKeyOperation;

		if (theDialgControlsTable[curItem].m_idcCheckEnabled != 0)
			m_arHotkey[curOp].m_fEnabled=(SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckEnabled,
				BM_GETCHECK,0,0)!=0);
		if (theDialgControlsTable[curItem].m_idcCheckAlt != 0)
			m_arHotkey[curOp].m_fAlt=(SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckAlt,
				BM_GETCHECK,0,0)!=0);
		if (theDialgControlsTable[curItem].m_idcCheckCtrl != 0)
			m_arHotkey[curOp].m_fCtrl=(SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckCtrl,
				BM_GETCHECK,0,0)!=0);
		if (theDialgControlsTable[curItem].m_idcCheckShift != 0)
			m_arHotkey[curOp].m_fShift=(SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckShift,
				BM_GETCHECK,0,0)!=0);
		if (theDialgControlsTable[curItem].m_idcCheckWin != 0)
			m_arHotkey[curOp].m_fWin=(SendDlgItemMessage(theDialgControlsTable[curItem].m_idcCheckWin,
				BM_GETCHECK,0,0)!=0);
	
		if (theDialgControlsTable[curItem].m_idcComboMsg != 0)
		{
			CULComboBox cbItemMsg;
			cbItemMsg.Attach(GetDlgItem(theDialgControlsTable[curItem].m_idcComboMsg));
			m_arHotkey[curOp].m_uMsg[0]=cbItemMsg.GetItemData(cbItemMsg.GetCurSel());

			if (theDialgControlsTable[curItem].m_fUpdateMsg2)
			{
				switch(m_arHotkey[curOp].m_uMsg[0])
				{
				case WM_LBUTTONDOWN:
					m_arHotkey[curOp].m_uMsg[2] = WM_LBUTTONUP;
					break;
				case WM_MBUTTONDOWN:
					m_arHotkey[curOp].m_uMsg[2] = WM_MBUTTONUP;
					break;
				case WM_RBUTTONDOWN:
					m_arHotkey[curOp].m_uMsg[2] = WM_RBUTTONUP;
					break;
				}
			}
			cbItemMsg.Detach();
		}
	}

	return CULPropPage::OnApply(nReturn);
}
