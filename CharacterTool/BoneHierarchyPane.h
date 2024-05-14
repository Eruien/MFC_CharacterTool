#pragma once
#include "BoneHierarchyForm.h"

// BoneHierarchyPane

class BoneHierarchyPane : public CDockablePane
{
	DECLARE_DYNAMIC(BoneHierarchyPane)
public:
	BoneHierarchyForm* m_HierarchyForm;
public:
	BoneHierarchyPane();
	virtual ~BoneHierarchyPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


