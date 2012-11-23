// ChildView.h : interface of the CChildView class
//


#pragma once
#include "coregraphics/displayeventhandler.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();
	DECLARE_DYNCREATE(CChildView)

// Attributes
public:
	//virtual void OnDraw(CDC* pDC);    // overridden to draw this view

// Operations
public:
	/// attach a display event handler
	void AttachEventHandler(const Ptr<CoreGraphics::DisplayEventHandler>& h);
	/// remove a display event handler
	void RemoveEventHandler(const Ptr<CoreGraphics::DisplayEventHandler>& h);

public:
	/// notify event handlers about an event
	bool NotifyEventHandlers(const CoreGraphics::DisplayEvent& e);

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	DECLARE_MESSAGE_MAP()

protected:/// compute absolute mouse position from lParam
	Math::float2 ComputeAbsMousePos(LPARAM lParam) const;
	/// compute normalized mouse position from absolute mouse pos
	Math::float2 ComputeNormMousePos(const Math::float2& absMousePos) const;
	/// called on mouse button event
	virtual void OnMouseButton(UINT uMsg, LPARAM lParam);
	/// called on WM_MOUSEMOVE
	virtual void OnMouseMove(LPARAM lParam);
	/// called on WM_MOUSEWHEEL
	virtual void OnMouseWheel(WPARAM wParam);

private:
	Util::Array<Ptr<CoreGraphics::DisplayEventHandler>> eventHandlers;
	bool inNotifyEventHandlers;
};
