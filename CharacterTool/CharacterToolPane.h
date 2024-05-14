#pragma once
#include "CharacterToolForm.h"

// CharacterToolPane

class CharacterToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(CharacterToolPane)
public:
	CharacterToolForm* m_wndForm;
public:
	CharacterToolPane();
	virtual ~CharacterToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


