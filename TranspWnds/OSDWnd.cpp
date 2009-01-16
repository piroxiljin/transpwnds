#define _WIN32_WINNT  0x0500
#include "OSDWnd.h"

COSDWnd::COSDWnd(void):
	CULWnd()
{
	m_rcPadding.left=30;
	m_rcPadding.top=30;
	m_rcPadding.right=30;
	m_rcPadding.bottom=30;
	MessageMap.AddMessage<COSDWnd>(WM_PAINT,&COSDWnd::OnPaint);
}

COSDWnd::~COSDWnd(void)
{
}

BOOL COSDWnd::Create(HWND hwndParent)
{
	WNDCLASSEX wcex;	    
	wcex.cbSize = sizeof(WNDCLASSEX); 		
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ULOther::ULGetResourceHandle();
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= _T("HINTWNDCLASS");
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);
//	if(!RegisterClassEx(&wcex))
//		return FALSE;

	return CULWnd::CreateEx(WS_EX_LAYERED|WS_EX_TOPMOST,wcex.lpszClassName,NULL,WS_POPUP|WS_VISIBLE,100,10,500,200,hwndParent,NULL);
}
void COSDWnd::ShowText(TCHAR* pszText,enOSDPos osdPos)
{
	m_strText=pszText;


	RECT rc={0,0,0,0};
	CULClientDC dc(*this);

	LOGFONT lf;
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
	lf.lfHeight=-50;
	HFONT hf=::CreateFontIndirect(&lf);
	dc.SelectObject(hf);


	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CALCRECT);
	int nScreenWidth=::GetSystemMetrics(SM_CXFULLSCREEN);
	int nScreenHeight=::GetSystemMetrics(SM_CYFULLSCREEN);
	switch(osdPos)
	{
		case osdpTopLeft:
			{
				MoveWindow(m_rcPadding.left,m_rcPadding.top,
					rc.right+m_rcPadding.right,rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpTopRight:
			{
				MoveWindow(nScreenWidth-rc.right-m_rcPadding.left-m_rcPadding.right,
					m_rcPadding.top,
					rc.right+m_rcPadding.right,
					rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpBotLeft:
			{
				MoveWindow(m_rcPadding.left,nScreenHeight-m_rcPadding.top-rc.bottom+m_rcPadding.bottom,
					rc.right+m_rcPadding.right,rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpBotRight:
			{
				MoveWindow(nScreenWidth-rc.right-m_rcPadding.left-m_rcPadding.right,
					nScreenHeight-m_rcPadding.top-rc.bottom+m_rcPadding.bottom,
					rc.right+m_rcPadding.right,
					rc.bottom+m_rcPadding.bottom,TRUE);
			}
			break;
		case osdpCenter:
			{
				MoveWindow(nScreenWidth/2-rc.right/2-m_rcPadding.left,
					nScreenHeight/2-rc.bottom/2-m_rcPadding.top,
					rc.right+(m_rcPadding.left+m_rcPadding.right),
					rc.bottom+(m_rcPadding.top+m_rcPadding.bottom),TRUE);
			}
			break;	
	}
	InvalidateRect();
}
//===================================
LRESULT COSDWnd::OnPaint(WPARAM,LPARAM)
{
	LOGFONT lf;
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(LOGFONT),&lf);
	lf.lfHeight=-50;
	HFONT hf=::CreateFontIndirect(&lf);



	CULPaintDC dc(*this);

	dc.SelectObject(hf);

	dc.SetBkMode(CULPaintDC::bmTRANSPARENT);
	dc.SetTextColor(0xaaaaaa);
	RECT rc;
	GetClientRect(&rc);
	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CENTER);

	dc.SetTextColor(0xff0000);
	rc.left-=3;
	rc.top-=3;
	rc.right-=3;
	rc.bottom-=3;
	dc.DrawText(m_strText,m_strText.GetLen(),&rc,DT_CENTER);
	return 0;
}