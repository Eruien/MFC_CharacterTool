// ExportPane.cpp: 구현 파일
//

#include "pch.h"
#include "CharacterTool.h"
#include "ExportPane.h"


// ExportPane

IMPLEMENT_DYNAMIC(ExportPane, CDockablePane)

ExportPane::ExportPane()
{

}

ExportPane::~ExportPane()
{
}


BEGIN_MESSAGE_MAP(ExportPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// ExportPane 메시지 처리기

int ExportPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndForm = ExportForm::CreateOne(this);

	return 0;
}

void ExportPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(
			NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}

int ExportPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();

	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	return  MA_NOACTIVATE;
}


