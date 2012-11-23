#pragma once

#include "resource.h" 
// CPaneDialog dialog
#define CPropertyGridDlgBase CXTPResizeDialog

class CPaneDialog : public CPropertyGridDlgBase
{
	DECLARE_DYNAMIC(CPaneDialog)

public:
	CPaneDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaneDialog();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MEDIUM };

public:
	CXTPPropertyGrid m_wndPropertyGrid;

	// Implementation
protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};
