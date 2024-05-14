#pragma once
#include "ExportForm.h"

// ExportPane

class ExportPane : public CDockablePane
{
	DECLARE_DYNAMIC(ExportPane)
public:
	ExportForm* m_wndForm;
public:
	ExportPane();
	virtual ~ExportPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


