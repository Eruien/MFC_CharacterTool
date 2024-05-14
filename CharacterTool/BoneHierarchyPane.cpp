// BoneHierarchyPane.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "BoneHierarchyPane.h"


// BoneHierarchyPane

IMPLEMENT_DYNAMIC(BoneHierarchyPane, CDockablePane)

BoneHierarchyPane::BoneHierarchyPane()
{

}

BoneHierarchyPane::~BoneHierarchyPane()
{
}


BEGIN_MESSAGE_MAP(BoneHierarchyPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// BoneHierarchyPane 메시지 처리기




int BoneHierarchyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_HierarchyForm = BoneHierarchyForm::CreateOne(this);
	
	return 0;
}


void BoneHierarchyPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_HierarchyForm)
	{
		m_HierarchyForm->SetWindowPos(
			NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
	
}


int BoneHierarchyPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd* pParentFrame = GetParentFrame();

	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
