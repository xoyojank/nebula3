// ChildView.cpp : implementation of the CChildView class
//

//#include "stdafx.h"
#include "stdneb.h"
#include "editorApp.h"
#include "ChildView.h"
#include "coregraphics/displaydevice.h"
#include "input/win32/win32inputdisplayeventhandler.h"
#include "MainFrm.h"

// CChildView
IMPLEMENT_DYNCREATE(CChildView, CWnd)

using namespace CoreGraphics;
using namespace Math;

CChildView::CChildView() :
inNotifyEventHandlers(false)
{
	Ptr<CoreGraphics::DisplayEventHandler> eventHandler = Win32::Win32InputDisplayEventHandler::Create();
	this->eventHandlers.Append(eventHandler);
}

CChildView::~CChildView()
{
	for (int i=0;i<this->eventHandlers.Size();i++)
	{
		this->eventHandlers[i] = 0;
	}
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
	//	::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	//return TRUE;

	//修改主窗口类名
	WNDCLASS wndclass;
	//GetClassInfo返回注册窗口类信息
	::GetClassInfo(AfxGetInstanceHandle(),cs.lpszClass,&wndclass);
	//重新设置窗口类的类名
	wndclass.lpszClassName=NEBULA3_WINDOW_CLASS;
	//重新注册新的窗口类
	VERIFY(AfxRegisterClass(&wndclass));
	// 利用新注册的类名修改主窗口类名
	cs.lpszClass=wndclass.lpszClassName;
	return TRUE;
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	//if( Win32::Win32DisplayDevice::HasInstance())
	//	return CoreGraphics::DisplayDevice::WinProc(pMsg->hwnd,pMsg->message,pMsg->wParam,pMsg->lParam);
	switch(pMsg->message)
	{
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		this->OnMouseButton(pMsg->message, pMsg->lParam);
		break;
	case WM_MOUSEMOVE:
		this->OnMouseMove(pMsg->lParam);
		break;
	case WM_MOUSEWHEEL:
		this->OnMouseWheel(pMsg->lParam);
		break;
	}
	
	return FALSE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages	
	//this->NotifyEventHandlers(DisplayEvent(DisplayEvent::Paint));
}

void CChildView::OnSetFocus(CWnd* pOldWnd)
{
	this->NotifyEventHandlers(DisplayEvent(DisplayEvent::SetFocus));
	ReleaseCapture();

	CMainFrame* pFrame = (CMainFrame*)this->GetParentFrame();
	pFrame->SetActive(TRUE);
}

void CChildView::OnKillFocus(CWnd* pNewWnd)
{	
	CMainFrame* pFrame = (CMainFrame*)this->GetParentFrame();
	pFrame->SetActive(FALSE);

	this->NotifyEventHandlers(DisplayEvent(DisplayEvent::KillFocus));
	ReleaseCapture();
}

void CChildView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// bypass base class.
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

//void CChildView::OnDraw(CDC* pDC)
//{
//	CMainFrame* pFrame = (CMainFrame*)this->GetParentFrame();
//	pFrame->SetActive(TRUE);
//}

//------------------------------------------------------------------------------
/**
*/
void
CChildView::OnMouseButton(UINT uMsg, LPARAM lParam)
{
	float2 absMousePos = this->ComputeAbsMousePos(lParam);
	float2 normMousePos = this->ComputeNormMousePos(absMousePos);
	HWND hWnd = this->GetSafeHwnd();
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDoubleClick, Input::MouseButton::LeftButton, absMousePos, normMousePos));
		break;

	case WM_RBUTTONDBLCLK:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDoubleClick, Input::MouseButton::RightButton, absMousePos, normMousePos));
		break;

	case WM_MBUTTONDBLCLK:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDoubleClick, Input::MouseButton::MiddleButton, absMousePos, normMousePos));
		break;

	case WM_LBUTTONDOWN:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDown, Input::MouseButton::LeftButton, absMousePos, normMousePos));
		this->SetCapture();
		break;

	case WM_RBUTTONDOWN:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDown, Input::MouseButton::RightButton, absMousePos, normMousePos));
		this->SetCapture();
		break;

	case WM_MBUTTONDOWN:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonDown, Input::MouseButton::MiddleButton, absMousePos, normMousePos));
		this->SetCapture();
		break;

	case WM_LBUTTONUP:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonUp, Input::MouseButton::LeftButton, absMousePos, normMousePos));
		ReleaseCapture();
		break;

	case WM_RBUTTONUP:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonUp, Input::MouseButton::RightButton, absMousePos, normMousePos));
		ReleaseCapture();
		break;

	case WM_MBUTTONUP:
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseButtonUp, Input::MouseButton::MiddleButton, absMousePos, normMousePos));
		ReleaseCapture();
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
CChildView::OnMouseMove(LPARAM lParam)
{
	float2 absMousePos = this->ComputeAbsMousePos(lParam);
	float2 normMousePos = this->ComputeNormMousePos(absMousePos);
	this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseMove, absMousePos, normMousePos));
}

//------------------------------------------------------------------------------
/**
*/
void
CChildView::OnMouseWheel(WPARAM wParam)
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	if (zDelta > 0)
	{
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseWheelForward));
	}
	else
	{
		this->NotifyEventHandlers(DisplayEvent(DisplayEvent::MouseWheelBackward));
	}
}

//------------------------------------------------------------------------------
/**
    Attach an event handler to the display device.
*/
void
CChildView::AttachEventHandler(const Ptr<DisplayEventHandler>& handler)
{
    n_assert(handler.isvalid());
    n_assert(InvalidIndex == this->eventHandlers.FindIndex(handler));
    n_assert(!this->inNotifyEventHandlers);
    this->eventHandlers.Append(handler);
    handler->OnAttach();
}

//------------------------------------------------------------------------------
/**
    Remove an event handler from the display device.
*/
void
CChildView::RemoveEventHandler(const Ptr<DisplayEventHandler>& handler)
{
    n_assert(handler.isvalid());
    n_assert(!this->inNotifyEventHandlers);
    IndexT index = this->eventHandlers.FindIndex(handler);
    n_assert(InvalidIndex != index);
    this->eventHandlers.EraseIndex(index);
    handler->OnRemove();
}

//------------------------------------------------------------------------------
/**
    Notify all event handlers about an event.
*/
bool
CChildView::NotifyEventHandlers(const DisplayEvent& event)
{
    n_assert(!this->inNotifyEventHandlers);
    bool handled = false;
    this->inNotifyEventHandlers = true;
    IndexT i;
    for (i = 0; i < this->eventHandlers.Size(); i++)
    {
        handled |= this->eventHandlers[i]->HandleEvent(event);
    }
    this->inNotifyEventHandlers = false;
    return handled;
}

//------------------------------------------------------------------------------
/**    
*/
float2
CChildView::ComputeAbsMousePos(LPARAM lParam) const
{
	return float2(float(short(LOWORD(lParam))), float(short(HIWORD(lParam))));
}

//------------------------------------------------------------------------------
/**    
*/
float2
CChildView::ComputeNormMousePos(const float2& absMousePos) const
{
	float2 normMousePos;
	RECT clientRect = { 0 };
	HWND hWnd = this->GetSafeHwnd();
	GetClientRect(&clientRect);LONG w = n_max(clientRect.right - clientRect.left, 1);
	LONG h = n_max(clientRect.bottom - clientRect.top, 1);
	normMousePos.set(absMousePos.x() / float(w), absMousePos.y() / float(h));
	return normMousePos;
}
